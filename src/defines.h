/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_H                 15
#define CANT                    5
#define SIZE_W                  80
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
//banana
#define GDT_COUNT 50

#define GDT_IDX_NULL_DESC           0

//banana dpl correcto ?
#define GDT_DPL_ROOT                0
#define GDT_DPL_USER                3    
#define GDT_TYPE_CODE_EXECUTE       8
#define GDT_TYPE_TSS				9
#define GDT_TYPE_DATA_RW            2    
#define GDT_NO_SYSTEM               1
#define GDT_SYSTEM                  0
#define GDT_SEGMENT_PRESENT         1 
#define GDT_32_BIT_SEGMENT          1
#define GDT_GRANULARITY_4K          1
#define GDT_GRANULARITY_1B			0
#define GDT_NO_64                   0

/*
	878MB / 1024 = 899072KB
	899072KB / 4KB(granularidad) = 224768 tamaño del segmento
	224768 = 0x36E00 tamaño del segmento
	Limite = tam del segm -1 (arranca de cero)
	Limite = 0x36E00 - 0x1 = 0x36DFF
*/
//esto son 878mb - 1b (en granularidad 4k)
#define GDT_LIMIT_HIGH 0x03
#define GDT_LIMIT_LOW  0x6dff
//esto son 8k - 1 byte (en granularidad 4k)       
#define GDT_VIDEO_LIMIT_LOW 0x1     
#define GDT_VIDEO_LIMIT_HIGH 0x0     
#define GDT_VIDEO_BASE_HIGH 0xb
#define GDT_VIDEO_BASE_LOW 0x8000

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO_SCREEN            0x000B8000 /* direccion fisica del buffer de video */


//Defines usados en MMU y scheduler
#define TAREA_INICIAL 0
#define TAREA_IDLE 1
#define TAREA_HEALT_BASE 2
#define TAREA_A_BASE 17
#define TAREA_B_BASE 22

#define SEGMENTO_CODE_KERNEL 0x20
#define SEGMENTO_DATA_KERNEL 0x28
#define SEGMENTO_CODE_USER   0x30
#define SEGMENTO_DATA_USER   0x38

#define INDICE_PRIMER_TSS_EN_GDT 9


//Defines del juego
#define VIDAS_INICIAL 20        // cantidad de tareas restantes
#define MAX_X 79
#define MAX_Y 43

#define POS_INVALIDA 99
    


#endif  /* !__DEFINES_H__ */
