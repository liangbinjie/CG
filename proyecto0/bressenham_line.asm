default rel

section .bss

current_x    resq 1
current_y    resq 1
end_x        resq 1
end_y        resq 1

dx_value     resq 1
dy_value     resq 1

d            resq 1
delta_D      resq 1
delta_R      resq 1

dir          resq 1

; ========================

section .text
global bressenham_line
extern plotMesa

bressenham_line:
    ; Input: rdi = x0, rsi = y0, rdx = x1, rcx = y1
    
    ; The arguments from C (int) are converted in signed 64-bit
    movsxd rdi, edi
    movsxd rsi, esi
    movsxd rdx, edx
    movsxd rcx, ecx
    
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8
    
    ; Calculate differences: r8 = (x1-x0), r9 = (y1-y0)
    mov r8, rdx        ; r8 = x1
    sub r8, rdi        ; r8 = x1 - x0
    mov r9, rcx        ; r9 = y1
    sub r9, rsi        ; r9 = y1 - y0

    ; Determine the absolute values of dx and dy
    mov r10, r8        ; r10 = dx
    cmp r10, 0
    jge .dx_positive
    neg r10            ; if dx < 0, make it positive abs(dx)

.dx_positive:
    mov r11, r9        ; r11 = dy
    cmp r11, 0
    jge .dy_positive
    neg r11            ; if dy < 0, make it positive abs(dy)

.dy_positive:
    ; Compare absolute values to determine dominant axis
    cmp r10, r11       ; compare abs(dx) and abs(dy)
    jg .x_dominant     ; if abs(dx) > abs(dy), X is dominant, which means, we do bressenhamH
    jmp .y_dominant     ; else, Y is dominant, which means, we do bressenhamV

.x_dominant:
    ; X dominant lines (bressenhamH())

    ; SWAP if necessary to ensure we draw from left to right
    cmp rdi, rdx       ; compare x0 and x1
    jle .no_swap_x     ; if x0 <= x1, no swap needed
    ; Swap x0 and x1
    xchg rdi, rdx      ; swap x0 and x1
    ; Swap y0 and y1
    xchg rsi, rcx      ; swap y0 and y1

.no_swap_x:

    mov [current_x], rdi
    mov [current_y], rsi
    mov [end_x], rdx
    mov [end_y], rcx

    ; dx = x1 - x0
    mov r10, rdx
    sub r10, rdi

    ; dy firmado = y1 - y0
    mov r11, rcx
    sub r11, rsi

    ; Determinar dirección de Y
    cmp r11, 0
    jge .x_dy_positive

    mov qword [dir], -1
    neg r11                 ; dy = abs(dy)
    jmp .x_dir_ready

.x_dy_positive:
    mov qword [dir], 1

.x_dir_ready:

    ; Guardar dx y |dy|
    mov [dx_value], r10
    mov [dy_value], r11

    ; delta_R = 2 * dy
    mov r12, r11
    shl r12, 1
    mov [delta_R], r12

    ; delta_D = 2 * (dy - dx)
    mov r13, r11
    sub r13, r10
    shl r13, 1
    mov [delta_D], r13

    ; d = 2*dy - dx
    mov r14, r11
    shl r14, 1
    sub r14, r10
    mov [d], r14

.loop_x:

    ; ¿Did we pass x1?
    mov rax, [current_x]
    cmp rax, [end_x]
    jg .x_done

    ; Get current x and y
    mov rdi, [current_x]
    mov rsi, [current_y]

    ; Call plot function to draw the point
    call plotMesa

    ; Check if we have reached the end point
    cmp rdi, [end_x]   ; compare current x with x1
    jg .x_done         ; if current x > x1, we are done

    ; Update decision parameter d
    mov r14, [d]       ; load current d
    cmp r14, 0         ; compare d with 0
    jg .update_d_positive

    ; If d < 0, choose E (East)
    add r14, [delta_R] ; d = d + 2 * abs(dy)
    mov [d], r14       ; store updated d
    inc qword [current_x] ; move to next x

    jmp .loop_x        ; repeat the loop

.update_d_positive:
    ; If d >= 0, choose NE (North East)
    add r14, [delta_D] ; d = d + (2 * abs(dy) - 2 * abs(dx))
    mov [d], r14       ; store updated d
    inc qword [current_x] ; move to next x
    mov rax, [dir]
    add [current_y], rax ; move to next y based on direction
    jmp .loop_x        ; repeat the loop

.x_done:
    ; Exit the function
    jmp .done


.y_dominant:
    ; Y dominant lines (bressenhamV())

    ; SWAP if necessary to ensure we draw from bottom to top
    cmp rsi, rcx       ; compare y0 and y1
    jle .no_swap_y     ; if y0 <= y1, no swap needed
    ; Swap x0 and x1
    xchg rdi, rdx      ; swap x0 and x1
    ; Swap y0 and y1
    xchg rsi, rcx      ; swap y0 and y1

.no_swap_y:

    mov [current_x], rdi
    mov [current_y], rsi
    mov [end_x], rdx
    mov [end_y], rcx

    ; dx firmado = x1 - x0
    mov r10, rdx
    sub r10, rdi

    ; dy = y1 - y0
    mov r11, rcx
    sub r11, rsi

    ; Dirección de X
    cmp r10, 0
    jge .y_dx_positive

    mov qword [dir], -1
    neg r10                 ; dx = abs(dx)
    jmp .y_dir_ready

.y_dx_positive:
    mov qword [dir], 1

.y_dir_ready:

    mov [dx_value], r10
    mov [dy_value], r11

    ; delta_R = 2*dx
    mov r12, r10
    shl r12, 1
    mov [delta_R], r12

    ; delta_D = 2*(dx-dy)
    mov r13, r10
    sub r13, r11
    shl r13, 1
    mov [delta_D], r13

    ; d = 2*dx-dy
    mov r14, r10
    shl r14, 1
    sub r14, r11
    mov [d], r14

    ; Start the loop to draw the line
.loop_y:
    
    mov rax, [current_y]
    cmp rax, [end_y]
    jg .y_done
    
    ; Get current x and y
    mov rdi, [current_x]
    mov rsi, [current_y]

    ; Call plot function to draw the point
    call plotMesa

    ; Check if we have reached the end point
    cmp rsi, [end_y]   ; compare current y with y1
    jg .y_done         ; if current y > y1, we are done

    ; Update decision parameter d
    mov r14, [d]       ; load current d
    cmp r14, 0         ; compare d with 0
    jg .update_d_positive_y

    ; If d < 0, choose N (North)
    add r14, [delta_R] ; d = d + 2 * abs(dx)
    mov [d], r14       ; store updated d
    inc qword [current_y] ; move to next y

    jmp .loop_y        ; repeat the loop

.update_d_positive_y:
    ; If d >= 0, choose NE (North East)
    add r14, [delta_D] ; d = d + (2 * abs(dx) - 2 * abs(dy))
    mov [d], r14       ; store updated d
    inc qword [current_y] ; move to next y
    mov rax, [dir]
    add [current_x], rax ; move to next x based on direction
    jmp .loop_y        ; repeat the loop

.y_done:
    ; Exit the function
    jmp .done
    
.done:
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    ret
