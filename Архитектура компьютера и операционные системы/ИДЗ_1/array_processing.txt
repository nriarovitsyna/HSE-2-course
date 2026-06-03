# Яровицына Наталья БПИ244
# array_processing.asm - Array Processing Module

.text

# Explicitly declare function as global
.globl build_array_B

# Subroutine: build_array_B
# Forms array B according to variant 30: elements A[i] > A[j] for all j < i
# Parameters: 
#   a0 - pointer to input array A
#   a1 - pointer to output array B  
#   a2 - size of array A
# Returns: 
#   a0 - size of newly created array B
# Uses stack for local variables and preserves callee-saved registers
build_array_B:
    # Prologue: allocate stack frame and save registers
    addi sp, sp, -28       # Allocate stack frame (7 words)
    sw ra, 24(sp)          # Save return address
    sw s0, 20(sp)          # Save s0 (will store array A pointer)
    sw s1, 16(sp)          # Save s1 (will store array B pointer) 
    sw s2, 12(sp)          # Save s2 (will store size of array A)
    sw s3, 8(sp)           # Save s3 (will store size of array B)
    sw s4, 4(sp)           # Save s4 (will store outer loop counter i)
    sw s5, 0(sp)           # Save s5 (will store inner loop counter j)
    
    # Copy parameters to saved registers
    mv s0, a0              # s0 = array A base address
    mv s1, a1              # s1 = array B base address  
    mv s2, a2              # s2 = size of array A
    li s3, 0               # Initialize size_B = 0
    
    # Outer loop: iterate through all elements of array A
    li s4, 0               # Initialize i = 0 (outer loop counter)
 
element_loop:
    bge s4, s2, return_result  # If i >= size_A, exit loop
    
    li t0, 1               # Initialize flag = 1 (true - element meets condition)
    
    # Inner loop: compare A[i] with all previous elements A[j] where j < i
    li s5, 0               # Initialize j = 0 (inner loop counter)
 
compare_previous_loop:
    bge s5, s4, check_flag # If j >= i, exit inner loop
    
    # Load A[i] into t2
    slli t1, s4, 2         # Calculate byte offset for i: t1 = i * 4
    add t1, s0, t1         # Calculate address of A[i]: t1 = A_base + i*4
    lw t2, 0(t1)           # Load A[i] into t2
    
    # Load A[j] into t4  
    slli t3, s5, 2         # Calculate byte offset for j: t3 = j * 4
    add t3, s0, t3         # Calculate address of A[j]: t3 = A_base + j*4
    lw t4, 0(t3)           # Load A[j] into t4
    
    # Check if A[i] > A[j]
    bgt t2, t4, next_comparison  # If A[i] > A[j], continue checking
    li t0, 0               # Else set flag = 0 (false - condition violated)
    j check_flag           # Break inner loop early
    
next_comparison:
    addi s5, s5, 1         # j++
    j compare_previous_loop # Continue inner loop

check_flag:
    # Check if current element meets the condition (flag == 1)
    beqz t0, next_element   # If flag == 0, skip adding to array B
    
    # Add A[i] to array B at position size_B
    slli t1, s4, 2         # Calculate byte offset for i: t1 = i * 4
    add t1, s0, t1         # Calculate address of A[i]: t1 = A_base + i*4
    lw t2, 0(t1)           # Load A[i] into t2
    
    slli t3, s3, 2         # Calculate byte offset for B position: t3 = size_B * 4
    add t3, s1, t3         # Calculate address of B[size_B]: t3 = B_base + size_B*4
    sw t2, 0(t3)           # Store A[i] into B[size_B]
    
    addi s3, s3, 1         # size_B++

next_element:
    addi s4, s4, 1         # i++
    j element_loop          # Continue outer loop

return_result:
    # Prepare return value and restore registers
    mv a0, s3              # Return size_B in a0
    
    # Epilogue: restore registers and deallocate stack frame
    lw s5, 0(sp)           # Restore s5
    lw s4, 4(sp)           # Restore s4  
    lw s3, 8(sp)           # Restore s3
    lw s2, 12(sp)          # Restore s2
    lw s1, 16(sp)          # Restore s1
    lw s0, 20(sp)          # Restore s0
    lw ra, 24(sp)          # Restore return address
    addi sp, sp, 28        # Deallocate stack frame
    ret                    # Return to caller