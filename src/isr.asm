; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

;0x00 = debug false ;0x01 = debug true ;0x02 = debug mostrando excepcion
modo_debug: db 0x00

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00


mensaje_exception db 'Ocurrio una excepcion nro: 00'
mensaje_exception_len equ $ - mensaje_exception


;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice

;; Game
extern keyPress
extern my_syscall
extern game_matar_tarea
extern obtenerIdxTssTareaActual

;; Screen
extern pantalla_debug

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:


    cmp byte [modo_debug], 0x01 ;si el debug esta encendido, ponemos el estado del debug en 'mostrar excepcion' con informacion de la tarea
    jne .resumir%1
    inc byte [modo_debug]
    call mostrarExcepcion

.resumir%1:

    mov eax, %1

    cmp eax, 10
    jl .esMenor%1
    mov byte [mensaje_exception + mensaje_exception_len - 2], 0x31
    sub eax, 10

.esMenor%1:
    add eax, 0x30
    mov [mensaje_exception + mensaje_exception_len - 1], al

    call game_matar_tarea

    imprimir_texto_mp mensaje_exception, mensaje_exception_len, 0x04, 0, 0


    jmp 0x50:0

    iret

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19



;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
pushad

;si estamos en modo debug mostrando la excepcion, ignoramos la interrupcion del reloj
cmp byte [modo_debug], 0x02
je .nojump

call proximo_reloj

call sched_proximo_indice
cmp ax, 0
je .nojump
    shl ax, 3
    mov [sched_tarea_selector], ax
    call fin_intr_pic1
    ;xchg bx, bx
    jmp far [sched_tarea_offset]
    jmp .end

.nojump:
call fin_intr_pic1

.end:
popad
iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
_isr33:
pushad

;mov ebx, mapa
;add bl, al
;imprimir_texto_mp eax, 1, 0x0f, 0, 0

xor eax, eax
in al, 0x60

;nos fijamos si se presiono 'y'
cmp al, 0x15
jne .continuar ;sino continuamos
;si se presiono y el juego esta detenido mostrando la pantalla de debug, pasamos de modo 'debug excepcion' -> 'debug true'
cmp byte [modo_debug], 0x02
jne .swapDebug ;sino alternamos entre prendido y apagado
mov byte [modo_debug], 0x00
.swapDebug:
xor byte [modo_debug], 0x01
;nos fijamos si ya estamos en modo debug mostrando la excepcion pero no se presiono la tecla 'y' ignoramos la interrupcion de teclado
.continuar:
cmp byte [modo_debug], 0x02
je .fin

push eax
call keyPress
pop eax

.fin:
call fin_intr_pic1
popad
iret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr102
_isr102:
push ecx
push ebx
push eax
;xchg bx,bx
call my_syscall
pop eax
pop ebx
pop ecx

; siempre despues de una syscall salto a idle
jmp 0x50:0

iret




%define DONDE  0x124
%define SOY    0xA6A
%define MAPEAR 0xFF3

%define VIRUS_ROJO 0x841
%define VIRUS_AZUL 0x325


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
        
  
mostrarExcepcion:
    pushad
    push cs
    push ds
    push es
    push fs
    push gs
    push ss

	xor eax, eax
	call obtenerIdxTssTareaActual
	push eax
    call pantalla_debug
	pop eax

    pop ss
    pop gs
    pop fs
    pop es
    pop ds
    pop cs
    popad
ret      
