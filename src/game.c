/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "screen.h"
#include "i386.h"
#include "sched.h"

void inicializar_juego() {
	unsigned int i;
	unsigned int j;
	for(j = 0; j<=MAX_Y; j++) {
		for(i = 0; i<=MAX_X; i++) {
			mapa[j][i].dirty = 0;
			mapa[j][i].infectado_por = POS_INVALIDA;
			mapa[j][i].tarea_de = POS_INVALIDA;
			mapa[j][i].pagina_de = POS_INVALIDA;
			mapa[j][i].tssIdx = POS_INVALIDA;
		}
	}

	for(i = 0; i<15; i++) {
		game_lanzar_aux(2, (i*2)+20, 20);
	}

	pantalla();
}



void game_mover_cursor(int jugador, direccion dir) {
	JUGADOR_INFO* j = jugador == 0 ? &jugador_a : &jugador_b;
	
	if((dir == IZQ && j->x == 0) || (dir == DER && j->x == MAX_X) ||
	   (dir == ARB && j->y == 0) || (dir == ABA && j->y == MAX_Y) ) {
		return;
	}

	mapa[j->y][j->x].dirty = 1;

	switch(dir) {
		case IZQ:
			j->x--;
		break;
		case DER:
			j->x++;
		break;
		case ARB:
			j->y--;
		break;
		case ABA:
			j->y++;
		break;

	}


}



void game_matar_tarea() {
	unsigned short tssIdx = obtenerIdxTssTareaActual();
	unsigned short x;
	unsigned short y;
	obtenerPosicionTarea(tssIdx, &x, &y);

	if(mapa[y][x].infectado_por == 0) {
		jugador_a.puntos--;
	} else if(mapa[y][x].infectado_por == 1) {
		jugador_b.puntos--;
	}

	if(mapa[y][x].tarea_de == 0) {
		jugador_a.tareasActivas--;
	} else if(mapa[y][x].tarea_de == 1) {
		jugador_b.tareasActivas--;
	}
	mapa[y][x].dirty = 1;
	unsigned short pagAnteriorX;
	unsigned short pagAnteriorY;
	matarTarea(mapa[y][x].tarea_de, mapa[y][x].tssIdx, &pagAnteriorX, &pagAnteriorY);
	mapa[y][x].tarea_de = POS_INVALIDA;
	mapa[y][x].tssIdx = POS_INVALIDA;
	mapa[y][x].infectado_por = POS_INVALIDA;

	if(pagAnteriorY != POS_INVALIDA && pagAnteriorX != POS_INVALIDA) {
		unsigned int cr3 = rcr3();
		mmu_unmapear_pagina(INICIO_VIRTUAL_MAPA + PAGE_SIZE, cr3);
		mapa[pagAnteriorY][pagAnteriorX].dirty = 1;
		mapa[pagAnteriorY][pagAnteriorX].pagina_de = getJugadoresPagina(pagAnteriorX, pagAnteriorY);
	}
}

 
void game_lanzar_aux(unsigned int jugador, unsigned int x, unsigned int y) {

	int offset = ((y * SIZE_W) + x) * 0x1000;

	//sumamos uno porque la primera tarea es la idle
	//(y el jugador 3 es la 3)
	addr dir = mmu_inicalizar_dir_tarea(jugador + 1, offset);

	unsigned int tssIdx = lanzarTarea(jugador);
	tss_inicializar_tarea(tssIdx, dir);

	if(mapa[y][x].tarea_de != POS_INVALIDA) {
		//Aca podriamos necesitar slot_de ademas de tarea_de si se lanzan en verdes
		if(mapa[y][x].infectado_por == 0) {
			jugador_a.puntos--;
		} else if(mapa[y][x].infectado_por == 1) {
			jugador_b.puntos--;
		}

		if(mapa[y][x].tarea_de == 0) {
			jugador_a.tareasActivas--;
		} else if(mapa[y][x].tarea_de == 1) {
			jugador_b.tareasActivas--;
		}

		unsigned short pagAnteriorX;
		unsigned short pagAnteriorY;
		matarTarea(mapa[y][x].tarea_de, mapa[y][x].tssIdx, &pagAnteriorX, &pagAnteriorY);

		if(pagAnteriorY != POS_INVALIDA && pagAnteriorX != POS_INVALIDA) {
			unsigned int cr3 = rcr3();
			mmu_unmapear_pagina(INICIO_VIRTUAL_MAPA + PAGE_SIZE, cr3);
			mapa[pagAnteriorY][pagAnteriorX].dirty = 1;
			mapa[pagAnteriorY][pagAnteriorX].pagina_de = getJugadoresPagina(pagAnteriorX, pagAnteriorY);
		}

	} 

	mapa[y][x].tarea_de = jugador;
	mapa[y][x].tssIdx = tssIdx;
	mapa[y][x].dirty = 1;

#ifdef DEBUG
	print_int(y, 50, 0, C_FG_WHITE);
	print_int(x, 55, 0, C_FG_WHITE);
	print_int(tssIdx, 60, 0, C_FG_WHITE);
	print_hex(dir, 8, 70, 0, C_FG_WHITE);
#endif


}




