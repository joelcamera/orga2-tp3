/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"
//#include "paginacion.h"


typedef unsigned int addr;
extern void flushtlb();

#define PD_COUNT 1024
#define PT_COUNT 1024

#define KERNEL_PAGE_DIRECTORY 0x27000
#define KERNEL_PAGE_TABLE 0x28000

#define ENTRY_USER 1
#define ENTRY_ROOT 0
#define ENTRY_PRESENT 1
#define ENTRY_WRITE 1


#define INICIO_PAGINA_LIBRE 0x100000
#define PAGE_SIZE 0x1000
#define INICIO_VIRTUAL_MAPA 0x08000000
#define INICIO_FISICO_MAPA 0x400000 
#define CODIGO_PRIMERA_TAREA 0x10000

/* Struct de una entrada de la Page Directory */
typedef struct page_entry_ {
    unsigned char present:1;				//1 (1=present)
    unsigned char read_write:1;				//1 (1=write)
    unsigned char user_super:1;				//0 (0=super)
    unsigned char page_level_write_tr:1;		//0
    unsigned char page_level_cache_dis:1;		//0
    unsigned char accessed:1;				//0
    unsigned char zero_dirty:1;				//0
    unsigned char page_size_o_attributes:1; 		//0 (0=4k)
    unsigned char global:1;				//0
    unsigned char disponible:3;				//000
    addr dir_base_pagina:20;
    
} __attribute__((__packed__, aligned (4))) page_entry;


addr mmu_inicalizar_dir_tarea(int tarea_id, int offset);
void mmu_inicializar_dir_kernel();
void inicializar_page_entry(addr pagedir, int index, addr dir_base, int user, int rw, int present);
void mmu_inicializar();
void test_tareas();
addr mmu_proxima_pagina_fisica_libre();
void mmu_mapear_pagina(addr virtual, unsigned int cr3, addr fisica, unsigned short DPL);
void mmu_unmapear_pagina(addr virtual, unsigned int cr3);

#endif	/* !__MMU_H__ */




