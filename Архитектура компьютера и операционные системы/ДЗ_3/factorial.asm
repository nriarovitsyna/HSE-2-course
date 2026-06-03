# Яровицына Наталья БПИ244
.data
ans_text: .asciz "Максимальный аргумент для 32-битного факториала: "
newline:    .asciz "\n"

.text
.globl main

# Макрос для вызова factorial с непосредственным значением аргумента
.macro factorial_imm %n
    li a0, %n             
    jal recursive_fact        
.end_macro

# Макрос для вызова factorial со значением аргумента из регистра
.macro factorial_reg %reg
    mv a0, %reg          
    jal recursive_fact        
.end_macro

main:
    li s0, 1              # Инициализация: n = 1 (текущее значение аргумента)
    li s1, 0              # Флаг определения типа вызова (первый/последующие)

# Основной цикл проверки факториалов на переполнение
loop:
    # Проверка флага: если первый вызов, используем макрос с числом 1
    beqz s1, first_call    
    
    # Вызов factorial для текущего значения n через макрос factorial_reg
    factorial_reg s0      
    j continue_loop  
    
# Обработчик первого тестового вычисления  
first_call:
    li s1, 1              # Устанавливаем флаг - первый вызов завершен
    factorial_imm 1       # Первый вызов через макрос с непосредственным значением 1
    
# Метка продолжения после успешного вычисления факториала
continue_loop:
    # Проверка на переполнение
    bltz a0, overflow
    
    # Увеличиваем n на 1 и продолжаем цикл
    addi s0, s0, 1       
    j loop

# Обработка переполнения
overflow:
    # Корректировка n: максимальное значение на 1 меньше текущего
    addi s0, s0, -1
    
    # Вывод результата
    la a0, ans_text    
    li a7, 4              
    ecall                 
    
    mv a0, s0             
    li a7, 1              
    ecall                 
    
    # Перевод строки
    la a0, newline        
    li a7, 4              
    ecall                
    
    # Возвращаем найденный аргумент в регистре a0
    mv a0, s0             
    
    # Завершаем программу
    li a7, 10             
    ecall      
              
# Рекурсивная функция вычисления факториала
recursive_fact:
    # Базовый случай: если n <= 1, возвращаем 1
    li t0, 1
    ble a0, t0, base_case  
    
    # Сохраняем в стеке возвратный адрес и текущее значение n
    addi sp, sp, -8       
    sw ra, 0(sp)          
    sw a0, 4(sp)          
    
    # Рекурсивный вызов для n-1
    addi a0, a0, -1       
    jal recursive_fact        
    
    # Проверяем переполнение из рекурсивного вызова
    bltz a0, overflow_return 
    
    # Восстанавливаем оригинальное значение n из стека
    lw t1, 4(sp)          
    
    # Проверка: не превысит ли fact(n-1) * n максимальное значение
    li t2, 0x7FFFFFFF     
    div t3, t2, t1        
    blt t3, a0, set_overflow
    
    # Умножаем результат рекурсивного вызова на n
    mul a0, a0, t1        
    j return_factorial    

# Базовый случай: factorial(0) = factorial(1) = 1
base_case:
    li a0, 1              
    ret                  

# Обработчик переполнения при умножении fact(n-1) * n
set_overflow:
    li a0, -1             # Возвращаем -1 как признак переполнения

# Восстановление регистров и возврат из рекурсивного вызова
return_factorial:
    lw ra, 0(sp)          
    addi sp, sp, 8        
    ret                  

# Обработка переполнения из рекурсивного вызова
overflow_return:
    lw ra, 0(sp)          
    addi sp, sp, 8        
    ret                   
