/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "i386.h"
//#include "sched.h"

#define BASE_ABAJO 45

    
JUGADOR_INFO jugador_a = {0, 0, VIDAS_INICIAL, 0, 0};
JUGADOR_INFO jugador_b = {0, 0, VIDAS_INICIAL, 0, 0};

MAPA mapa[44][80] = {};


void fondo(unsigned char todo) {
    unsigned int x = 0;
    unsigned int y = 1;

    for(y = 1; y<BASE_ABAJO; y++) {
        for(x = 0; x<80; x++) {
            if(todo || mapa[y-1][x].dirty) {
                print("\xB0", x, y, C_FG_LIGHT_GREY);
            }
        }
    }
    
}

void vidas(int x, int vidas) {
    print("vidas", x, BASE_ABAJO + 1, C_FG_WHITE);
    if(vidas < 10) print(" ", x+1, BASE_ABAJO + 3, C_FG_WHITE);
    print_int(vidas, x+2, BASE_ABAJO + 3, C_FG_WHITE);
}

void puntos(int x, int ancho, int puntos, int color) {
    unsigned int y = 0;
    unsigned int i = 0;
    for(y = BASE_ABAJO; y<50; y++) {
        for(i = x ; i<x+ancho; i++) {
            print(" ", i, y, color + C_FG_WHITE);
        }
    }
    print_int(puntos, x+(ancho/2), BASE_ABAJO + 2, color + C_FG_WHITE);
}


void barraAbajo() {
    //reloj(); //pasar todos los relojes

    vidas(40, jugador_a.vidas);
    vidas(62, jugador_b.vidas);

    puntos(46, 6, jugador_a.puntos, C_BG_RED);
    puntos(54, 6, jugador_b.puntos, C_BG_BLUE);

}



void cursor() {
    if(jugador_a.x == jugador_b.x && jugador_a.y == jugador_b.y) {
        print("*", jugador_a.x, jugador_a.y+1, C_FG_WHITE + C_BG_BROWN);
    } else {
        print("\x01", jugador_a.x, jugador_a.y+1, C_FG_WHITE + C_BG_RED);
        print("\x01", jugador_b.x, jugador_b.y+1, C_FG_WHITE + C_BG_BLUE);
    }
}

void tarea() {
    unsigned int y = 0;
    unsigned int x = 0;
    for(x = 0; x<80; x++) {
        for(y = 0; y<44; y++) {
            if(mapa[y][x].dirty == 0) continue;
            mapa[y][x].dirty = 0;
            unsigned char inf = mapa[y][x].infectado_por;
            unsigned char tar = mapa[y][x].tarea_de;
            unsigned char pag = mapa[y][x].pagina_de;
            unsigned char j = inf;
            //Si no esta infectada, muestro el color de la tarea
            if(j == POS_INVALIDA) j = tar;
            //Si no tiene tarea, muestro el color del que mapeo
            //if(j == POS_INVALIDA) j = pag;
            //Si es invalida no hago nada
            if(j != POS_INVALIDA || pag != POS_INVALIDA) {
                //Muestro el color segun corresponda
                unsigned short color = j == 0 ? C_BG_RED : (j == 1 ? C_BG_BLUE : C_BG_GREEN);
                if(j==POS_INVALIDA) color = C_BG_LIGHT_GREY;
                //Muestro letra o espacio dependiendo si esta mapeado por alguien
                char a[] = "A";
                char b[] = "B";
                char h[] = "H";
                char xc[] = "X";
                char sp[] = " ";
                char* mostrar[] = {a,b,h,xc,sp};
                unsigned int show = pag;
                if(pag == POS_INVALIDA) show = 4;
                print(mostrar[show], x, y+1, color);
            }           
        }
    }

}

void pantalla() {
    fondo(0);
    tarea(); //varias
    cursor(); //varios
    barraAbajo();
}

