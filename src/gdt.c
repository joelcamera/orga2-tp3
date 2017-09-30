/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         	   /* limit[0:15]  */
        (unsigned short)    0x0000,         	   /* base[0:15]   */
        (unsigned char)     0x00,           	   /* base[23:16]  */
        (unsigned char)     0x00,           	   /* type         */
        (unsigned char)     0x00,           	   /* s            */
        (unsigned char)     0x00,           	   /* dpl          */
        (unsigned char)     0x00,           	   /* p            */
        (unsigned char)     0x00,           	   /* limit[16:19] */
        (unsigned char)     0x00,           	   /* avl          */
        (unsigned char)     0x00,           	   /* l            */
        (unsigned char)     0x00,           	   /* db           */
        (unsigned char)     0x00,           	   /* g            */
        (unsigned char)     0x00,           	   /* base[31:24]  */
    },
  
    //0x20
    [4] = (gdt_entry) {
        (unsigned short)    GDT_LIMIT_LOW,         /* limit[0:15]  */
        (unsigned short)    0x0000,         	   /* base[0:15]   */
        (unsigned char)     0x00,           	   /* base[23:16]  */
        (unsigned char)     GDT_TYPE_CODE_EXECUTE, /* type         */
        (unsigned char)     GDT_NO_SYSTEM,         /* s            */
        (unsigned char)     GDT_DPL_ROOT,          /* dpl          */
        (unsigned char)     GDT_SEGMENT_PRESENT,   /* p            */
        (unsigned char)     GDT_LIMIT_HIGH,        /* limit[16:19] */
        (unsigned char)     0x00,           	   /* avl          */
        (unsigned char)     GDT_NO_64,      	   /* l            */
        (unsigned char)     GDT_32_BIT_SEGMENT,    /* db           */
        (unsigned char)     GDT_GRANULARITY_4K,    /* g            */ //banana 1 es 4k ?
        (unsigned char)     0x00,          		   /* base[31:24]  */
    },
    
    //0x28
    [5] = (gdt_entry) {
        (unsigned short)    GDT_LIMIT_LOW,         /* limit[0:15]  */
        (unsigned short)    0x0000,        		   /* base[0:15]   */
        (unsigned char)     0x00,          		   /* base[23:16]  */
        (unsigned char)     GDT_TYPE_DATA_RW,      /* type         */
        (unsigned char)     GDT_NO_SYSTEM,         /* s            */
        (unsigned char)     GDT_DPL_ROOT,          /* dpl          */
        (unsigned char)     GDT_SEGMENT_PRESENT,   /* p            */
        (unsigned char)     GDT_LIMIT_HIGH,        /* limit[16:19] */
        (unsigned char)     0x00,          	       /* avl          */
        (unsigned char)     GDT_NO_64,             /* l            */
        (unsigned char)     GDT_32_BIT_SEGMENT,    /* db           */
        (unsigned char)     GDT_GRANULARITY_4K,    /* g            */ //banana 1 es 4k ?
        (unsigned char)     0x00,        	       /* base[31:24]  */
    },
    

    //0x30
    [6] = (gdt_entry) {
        (unsigned short)    GDT_LIMIT_LOW,         /* limit[0:15]  */
        (unsigned short)    0x0000,         	   /* base[0:15]   */
        (unsigned char)     0x00,           	   /* base[23:16]  */
        (unsigned char)     GDT_TYPE_CODE_EXECUTE, /* type         */
        (unsigned char)     GDT_NO_SYSTEM,         /* s            */
        (unsigned char)     GDT_DPL_USER,          /* dpl          */
        (unsigned char)     GDT_SEGMENT_PRESENT,   /* p            */
        (unsigned char)     GDT_LIMIT_HIGH,        /* limit[16:19] */
        (unsigned char)     0x00,          		   /* avl          */
        (unsigned char)     GDT_NO_64,             /* l            */
        (unsigned char)     GDT_32_BIT_SEGMENT,    /* db           */
        (unsigned char)     GDT_GRANULARITY_4K,    /* g            */ //banana 1 es 4k ?
        (unsigned char)     0x00,         		   /* base[31:24]  */
    },
    

    //0x38
    [7] = (gdt_entry) {
        (unsigned short)    GDT_LIMIT_LOW,          /* limit[0:15]  */
        (unsigned short)    0x0000,                 /* base[0:15]   */
        (unsigned char)     0x00,                   /* base[23:16]  */
        (unsigned char)     GDT_TYPE_DATA_RW,       /* type         */
        (unsigned char)     GDT_NO_SYSTEM,          /* s            */
        (unsigned char)     GDT_DPL_USER,           /* dpl          */
        (unsigned char)     GDT_SEGMENT_PRESENT,    /* p            */
        (unsigned char)     GDT_LIMIT_HIGH,         /* limit[16:19] */
        (unsigned char)     0x00,          	        /* avl          */
        (unsigned char)     GDT_NO_64,      	    /* l            */
        (unsigned char)     GDT_32_BIT_SEGMENT,     /* db           */
        (unsigned char)     GDT_GRANULARITY_4K,     /* g            */ //banana 1 es 4k ?
        (unsigned char)     0x00,        		    /* base[31:24]  */
    },
    
    //0x40
    [8] = (gdt_entry) {
        (unsigned short)    GDT_VIDEO_LIMIT_LOW,     /* limit[0:15]  */
        (unsigned short)    GDT_VIDEO_BASE_LOW,      /* base[0:15]   */
        (unsigned char)     GDT_VIDEO_BASE_HIGH,     /* base[23:16]  */
        (unsigned char)     GDT_TYPE_DATA_RW,     	 /* type         */
        (unsigned char)     GDT_NO_SYSTEM,        	 /* s            */
        (unsigned char)     GDT_DPL_ROOT,         	 /* dpl          */
        (unsigned char)     GDT_SEGMENT_PRESENT,     /* p            */
        (unsigned char)     GDT_VIDEO_LIMIT_HIGH,    /* limit[16:19] */
        (unsigned char)     0x00,          		     /* avl          */
        (unsigned char)     GDT_NO_64,      		 /* l            */
        (unsigned char)     GDT_32_BIT_SEGMENT,      /* db           */
        (unsigned char)     GDT_GRANULARITY_4K,      /* g            */ //banana 1 es 4k ?
        (unsigned char)     0x00,       		     /* base[31:24]  */
    },
    

};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};


