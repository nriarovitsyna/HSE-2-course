# macros_lib.asm
# Macros for input/output and data processing

# Macro for string output
.macro PRINT_STRING (%str)
    li a7, 4
    la a0, %str
    ecall
.end_macro

# Macro for floating point number output
.macro PRINT_FLOAT (%reg)
    li a7, 2
    fmv.s fa0, %reg
    ecall
.end_macro

# Macro for floating point number input
.macro INPUT_FLOAT (%reg)
    li a7, 6
    ecall
    fmv.s %reg, fa0
.end_macro

# Macro for integer input
.macro INPUT_INT (%reg)
    li a7, 5
    ecall
    mv %reg, a0
.end_macro

# Macro for arccos result output
.macro PRINT_ARCCOS_RESULT (%x_reg, %result_reg, %result_msg, %result_msg2, %newline)
    addi sp, sp, -16
    sw ra, 12(sp)
    fsw %x_reg, 8(sp)
    fsw %result_reg, 4(sp)
    
    PRINT_STRING(%result_msg)
    PRINT_FLOAT(%x_reg)
    PRINT_STRING(%result_msg2)
    PRINT_FLOAT(%result_reg)
    PRINT_STRING(%newline)
    
    flw %result_reg, 4(sp)
    flw %x_reg, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
.end_macro

# Macro for single value processing
.macro PROCESS_SINGLE (%prompt, %error_msg, %result_msg, %result_msg2, %newline)
    addi sp, sp, -16
    sw ra, 12(sp)
    fsw fs0, 8(sp)
    fsw fs1, 4(sp)
    
    PRINT_STRING(%newline)
    
input_loop:
    PRINT_STRING(%prompt)
    INPUT_FLOAT(fs0)
    
    # Range validation
    # Parameter: fs0 (user input value) -> fa0
    # Return: a0 = 1 if valid, 0 if invalid
    fmv.s fa0, fs0
    jal check_range
    bnez a0, range_ok
    
    PRINT_STRING(%error_msg)
    j input_loop

range_ok:
    # Parameter: fs0 (x value) -> fa0
    # Return: fa0 = arccos(x) result
    fmv.s fa0, fs0
    jal calc_arccos
    fmv.s fs1, fa0
    
    # Output result: fs0 (original x), fs1 (arccos result)
    PRINT_ARCCOS_RESULT(fs0, fs1, %result_msg, %result_msg2, %newline)
    
    PRINT_STRING(%newline)
    
    flw fs1, 4(sp)
    flw fs0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
.end_macro

# Macro for array processing
.macro PROCESS_ARRAY (%size_prompt, %error_size, %single_prompt, %error_range, %result_msg, %result_msg2, %newline, %input_buf, %output_buf, %max_size)
    addi sp, sp, -32
    sw ra, 28(sp)
    sw s0, 24(sp)
    sw s1, 20(sp)
    sw s2, 16(sp)
    sw s3, 12(sp)
    
    PRINT_STRING(%newline)

input_size:
    PRINT_STRING(%size_prompt)
    INPUT_INT(s1)
    
    li t0, 1
    blt s1, t0, error_size
    li t0, %max_size
    bgt s1, t0, error_size
    j size_ok

error_size:
    PRINT_STRING(%error_size)
    j input_size

size_ok:
    la s2, %input_buf
    la s3, %output_buf
    li s0, 0

array_input_loop:
    bge s0, s1, array_input_done
    
element_input:
    PRINT_STRING(%single_prompt)
    INPUT_FLOAT(fa0)
    
    # Parameter: fa0 (user input value) -> fa0
    # Return: a0 = 1 if valid, 0 if invalid
    jal check_range
    bnez a0, element_ok
    
    PRINT_STRING(%error_range)
    j element_input

element_ok:
    # Store user input to input_buf[i]
    slli t0, s0, 2
    add t1, s2, t0
    fsw fa0, 0(t1)
    addi s0, s0, 1
    j array_input_loop

array_input_done:
    li s0, 0

array_calc_arccos:
    bge s0, s1, array_calc_arccos_done
    # Load input_buf[i] -> fa0
    slli t0, s0, 2
    add t1, s2, t0
    flw fa0, 0(t1)
    
    # Parameter: fa0 (input_buf[i]) -> fa0
    # Return: fa0 = arccos(input_buf[i])
    jal calc_arccos
    fmv.s fa1, fa0
    
    # Store result to output_buf[i]
    slli t0, s0, 2
    add t1, s3, t0
    fsw fa1, 0(t1)
    addi s0, s0, 1
    j array_calc_arccos

array_calc_arccos_done:
    li s0, 0

array_output:
    bge s0, s1, array_output_done
    # Load input_buf[i] -> fa0, output_buf[i] -> fa1
    slli t0, s0, 2
    add t1, s2, t0
    flw fa0, 0(t1)
    add t1, s3, t0
    flw fa1, 0(t1)
    
    # Output: fa0 (original value), fa1 (arccos result)
    PRINT_ARCCOS_RESULT(fa0, fa1, %result_msg, %result_msg2, %newline)
    
    addi s0, s0, 1
    j array_output

array_output_done:
    PRINT_STRING(%newline)
    
    lw s3, 12(sp)
    lw s2, 16(sp)
    lw s1, 20(sp)
    lw s0, 24(sp)
    lw ra, 28(sp)
    addi sp, sp, 32
.end_macro