void pantalla_inicial() {

    unsigned int x = 0;
    for(x = 0; x<80; x++) {
        print(" ", x, 0, C_BG_BLACK);
    }
    unsigned int y = 0;
    for(y = BASE_ABAJO; y<50; y++) {
        for(x = 0; x<80; x++) {
            print(" ", x, y, C_FG_BLACK);
        }
    }


    fondo(1);
    //barraAbajo();

}

void pantalla_debug(unsigned short idx, unsigned short ss, unsigned short gs, unsigned short fs, unsigned short es, unsigned short ds, unsigned short cs,
unsigned int edi, unsigned int esi, unsigned int ebp, unsigned int esp, unsigned int ebx, unsigned int edx, unsigned int ecx, unsigned int eax,
unsigned int eipIsr, unsigned int errCode, unsigned int eipTask, unsigned short cs2, unsigned int eflags) {
    unsigned int y = 0;
    unsigned int x = 0;
    unsigned int i = 0;

    for(y = 6; y<42; y++) {
        for(x = 24; x<56; x++) {
            mapa[y-1][x].dirty = 1;
            unsigned short color = C_BG_LIGHT_GREY;
            if(x == 24) color = C_BG_BLACK;
            if(x == 55) color = C_BG_BLACK;
            if(y == 6) color = C_BG_BLACK;
            if(y == 41) color = C_BG_BLACK;
            print(" ", x, y, color);
        }
    }

    char* virus;
    unsigned short color;
    if(idx >= 0x11 && idx <= 0x15) {
        color = C_BG_RED;
        virus = "virus A";
    } else if(idx >= 0x16 && idx <= 0x20) {
        color = C_BG_BLUE;
        virus = "virus B";
    } else {
        color = C_BG_GREEN;
        virus = "tarea H";
    }
    for(x = 25; x<55; x++) {
        print(" ", x, 7, color);
    }
    print(virus, 25, 7, C_FG_WHITE + color);

    y = 6; x = 24;

    print("eax", x+2, y+3, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(eax, 8, x+6, y+3, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("ebx", x+2, y+5, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(ebx, 8, x+6, y+5, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("ecx", x+2, y+7, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(ecx, 8, x+6, y+7, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("edx", x+2, y+9, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(edx, 8, x+6, y+9, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("esi", x+2, y+11, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(esi, 8, x+6, y+11, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("edi", x+2, y+13, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(edi, 8, x+6, y+13, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("ebp", x+2, y+15, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(ebp, 8, x+6, y+15, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("esp", x+2, y+17, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(esp, 8, x+6, y+17, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("eip", x+2, y+19, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(eipTask, 8, x+6, y+19, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("cs", x+3, y+21, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(cs, 4, x+6, y+21, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("ds", x+3, y+23, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(ds, 4, x+6, y+23, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("es", x+3, y+25, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(es, 4, x+6, y+25, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("fs", x+3, y+27, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(fs, 4, x+6, y+27, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("gs", x+3, y+29, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(gs, 4, x+6, y+29, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("ss", x+3, y+31, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(ss, 4, x+6, y+31, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("eflags", x+3, y+33, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(eflags, 8, x+9, y+33, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("cr0", x+16, y+3, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(rcr0(), 8, x+20, y+3, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("cr2", x+16, y+5, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(rcr2(), 8, x+20, y+5, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("cr3", x+16, y+7, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(rcr3(), 8, x+20, y+7, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("cr4", x+16, y+9, C_FG_BLACK + C_BG_LIGHT_GREY);
    print_hex(rcr4(), 8, x+20, y+9, C_FG_WHITE + C_BG_LIGHT_GREY);

    print("stack", x+16, y+21, C_FG_BLACK + C_BG_LIGHT_GREY);
    for(i = 0; i < 5; i++) {
        unsigned int* v = (unsigned int*) (esp+4*i);
        print_hex(*v, 8, x+16, y+23+i*2, C_FG_WHITE + C_BG_LIGHT_GREY);
    }
}


void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}