void game_lanzar(unsigned int jugador) {
	//breakpoint();

	JUGADOR_INFO* j = jugador == 0 ? &jugador_a : &jugador_b;
	if(j->vidas == 0) return;
	if(j->tareasActivas == 5) return;
	j->vidas--;
	j->puntos++;
	game_lanzar_aux(jugador, j->x, j->y);
	j->tareasActivas++;
	mapa[j->y][j->x].infectado_por = jugador;

}

void invalid_syscall() {
	game_matar_tarea();
}


unsigned short obtenerIdxTssTareaActual() {
	//selector de segmento de tss
	unsigned short trActual = rtr() / 8;
	//indice en tabla gdt
	unsigned short tssIdx = trActual - INDICE_PRIMER_TSS_EN_GDT;
#ifdef DEBUG
	print_int(tssIdx, 5, 0, C_FG_WHITE);
#endif
	return tssIdx;
}

void obtenerPosicionTarea(unsigned short tssIdx, unsigned short* x, unsigned short* y) {
	unsigned short i;
	unsigned short j;
	for(i = 0; i < SIZE_H; i++) {
		for(j = 0; j< SIZE_W; j++) {
			if(mapa[i][j].tssIdx == tssIdx) {
				*y = i;
				*x = j;
				return;
			}
		}
	}
	*y = POS_INVALIDA;
	*x = POS_INVALIDA;
	return;
} 

addr obtenerDireccionFisica(unsigned short tssIdx, unsigned short* x, unsigned short* y) {
	obtenerPosicionTarea(tssIdx, x, y);
	if((*x) == POS_INVALIDA) return 0;
	int offset = (((*y) * SIZE_W) + (*x)) * 0x1000;	
	return INICIO_FISICO_MAPA + offset;
}


void game_soy(unsigned int yoSoy) {
	unsigned short tssIdx = obtenerIdxTssTareaActual();
	unsigned short x;
	unsigned short y;
	obtenerPosicionTarea(tssIdx, &x, &y);
	if(x == POS_INVALIDA || y == POS_INVALIDA) {
		invalid_syscall();
		return;
	}

	if(yoSoy == VIRUS_ROJO) {
		if(mapa[y][x].infectado_por != 0) {
			mapa[y][x].dirty = 1;
			jugador_a.puntos++;
		}
		if(mapa[y][x].infectado_por == 1) {
			jugador_b.puntos--;
		}
		mapa[y][x].infectado_por = 0;
	} else if(yoSoy == VIRUS_AZUL) {
		if(mapa[y][x].infectado_por != 1) {
			mapa[y][x].dirty = 1;
			jugador_b.puntos++;
		}
		if(mapa[y][x].infectado_por == 0) {
			jugador_a.puntos--;
		}
		mapa[y][x].infectado_por = 1;
	} else {
		invalid_syscall();
	}

	pantalla();

}

void game_donde(short* pos) {
	//se recibe direccion virtual
	addr p = (addr)pos;
	if(p < INICIO_VIRTUAL_MAPA || p + (sizeof(short)*2) > INICIO_VIRTUAL_MAPA + PAGE_SIZE - 1) {
		invalid_syscall();
		return;
	}

	unsigned short x;
	unsigned short y;
	addr fisica = obtenerDireccionFisica(obtenerIdxTssTareaActual(), &x, &y);
	if(fisica == 0) {
		invalid_syscall();
		return;
	}

#ifdef DEBUG
	print_hex((unsigned int)fisica, 8, 10, 1, C_FG_WHITE);
	print_hex(p, 8, 10, 0, C_FG_WHITE);
#endif

	//le sumo le offset del puntero de memoria virtual
	fisica += (p & 0x0fff);

	//devuelvo coordenadas
	short* fisicaPos = (short*)fisica;
	fisicaPos[0] = x;
	fisicaPos[1] = y;

}



