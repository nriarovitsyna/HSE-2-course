# Яровицына Наталья БПИ244
# test.asm - Automated Testing Module

.data
    # Test case 1: Invalid size (N=0) - should trigger size error
    test1_A: .word 0                    # N=0
    test1_B: .word 0 : 10
    
    # Test case 2: Invalid size (N=11) - exceeds maximum allowed size
    test2_A: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11  # N=11
    test2_B: .word 0 : 10
    
    # Test case 3: All elements equal [2,2,2] - only first element should pass
    test3_A: .word 2, 2, 2              # N=3
    test3_B: .word 0 : 10
    
    # Test case 4: Strictly decreasing sequence [5,4,3,2,1] - only first element
    test4_A: .word 5, 4, 3, 2, 1        # N=5
    test4_B: .word 0 : 10
    
    # Test case 5: Strictly increasing sequence [1,2,3,4,5] - all elements
    test5_A: .word 1, 2, 3, 4, 5        # N=5  
    test5_B: .word 0 : 10
    
    # Test case 6: Mixed sequence [1,2,5,4,3] - elements greater than all previous
    test6_A: .word 1, 2, 5, 4, 3        # N=5
    test6_B: .word 0 : 10
    
    # Test output messages
    test_pass: .asciz "Test PASSED\n"
    test_fail: .asciz "Test FAILED\n"
    test_N:    .asciz "N = "
    test_A:    .asciz "A = "
    test_B:    .asciz "B = "
    test_error: .asciz "Error: invalid array size\n"
    test_expected: .asciz "Expected size: "
    test_actual: .asciz "Actual size: "
    nl:        .asciz "\n"
    spc:       .asciz " "
    test_header: .asciz "=== AUTOMATED TESTS ===\n"
    test_separator: .asciz "-------------------\n"

.text
.globl run_tests, run_test, check_N

# Subroutine: run_tests
# Executes comprehensive test suite for array processing functionality
# Tests include: boundary cases, invalid inputs, various data patterns
# Stack: Allocates 4 bytes, saves ra
run_tests:
    addi sp, sp, -4        # Allocate stack frame
    sw ra, 0(sp)           # Save return address
    
    # Display test suite header
    PRINT_STR(test_header)
    
    # Test 1: N=0 (invalid size - lower boundary)
    PRINT_STR(test_separator)
    la a0, test1_A         # Load test array A address
    li a1, 0               # Array size = 0 (invalid)
    la a2, test1_B         # Load output array B address
    li a3, 0               # Expected output size = 0 (error case)
    jal run_test           # Execute individual test
    
    # Test 2: N=11 (invalid size - upper boundary)
    PRINT_STR(test_separator)
    la a0, test2_A         # Load test array A address
    li a1, 11              # Array size = 11 (exceeds maximum)
    la a2, test2_B         # Load output array B address
    li a3, 0               # Expected output size = 0 (error case)
    jal run_test           # Execute individual test
    
    # Test 3: N=3, [2,2,2] (all elements equal - only first qualifies)
    PRINT_STR(test_separator)
    la a0, test3_A         # Load test array A address
    li a1, 3               # Array size = 3
    la a2, test3_B         # Load output array B address
    li a3, 1               # Expected output size = 1 (only element 2)
    jal run_test           # Execute individual test
    
    # Test 4: N=5, [5,4,3,2,1] (strictly decreasing - only first qualifies)
    PRINT_STR(test_separator)
    la a0, test4_A         # Load test array A address
    li a1, 5               # Array size = 5
    la a2, test4_B         # Load output array B address
    li a3, 1               # Expected output size = 1 (only element 5)
    jal run_test           # Execute individual test
    
    # Test 5: N=5, [1,2,3,4,5] (strictly increasing - all qualify)
    PRINT_STR(test_separator)
    la a0, test5_A         # Load test array A address
    li a1, 5               # Array size = 5
    la a2, test5_B         # Load output array B address
    li a3, 5               # Expected output size = 5 (all elements)
    jal run_test           # Execute individual test
    
    # Test 6: N=5, [1,2,5,4,3] (mixed sequence - elements 1,2,5 qualify)
    PRINT_STR(test_separator)
    la a0, test6_A         # Load test array A address
    li a1, 5               # Array size = 5
    la a2, test6_B         # Load output array B address
    li a3, 3               # Expected output size = 3 (elements 1,2,5)
    jal run_test           # Execute individual test
    
    PRINT_STR(test_separator) # Final separator after all tests
    
    lw ra, 0(sp)           # Restore return address
    addi sp, sp, 4         # Deallocate stack frame
    ret                    # Return to caller

