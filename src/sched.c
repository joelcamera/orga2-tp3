/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "defines.h"
#include "screen.h"


unsigned char proximaViva(TAREA_INFO* lista, unsigned int cantidad, unsigned int* actual, unsigned int* tssIdx);
void inicializarListaTarea(TAREA_INFO* lista, unsigned int cantidad, unsigned int baseTssIdx);


unsigned int actual_health;
unsigned int actual_a;
unsigned int actual_b;
unsigned int actual_tipo;
unsigned int tssActual;

TAREA_INFO tareas_health[15];
TAREA_INFO tareas_a[5];
TAREA_INFO tareas_b[5];


void inicializar_scheduler() {
	inicializarListaTarea(tareas_health, 15, TAREA_HEALT_BASE);
	inicializarListaTarea(tareas_a, 5, TAREA_A_BASE);
	inicializarListaTarea(tareas_b, 5, TAREA_B_BASE);
	actual_tipo = 0;
	actual_health = 0;
	actual_a = 0;
	actual_b = 0;
	tssActual = 0;
}


unsigned short proximaLibre(unsigned int jugador) {
	TAREA_INFO* t = jugador == 0 ? tareas_a : (jugador == 1 ? tareas_b : tareas_health);
	int size = jugador == 2 ? 15 : 5;
	unsigned short i = 0;
	for(i = 0; i < size; i++) {
		if(t[i].viva == 0) {
			return i;
		}
	}
	//suponemos que no deberia llegar aca
	return POS_INVALIDA;
}

unsigned int lanzarTarea(unsigned int jugador) {
	TAREA_INFO* t = jugador == 0 ? tareas_a : (jugador == 1 ? tareas_b : tareas_health);
	unsigned short i = proximaLibre(jugador);
	if(i == POS_INVALIDA) return i;

 	t[i].viva = 1;
 	return t[i].tssIdx;
}


void matarTarea(unsigned int jugador, unsigned int tssIdx, unsigned short* pagina_x, unsigned short* pagina_y) {

	TAREA_INFO* t = jugador == 0 ? tareas_a : (jugador == 1 ? tareas_b : tareas_health);
	int size = jugador == 2 ? 15 : 5;
	unsigned short i = 0;
	for(i = 0; i < size; i++) {
		if(t[i].tssIdx == tssIdx) {
			t[i].viva = 0;
			*pagina_x = t[i].pagina_x;
			*pagina_y = t[i].pagina_y;
			t[i].pagina_x = POS_INVALIDA;
			t[i].pagina_y = POS_INVALIDA;
		}
	}
	

}


void obtenerPosPaginaAnterior(unsigned int tssIdx, unsigned short* x, unsigned short* y) {
	unsigned int jugador = 0;
	//recorro los 3 vectores y para buscar el tssIdx que coinciden
	for(jugador = 0; jugador < 3; jugador++) {

		TAREA_INFO* t = jugador == 0 ? tareas_a : (jugador == 1 ? tareas_b : tareas_health);
		int size = jugador == 2 ? 15 : 5;
		unsigned short i = 0;
		for(i = 0; i < size; i++) {
			if(t[i].tssIdx == tssIdx) {
				*x = t[i].pagina_x;
				*y = t[i].pagina_y;
				return;	
			}
		}

	}
	*x = POS_INVALIDA;
	*y = POS_INVALIDA;
}

unsigned char getJugadoresPagina(unsigned short x, unsigned short y) {
	unsigned char res = POS_INVALIDA;
	unsigned int jugador = 0;
	//recorro los 3 vectores y para buscar el tssIdx que coinciden
	for(jugador = 0; jugador < 3; jugador++) {

		TAREA_INFO* t = jugador == 0 ? tareas_a : (jugador == 1 ? tareas_b : tareas_health);
		int size = jugador == 2 ? 15 : 5;
		unsigned short i = 0;
		for(i = 0; i < size; i++) {
			if(t[i].pagina_x == x && t[i].pagina_y == y) {
				if(res == POS_INVALIDA || res == jugador) res = jugador;
				else {
					res = 3;
					return res;
				}
			}
		}

	}
	return res;

}