void game_mapear(int x, int y) {
	if(x < 0 || x >= SIZE_W || y < 0 || y >= SIZE_H) {
		invalid_syscall();
		return;
	}


	unsigned short tssIdx = obtenerIdxTssTareaActual();
	unsigned short soyx;
	unsigned short soyy;
	obtenerPosicionTarea(tssIdx, &soyx, &soyy);
	if(soyx == POS_INVALIDA || soyy == POS_INVALIDA) {
		invalid_syscall();
		return;
	}

	//breakpoint();


	unsigned int cr3 = rcr3();

	//desmapeamos la pagina anterior si la hubiese
	unsigned short pagAnteriorX;
	unsigned short pagAnteriorY;
	obtenerPosPaginaAnterior(tssIdx, &pagAnteriorX, &pagAnteriorY);
	if(pagAnteriorX != POS_INVALIDA && pagAnteriorY != POS_INVALIDA) {
		//banana
		mapa[pagAnteriorY][pagAnteriorX].dirty = 1;
		mmu_unmapear_pagina(INICIO_VIRTUAL_MAPA + PAGE_SIZE, cr3);
		int offset = ((pagAnteriorY * SIZE_W) + pagAnteriorX) * 0x1000;	
		addr fisica = INICIO_FISICO_MAPA + offset;
		mmu_unmapear_pagina(fisica, cr3);
	}


	//mapeamos la pagina pedida
	int offset = ((y * SIZE_W) + x) * 0x1000;	
	addr fisica = INICIO_FISICO_MAPA + offset;
	mmu_mapear_pagina(INICIO_VIRTUAL_MAPA + PAGE_SIZE, cr3, fisica, ENTRY_USER);
	mmu_mapear_pagina(fisica, cr3, fisica, ENTRY_ROOT);


	//unsigned char infectado_por = mapa[soyy][soyx].infectado_por;
	//unsigned char tarea_de = mapa[soyy][soyx].tarea_de;
	//unsigned char jugador = infectado_por == POS_INVALIDA ? tarea_de : infectado_por;

	tareaMapeaPagina(tssIdx, x, y);

	mapa[y][x].pagina_de = getJugadoresPagina(x, y);
	if(pagAnteriorX != POS_INVALIDA && pagAnteriorY != POS_INVALIDA) {
		mapa[pagAnteriorY][pagAnteriorX].pagina_de = getJugadoresPagina(pagAnteriorX, pagAnteriorY);
	}
	mapa[y][x].dirty = 1;
	pantalla();

#ifdef DEBUG
	print_int(x, 20, 1, C_FG_WHITE);
	print_int(y, 25, 1, C_FG_WHITE);
#endif

}


void my_syscall(unsigned int eax, unsigned int ebx, unsigned int ecx) {
	switch(eax) {
		case DONDE:
			game_donde((short*)ebx);
		break;

		case SOY:
			game_soy(ebx);
		break;


		case MAPEAR:
			game_mapear(ebx, ecx);
		break;

		default:
			invalid_syscall();
		break;
	}
	saltasteAIdle();
}

void keyPress(unsigned char keycode) {

	direccion dir;
	unsigned int jugador = 0;
	unsigned int lanzar = 0;
	unsigned int invalida = 0;
	switch(keycode) {
		case 0x1f:
			dir = IZQ;
			jugador = 0;
			break;
		case 0x20:
			dir = ABA;
			jugador = 0;
			break;
		case 0x21:
			dir = DER;
			jugador = 0;
			break;
		case 0x12:
			dir = ARB;
			jugador = 0;
			break;
		case 0x2a:
			lanzar = 1;
			jugador = 0;
			break;


		case 0x24:
			dir = IZQ;
			jugador = 1;
			break;
		case 0x25:
			dir = ABA;
			jugador = 1;
			break;
		case 0x26:
			dir = DER;
			jugador = 1;
			break;
		case 0x17:
			dir = ARB;
			jugador = 1;
			break;
		case 0x36:
			lanzar = 1;
			jugador = 1;
			break;

		default:
			invalida = 1;
			break;
	}


	if(invalida) return;

	if(lanzar) {
		game_lanzar(jugador);
	} else {
		game_mover_cursor(jugador, dir);
	}

	pantalla();
#ifdef DEBUG
	print_hex(keycode, 2, 0, 0, C_FG_WHITE);
#endif

}