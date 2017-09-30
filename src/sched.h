/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "gdt.h"


typedef struct _TAREA_INFO {
	unsigned char tick;
	unsigned char viva;
	unsigned short pagina_x;
	unsigned short pagina_y;
	unsigned char estado;    // health = 0, infectada_a = 1, infectada_b = 2
	unsigned int tssIdx;   	// este indice apunta al indice en tss_entries
						   	// como la primer entrada de tarea en la GDT esta en el indice 9
							// sumando 9 a esta valor tenemos el indice en la GDT
} TAREA_INFO;

extern TAREA_INFO tareas_health[15];
extern TAREA_INFO tareas_a[5];
extern TAREA_INFO tareas_b[5];


unsigned short sched_proximo_indice();

void saltasteAIdle();
void inicializar_scheduler();
unsigned int lanzarTarea(unsigned int jugador);
void matarTarea(unsigned int jugador, unsigned int tssIdx, unsigned short* pagina_x, unsigned short* pagina_y);

void obtenerPosPaginaAnterior(unsigned int tssIdx, unsigned short* x, unsigned short* y);
void tareaMapeaPagina(unsigned int tssIdx, unsigned short x, unsigned short y);
unsigned char getJugadoresPagina(unsigned short x, unsigned short y);



#endif	/* !__SCHED_H__ */