void tareaMapeaPagina(unsigned int tssIdx, unsigned short x, unsigned short y) {

	unsigned int jugador = 0;
	//recorro los 3 vectores y para buscar el tssIdx que coinciden
	for(jugador = 0; jugador < 3; jugador++) {

		TAREA_INFO* t = jugador == 0 ? tareas_a : (jugador == 1 ? tareas_b : tareas_health);
		int size = jugador == 2 ? 15 : 5;
		unsigned short i = 0;
		for(i = 0; i < size; i++) {
			if(t[i].tssIdx == tssIdx) {
				t[i].pagina_x = x;
				t[i].pagina_y = y;
				return;	
			}
		}

	}



}


void saltasteAIdle() {
	tssActual = 0;
}


void reloj() {
    //posicon y valor
    char r0[] = "|";
    char r1[] = "/";
    char r2[] = "-";
    char r3[] = "\\";
    char* relojstr[] = {r0, r1,r2, r3};

    unsigned int i;
    unsigned int x;
    unsigned int y;
    for(i = 0; i<15; i++) {
    	x = 3 + (i * 2);
    	y = 48;
        if(tareas_health[i].viva == 0) {
            print("*",x, y, C_FG_WHITE);
        } else {
            print(relojstr[tareas_health[i].tick], x, y, C_FG_WHITE);
        }
    }

    print("<A", 13, 46, C_FG_WHITE);
    for(i = 0; i<5; i++) {
    	x = 3 + (i * 2);
    	y = 46;
        if(tareas_a[i].viva == 0) {
            print("*", x, y, C_FG_WHITE);
        } else {
            print(relojstr[tareas_a[i].tick], x, y, C_FG_WHITE);
        }
    }

    print("B>", 20, 46, C_FG_WHITE);
    for(i = 0; i<5; i++) {
    	x = 23 + (i * 2);
    	y = 46;
        if(tareas_b[i].viva == 0) {
            print("*", x, y, C_FG_WHITE);
        } else {
            print(relojstr[tareas_b[i].tick], x, y, C_FG_WHITE);
        }
    }


}


unsigned short sched_proximo_indice() {
	unsigned int i = 0;

	unsigned int proxima = 0;
	unsigned char encontro = 0;

	for(i = 0; i<3 && encontro == 0; i++) {
		//print_int(i, 10, 10, C_FG_WHITE);

		actual_tipo = (actual_tipo + 1) % 3;

		if(actual_tipo == 0) {
			encontro = proximaViva(tareas_health, 15, &actual_health, &proxima);
		} else if(actual_tipo == 1) {
			encontro = proximaViva(tareas_a, 5, &actual_a, &proxima);
		} else {
			encontro = proximaViva(tareas_b, 5, &actual_b, &proxima);
		}

	}

	reloj();
	if(encontro == 1) {
		unsigned int nuevoIndice = proxima + INDICE_PRIMER_TSS_EN_GDT;
#ifdef DEBUG
		print_int(nuevoIndice, 30, 0, C_FG_WHITE);
#endif
		if(tssActual == nuevoIndice) {
			return 0;
		} else {
			tssActual = nuevoIndice;
			return  tssActual;
		}
	} else {
		return 0; //TAREA_IDLE + INDICE_PRIMER_TSS_EN_GDT;
	}

}


unsigned char proximaViva(TAREA_INFO* lista, unsigned int cantidad, unsigned int* actual, unsigned int* tssIdx) {
	unsigned int i = 0;
	for(i = 1; i <= cantidad; i++) {
		unsigned int j = (*actual + i) % cantidad;
		if(lista[j].viva) {
			*actual = j;
			*tssIdx = lista[j].tssIdx;
			lista[j].tick = (lista[j].tick+1) % 4;
			return 1;
		}
	}
	*actual = 0;
	return 0;
}


void inicializarListaTarea(TAREA_INFO* lista, unsigned int cantidad, unsigned int baseTssIdx) {
	unsigned int i = 0;
	for(i = 0; i < cantidad; i++) {
		lista[i].viva = 0;
		lista[i].estado = 0;
		lista[i].tssIdx = i + baseTssIdx;
		lista[i].pagina_x = POS_INVALIDA;
		lista[i].pagina_y = POS_INVALIDA;
	}
}