# Subroutine: run_test
# Executes single test case with full validation and reporting
# Parameters:
#   a0 - address of input array A
#   a1 - size of array A (N)
#   a2 - address of output array B
#   a3 - expected size of array B for test validation
# Uses: s0 (array A address), s1 (size N), s2 (array B address), 
#       s3 (expected size), s4 (actual size)
# Stack: Allocates 24 bytes, saves ra, s0-s4
run_test:
    addi sp, sp, -24       # Allocate stack frame (6 words)
    sw ra, 20(sp)          # Save return address
    sw s0, 16(sp)          # Save s0 (array A address)
    sw s1, 12(sp)          # Save s1 (array size N)
    sw s2, 8(sp)           # Save s2 (array B address)
    sw s3, 4(sp)           # Save s3 (expected output size)
    sw s4, 0(sp)           # Save s4 (actual output size)
    
    mv s0, a0              # Store array A address
    mv s1, a1              # Store array size N
    mv s2, a2              # Store array B address
    mv s3, a3              # Store expected output size
    
    # Display test array size information
    PRINT_STR(test_N)      # "N = " label
    PRINT_INT(s1)          # Print actual array size
    PRINT_STR(nl)          # Newline for formatting
    
    # Validate array size before proceeding with processing
    mv a0, s1              # Pass array size to validation function
    jal check_N            # Call size validation subroutine
    beqz a0, test_invalid_N # Jump to error handling if size invalid
    
    # Display original array A contents
    PRINT_STR(test_A)      # "A = " label
    mv a0, s0              # Array A address
    mv a1, s1              # Array A size
    jal print_array        # Call array printing subroutine
    
    # Process array A to generate array B according to variant rules
    mv a0, s0              # Source array A address
    mv a1, s2              # Destination array B address  
    mv a2, s1              # Source array size
    jal build_array_B      # Call array processing subroutine
    mv s4, a0              # Store actual output array size
    
    # Display resulting array B contents
    PRINT_STR(test_B)      # "B = " label
    mv a0, s2              # Array B address
    mv a1, s4              # Array B size
    jal print_array        # Call array printing subroutine
    
    # Display expected vs actual results for comparison
    PRINT_STR(test_expected) # "Expected size: " label
    PRINT_INT(s3)          # Print expected output size
    PRINT_STR(nl)          # Newline for formatting
    
    PRINT_STR(test_actual) # "Actual size: " label
    PRINT_INT(s4)          # Print actual output size
    PRINT_STR(nl)          # Newline for formatting
    
    # Validate test results by comparing expected vs actual
    beq s4, s3, test_passed # Jump to success if sizes match
    j test_failed          # Jump to failure if sizes differ

test_invalid_N:
    # Handle invalid array size test case
    PRINT_STR(test_error)  # Display size error message
    PRINT_STR(test_expected) # "Expected size: " label
    PRINT_INT(s3)          # Print expected output size
    PRINT_STR(nl)          # Newline for formatting
    
    li s4, -1              # Mark actual size as error indicator
    j test_passed          # Consider error case as expected behavior

test_failed:
    # Handle test failure - expected vs actual mismatch
    PRINT_STR(test_fail)   # Display failure message
    j test_end             # Jump to cleanup

test_passed:
    # Handle test success - expected matches actual
    PRINT_STR(test_pass)   # Display success message

test_end:
    # Cleanup and return from test execution
    lw ra, 20(sp)          # Restore return address
    lw s0, 16(sp)          # Restore s0
    lw s1, 12(sp)          # Restore s1
    lw s2, 8(sp)           # Restore s2
    lw s3, 4(sp)           # Restore s3
    lw s4, 0(sp)           # Restore s4
    addi sp, sp, 24        # Deallocate stack frame
    ret                    # Return to test suite

# Subroutine: check_N
# Validates array size against program constraints (1-10 elements)
# Parameters: a0 - array size N to validate
# Returns: a0 - validation result (1 = valid, 0 = invalid)
check_N:
    li t0, 1               # Minimum allowed array size
    blt a0, t0, invalid_N  # Jump to invalid if size < 1
    li t0, 10              # Maximum allowed array size
    bgt a0, t0, invalid_N  # Jump to invalid if size > 10
    li a0, 1               # Return 1 (valid size)
    ret                    # Return to caller
    
invalid_N:
    li a0, 0               # Return 0 (invalid size)
    ret                    # Return to caller