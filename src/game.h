/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"
#include "tss.h"

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ARB = 0xA33, ABA = 0x883  } direccion;


#define DONDE  0x124
#define SOY    0xA6A
#define MAPEAR 0xFF3

#define VIRUS_ROJO 0x841
#define VIRUS_AZUL 0x325


void my_syscall(unsigned int eax, unsigned int ebx, unsigned int ecx);

void game_lanzar(unsigned int jugador);
void game_lanzar_aux(unsigned int jugador, unsigned int x, unsigned int y);

void game_soy(unsigned int soy);

void game_donde(short* pos);

void game_mover_cursor(int jugador, direccion dir);

void keyPress(unsigned char keycode);

void inicializar_juego();
unsigned short obtenerIdxTssTareaActual();
void obtenerPosicionTarea(unsigned short tssIdx, unsigned short* x, unsigned short* y);


#endif  /* !__GAME_H__ */