unsigned int proxima_tss_entry = INDICE_PRIMER_TSS_EN_GDT;
//0x48

void inicializar_gdt_tss() {
    unsigned int i;
    for(i = 0; i<27; i++) {
        crearTssEntry();
    }

}

void crearTssEntry() {

    unsigned int addrTss = (unsigned int)&tss_entries[proxima_tss_entry-INDICE_PRIMER_TSS_EN_GDT];
    unsigned int addrHigh = addrTss >> 16;
    unsigned int addrLow  = addrTss & 0xffff;

    gdt[proxima_tss_entry] = (gdt_entry) {
        (unsigned short)    103,                        /* limit[0:15]  */
        (unsigned short)    addrLow,                    /* base[0:15]   */
        (unsigned char)     addrHigh,                   /* base[23:16]  */
        (unsigned char)     GDT_TYPE_TSS,               /* type         */
        (unsigned char)     GDT_SYSTEM,                 /* s            */
        (unsigned char)     GDT_DPL_ROOT,               /* dpl          */
        (unsigned char)     GDT_SEGMENT_PRESENT,        /* p            */
        (unsigned char)     0,                          /* limit[16:19] */
        (unsigned char)     0x00,                       /* avl          */
        (unsigned char)     GDT_NO_64,                  /* l            */
        (unsigned char)     GDT_32_BIT_SEGMENT,         /* db           */
        (unsigned char)     GDT_GRANULARITY_1B,         /* g            */ //banana 1 es 4k ?
        (unsigned char)     0x00,                       /* base[31:24]  */
    };

    proxima_tss_entry++;
}

