    .intel_syntax noprefix

    .text
    .global very_important_function

very_important_function:
    # Сохраняем на стеке регистры, которыми мы будем пользоваться
    # (только те, которые обязаны сохранить по calling conventions)
    push rbx # Текущий индекс i
    push r12 # Размер массива N
    push r13 # i-ый элемент массива A
    push r14 # Вычисленное значение на текущем шаге

    sub rsp, 8  # Выделяем 4 байта под int и выравниванием rsp

    mov r12, rdi # Первый аргумент (N) лежит в rdi

    xor rbx, rbx # Обнуляем счётчик

    .Loop:
        # Передаём аргументы через регистры и вызываем scanf
        mov rdi, offset scanf_format_string
        mov rsi, rsp
        call scanf

        movsxd r14, [rsp] # Читаем из rsp 4 байта и расширяем до 8 с учётом знака
        mov r13, A

        movsxd r13, [r13 + 4*rbx] # i-ый элемент
        imul r14, r13 # i-ый элемент * введённое число

        # Передаём аргументы через регистры и вызываем printf
        mov rdi, offset printf_format_string
        mov rsi, r14
        call printf

        inc rbx
        cmp r12, rbx
        ja .Loop

    # Освобождаем выделенную на стеке память
    add rsp, 8

    # Восстанавливаем сохранённые регистры
    pop r14
    pop r13
    pop r12
    pop rbx

    ret

    .section .data
scanf_format_string:
    .string "%d"
printf_format_string:
    .string "%lld\n"
