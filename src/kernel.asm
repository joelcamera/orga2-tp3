; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start
extern GDT_DESC
extern idt_inicializar
extern IDT_DESC
extern mmu_inicializar_dir_kernel
extern pantalla_inicial
extern mmu_inicializar
extern test_tareas
extern resetear_pic
extern habilitar_pic
global flushtlb
extern keyPress
extern inicializar_gdt_tss
extern tss_inicializar
extern inicializar_scheduler
extern inicializar_juego

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

pantalla_limpia times 4000 db 176, 7
pantalla_limpia_len equ   $ - pantalla_limpia

nombre_grupo db         'Mas peligroso que mono con navaja'
nombre_grupo_len equ   $ - nombre_grupo

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    
    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax


    ; Saltar a modo protegido
    jmp 0x20:en_modo_protegido

    en_modo_protegido:
BITS 32

    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax, 0x28
    ; configuramos el selector de segmento de datos
    mov ds, ax
    ; configuramos el selector de segmento de pila (stack)
    mov ss, ax

    ; Establecer la base de la pila
    mov ebp, 0x27000
    mov esp, ebp
    
    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    ;imprimir_texto_mp pantalla_limpia, pantalla_limpia_len, 0x07, 0, 0
    mov esi, pantalla_limpia
    mov edi, 0x0
    mov ax, 0x40
    mov es, ax
    mov ecx, 4000
    rep movsw

    ; Inicializar la IDT
    call idt_inicializar
    
    ; Cargar IDT
    lidt [IDT_DESC]

    
    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov eax, 0x27000
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Inicializar el manejador de memoria
    call mmu_inicializar

    ;Test de identity mapping de tareas
    ;call test_tareas
    ;xchg bx, bx
    ;mov eax, 0x100000
    ;mov cr3, eax
    ;xchg bx, bx


 
    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp nombre_grupo, nombre_grupo_len, 0x07, 0, 47


    ; Configurar controlador de interrupciones
    call resetear_pic    
    call habilitar_pic

  
    ; test interrupcion de soft syscalll
    ; int 0x66
    push eax
    call keyPress
    pop eax


    ; Inicializar gdt tss descriptors
    call inicializar_gdt_tss

    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    call tss_inicializar    


    ; Inicializar el scheduler
    call inicializar_scheduler

    ; Imprimo algo
    call pantalla_inicial

    ; inicializar juego
    call inicializar_juego

   
    ; Cargar tarea inicial
    mov eax, 0x48
    ltr ax


    ; habilito interrupciones
    sti

    ; Saltar a la primera tarea: Idle
    jmp 0x50:0




    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;


flushtlb:
mov eax, cr3
mov cr3, eax
ret

;; inicializar_paginazion

;mmu_inicializar_dir_kernel:




%include "a20.asm"
