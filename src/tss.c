/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "defines.h"

tss tss_inicial;
tss tss_idle;

tss tss_entries[27] = {};


void tss_inicializar() {
    tss_inicializar_idle();
}

void tss_inicializar_tarea(unsigned int tssIdx, addr cr3) {
    addr spKernel = mmu_proxima_pagina_fisica_libre() + PAGE_SIZE;
    inicializar_tss(tssIdx,  cr3, INICIO_VIRTUAL_MAPA, INICIO_VIRTUAL_MAPA+PAGE_SIZE, spKernel, SEGMENTO_CODE_USER | 3, SEGMENTO_DATA_USER | 3, SEGMENTO_DATA_KERNEL);
}


void tss_inicializar_idle() {
    addr sp = mmu_proxima_pagina_fisica_libre() + PAGE_SIZE;
    addr spKernel = sp;
    inicializar_tss(TAREA_IDLE, 0x27000, 0x10000, sp, spKernel, SEGMENTO_CODE_KERNEL, SEGMENTO_DATA_KERNEL, SEGMENTO_DATA_KERNEL);
}


void inicializar_tss(unsigned int tssIdx, addr cr3, addr eip, addr sp, addr spKernel,
		unsigned short  sCodigo, unsigned short  sDatos,  unsigned short  sStackKernel)
{
	unsigned short sStackUser = sDatos;

    tss_entries[tssIdx].ptl = 0;
    tss_entries[tssIdx].unused0 = 0;
    tss_entries[tssIdx].esp0 = spKernel;
    tss_entries[tssIdx].ss0 = sStackKernel;
    tss_entries[tssIdx].unused1 = 0;
    tss_entries[tssIdx].esp1 = 0;
    tss_entries[tssIdx].ss1 = 0;
    tss_entries[tssIdx].unused2 = 0;
    tss_entries[tssIdx].esp2 = 0;
    tss_entries[tssIdx].ss2 = 0;
    tss_entries[tssIdx].unused3 = 0;
    tss_entries[tssIdx].cr3 = cr3;
    tss_entries[tssIdx].eip = eip;
    tss_entries[tssIdx].eflags = 0x202;
    tss_entries[tssIdx].eax = 0;
    tss_entries[tssIdx].ecx = 0;
    tss_entries[tssIdx].edx = 0;
    tss_entries[tssIdx].ebx = 0;
    tss_entries[tssIdx].esp = sp;
    tss_entries[tssIdx].ebp = sp;
    tss_entries[tssIdx].esi = 0;
    tss_entries[tssIdx].edi = 0;
    tss_entries[tssIdx].es = sDatos;
    tss_entries[tssIdx].unused4 = 0;
    tss_entries[tssIdx].cs = sCodigo;
    tss_entries[tssIdx].unused5 = 0;
    tss_entries[tssIdx].ss = sStackUser;
    tss_entries[tssIdx].unused6 = 0;
    tss_entries[tssIdx].ds = sDatos;
    tss_entries[tssIdx].unused7 = 0;
    tss_entries[tssIdx].fs = sDatos;
    tss_entries[tssIdx].unused8 = 0;
    tss_entries[tssIdx].gs = sDatos;
    tss_entries[tssIdx].unused9 = 0;
    tss_entries[tssIdx].ldt = 0;
    tss_entries[tssIdx].unused10 = 0;
    tss_entries[tssIdx].dtrap = 0;
    tss_entries[tssIdx].iomap = 0;

}

