# main.asm
# Main program module for arccos calculation
# Provides interactive menu for calculating arccos values
# Features: single value calculation, array processing, automated testing
# Dependencies: macros_lib.asm, arccos.asm, tests.asm

.data
    # Text messages for user interface
    menu_prompt:    .asciz "========= MENU =========\n1. Calculate arccos for single value\n2. Calculate arccos for array\n3. Run automated testing\n4. Exit program\n\nSelect option (1-4): "
    single_prompt:  .asciz "Enter x in range [-1, 1]: "
    size_prompt:    .asciz "Enter size of array (1-10): "
    result_msg:     .asciz "arccos("
    result_msg2:    .asciz ") = "
    error_range:    .asciz "ERROR: x must be in range [-1, 1]\n"
    error_size:     .asciz "ERROR: size must be from 1 to 10\n"
    error_menu:     .asciz "ERROR: select option from 1 to 4\n"
    newline:        .asciz "\n"
    
    # Buffers for storing input and output array data
    input_array:    .float 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    output_array:   .float 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0

.text
.globl main

.include "macros_lib.asm"  # Include macros file

main:
    addi sp, sp, -16
    sw ra, 12(sp)    # Save return address for correct exit from main
    sw s0, 8(sp)     # Save s0 register for storing menu choice

main_loop:
    PRINT_STRING(menu_prompt)
    INPUT_INT(s0)    # Input macro call: result -> s0 (user choice)
    
    li t0, 1
    beq s0, t0, single_processing
    li t0, 2
    beq s0, t0, array_processing
    li t0, 3
    beq s0, t0, automated_testing
    li t0, 4
    beq s0, t0, exit_program
    
    PRINT_STRING(error_menu)  # Output macro: parameter -> error_menu (error string)
    PRINT_STRING(newline)
    j main_loop

single_processing:
    # PROCESS_SINGLE macro call:
    # Parameters: 
    #   %prompt = single_prompt (input prompt message)
    #   %error_msg = error_range (range validation error message)  
    #   %result_msg, %result_msg2, %newline (output formatting)
    # Return: calculated arccos value printed to console
    PROCESS_SINGLE(single_prompt, error_range, result_msg, result_msg2, newline)
    j main_loop

array_processing:
    # PROCESS_ARRAY macro call:
    # Parameters:
    #   %size_prompt, %error_size (array size input and validation)
    #   %single_prompt, %error_range (element input and validation)
    #   %result_msg, %result_msg2, %newline (output formatting)
    #   %input_buf = input_array, %output_buf = output_array (data buffers)
    #   %max_size = 10 (maximum array size)
    # Return: filled output_array and all calculated values printed to console
    PROCESS_ARRAY(size_prompt, error_size, single_prompt, error_range, result_msg, result_msg2, newline, input_array, output_array, 10)
    j main_loop

automated_testing:
    addi sp, sp, -8
    sw ra, 4(sp)
    # run_tests call:
    # Parameters: implicitly through global data or registers
    # Returned result: test statistics printed to screen
    jal run_tests    # Testing result -> console output
    lw ra, 4(sp)
    addi sp, sp, 8
    j main_loop

exit_program:
    lw s0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    li a7, 10
    ecall        

# Include arccos implementation and tests files
.include "arccos.asm"
.include "tests.asm"