/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;


typedef struct _MAPA {
	unsigned short tssIdx;
	unsigned char dirty;
	unsigned char infectado_por;// 0 jugador a, 1 jugador b,               99 nadie
	unsigned char tarea_de;		// 0 jugador a, 1 jugador b, 2 tarea sana, 99 nadie		
	unsigned char pagina_de;    // 0 jugador a, 1 jugador b, 2 mapeado por los dos  99 nadie
}  MAPA;


typedef struct _JUGADOR_INFO {
	unsigned int x;
	unsigned int y;
	unsigned int vidas;
	unsigned int puntos;
	unsigned int tareasActivas;
} JUGADOR_INFO;

extern JUGADOR_INFO jugador_a;
extern JUGADOR_INFO jugador_b;
extern MAPA mapa[44][80];


void pantalla_inicial();

void pantalla();

void pantalla_debug(unsigned short idx, unsigned short ss, unsigned short gs, unsigned short fs, unsigned short es, unsigned short ds, unsigned short cs,
unsigned int edi, unsigned int esi, unsigned int ebp, unsigned int esp, unsigned int ebx, unsigned int edx, unsigned int ecx, unsigned int eax,
unsigned int eipIsr, unsigned int eipTask, unsigned short cs2, unsigned int eflags);

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);


#endif  /* !__SCREEN_H__ */
