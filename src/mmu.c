/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


unsigned int proxima_pagina_libre;


void inicializar_page_entry(addr pagedir, int index, addr dir_base, int user, int rw, int present) {
	page_entry* pde = (page_entry*) (pagedir + (index * 4));
	*(unsigned int*)pde = 0;

	pde->dir_base_pagina = (dir_base) / (1024 * 4);
	pde->read_write = rw;
	pde->present = present;
	pde->user_super = user;
}


#define PAGINAS_IDENTITY 1
void inicializar_identity_mapping(addr page_directory, addr primera_page_table ) {
	unsigned int i = 0;
	unsigned int j = 0;
	for(i = 0; i < PAGINAS_IDENTITY; i++) {
		inicializar_page_entry(page_directory, i, primera_page_table+(i*0x1000), ENTRY_ROOT, ENTRY_WRITE, ENTRY_PRESENT);
	}
	for(i = PAGINAS_IDENTITY; i < PD_COUNT; i++) {
		inicializar_page_entry(page_directory, i, 0, 0, 0, 0);
	}

	for(j = 0; j < PAGINAS_IDENTITY; j++) {
		for(i = 0; i < PT_COUNT; i++) {
			inicializar_page_entry(primera_page_table+(j*0x1000), i, i*0x1000+j*0x400000, ENTRY_ROOT, ENTRY_WRITE, ENTRY_PRESENT);
		}
	}

}

void mmu_inicializar_dir_kernel() {

	inicializar_identity_mapping(KERNEL_PAGE_DIRECTORY, KERNEL_PAGE_TABLE);
	flushtlb();
}


void mmu_inicializar() {
	proxima_pagina_libre = INICIO_PAGINA_LIBRE;
}

addr mmu_proxima_pagina_fisica_libre() {
	addr tmp = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return tmp;
}

addr mmu_inicalizar_dir_tarea(int tarea_id, int offset) {
	//reservamos 2 paginas del area libre para page directories de tareas
	addr tarea_page_dir   = mmu_proxima_pagina_fisica_libre();

	addr identity_mapping_page_table = mmu_proxima_pagina_fisica_libre();
	unsigned int i;
	for(i = 1; i<PAGINAS_IDENTITY; i++) {
		mmu_proxima_pagina_fisica_libre();
	}

	inicializar_identity_mapping(tarea_page_dir, identity_mapping_page_table);

	//mapeamos temporalmente en la la posicion en el pagedir de la tarea actual
	addr cr3 = rcr3();
	mmu_mapear_pagina(INICIO_FISICO_MAPA + offset, cr3, INICIO_FISICO_MAPA + offset, ENTRY_ROOT);

	//mapeamos para la tarea actual y para 
	mmu_mapear_pagina(INICIO_FISICO_MAPA + offset, tarea_page_dir, INICIO_FISICO_MAPA + offset, ENTRY_ROOT);
	mmu_mapear_pagina(INICIO_VIRTUAL_MAPA, tarea_page_dir, INICIO_FISICO_MAPA + offset, ENTRY_USER);

	flushtlb();

	//breakpoint();
	//copiamos tarea a posicion en el mapa
	char* posTarea = (char*)(CODIGO_PRIMERA_TAREA + (tarea_id * PAGE_SIZE));
	char* destTarea = (char*)(INICIO_FISICO_MAPA + offset);
	for(i = 0; i < PAGE_SIZE; i++) {
		destTarea[i] = posTarea[i];
	}
	//breakpoint();

	mmu_unmapear_pagina(INICIO_FISICO_MAPA + offset, cr3);


	flushtlb();

	return tarea_page_dir;
}

void mmu_mapear_pagina(addr virtual, unsigned int cr3, addr fisica, unsigned short DPL) {
	//la entrada del directorio de paginas que necesitamos esta en los bits virtual[31:22]
	unsigned int dir_offset = virtual >> 22;
	unsigned int i;
	page_entry* page_directory_entry = (page_entry*) (cr3+dir_offset*4);

	if(!page_directory_entry->present) {
		addr tabla = mmu_proxima_pagina_fisica_libre();
		inicializar_page_entry(cr3, dir_offset, tabla, DPL, ENTRY_WRITE, ENTRY_PRESENT);
		for(i = 0; i < PT_COUNT; i++) {
			inicializar_page_entry(tabla, i, 0, 0, 0, 0);
		}

	}

	addr tabla_paginas = page_directory_entry->dir_base_pagina * (1024 * 4);

	//la pagina de la tabla de paginas esta en los bits virtual[21:12]
	unsigned int pagina_offset = virtual >> 12; //movemos los 10 bits hacia la derecha
	pagina_offset = pagina_offset & 0x000003FF; //limpiamos toda la parte alta, o sea los bits [31:11]

	inicializar_page_entry(tabla_paginas, pagina_offset, fisica, DPL, ENTRY_WRITE, ENTRY_PRESENT);

	flushtlb();
}

void mmu_unmapear_pagina(addr virtual, unsigned int cr3) {
	unsigned int dir_offset = virtual >> 22;
	unsigned int pagina_offset = virtual >> 12; //movemos los 10 bits hacia la derecha
	pagina_offset = pagina_offset & 0x000003FF; //limpiamos toda la parte alta, o sea los bits [31:11]
	page_entry* page_directory_entry = (page_entry*) (cr3+dir_offset*4);

	if(page_directory_entry->present) {
		addr tabla_paginas = page_directory_entry->dir_base_pagina * (1024 * 4);
		inicializar_page_entry(tabla_paginas, pagina_offset, 0, 0, 0, 0);
		//recorrer y si estan todos en 0 borrar tabla
	}

	flushtlb();
}

void test_tareas() {
	mmu_inicalizar_dir_tarea(1, 0);
	//mmu_mapear_pagina(0x05000000, INICIO_PAGINA_LIBRE, 0x410000);
	//mmu_unmapear_pagina(0x05000000, INICIO_PAGINA_LIBRE);
}
