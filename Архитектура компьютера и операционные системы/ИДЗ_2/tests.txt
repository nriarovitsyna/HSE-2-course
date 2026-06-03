# tests.asm
# Automated test suite for arccos calculation module

.data
    # Test messages
    test1_msg:      .asciz "Test 1: x = 1.0"
    test2_msg:      .asciz "Test 2: x = -1.0"
    test3_msg:      .asciz "Test 3: x = 0.0"
    test4_msg:      .asciz "Test 4: x = 0.66"
    test5_msg:      .asciz "Test 5: x = -0.33"
    test6_msg:      .asciz "Test 6: x = 0.99"
    test7_msg:      .asciz "Test 7: x = -0.99"
    error_rate_msg: .asciz "Error rate: "
    percent_msg:    .asciz "%"
    test_passed_msg:    .asciz "Test PASSED (error < 0.1%)"
    test_failed_msg:    .asciz "Test FAILED (error > 0.1%)"
    
    # Test values
    test_val1:      .float 1.0
    test_val2:      .float -1.0
    test_val3:      .float 0.0
    test_val4:      .float 0.66
    test_val5:      .float -0.33
    test_val6:      .float 0.99
    test_val7:      .float -0.99
    
    # Expected results (arccos values)
    expected1:      .float 0.0
    expected2:      .float 3.14159265
    expected3:      .float 1.57079633
    expected4:      .float 0.849985
    expected5:      .float 1.907099
    expected6:      .float 0.141540
    expected7:      .float 3.000053
    
    # Constants for comparison
    tolerance:      .float 0.001     # 0.1% tolerance
    hundred:        .float 100.0     # 100 for percentage calculation

.text

# Automated testing function
run_tests:
    addi sp, sp, -8
    sw ra, 4(sp)
    
    # Test 1: x = 1.0
    la a0, test1_msg
    la a1, test_val1
    la a2, expected1
    jal run_single_test
    
    # Test 2: x = -1.0
    la a0, test2_msg
    la a1, test_val2
    la a2, expected2
    jal run_single_test
    
    # Test 3: x = 0.0
    la a0, test3_msg
    la a1, test_val3
    la a2, expected3
    jal run_single_test
    
    # Test 4: x = 0.66
    la a0, test4_msg
    la a1, test_val4
    la a2, expected4
    jal run_single_test
    
    # Test 5: x = -0.33
    la a0, test5_msg
    la a1, test_val5
    la a2, expected5
    jal run_single_test
    
    # Test 6: x = 0.99
    la a0, test6_msg
    la a1, test_val6
    la a2, expected6
    jal run_single_test
    
    # Test 7: x = -0.99
    la a0, test7_msg
    la a1, test_val7
    la a2, expected7
    jal run_single_test
    
    PRINT_STRING(newline)
    
    lw ra, 4(sp)
    addi sp, sp, 8
    ret
    
# General function for running a single test case
# Parameters: a0 - address of test message, a1 - address of test value, a2 - address of expected value
run_single_test:
    addi sp, sp, -20
    sw ra, 16(sp)
    sw a0, 12(sp)              
    sw a1, 8(sp)               
    sw a2, 4(sp)               
    
    # Print test message
    PRINT_STRING(newline)
    lw a0, 12(sp)              
    li a7, 4                   
    ecall
    PRINT_STRING(newline)
    
    # Load test value and calculate arccos
    lw a1, 8(sp)               
    flw fa0, 0(a1)             
    jal calc_arccos            
    fmv.s fa1, fa0             
    
    # Print result
    flw fa0, 0(a1)             
    PRINT_ARCCOS_RESULT(fa0, fa1, result_msg, result_msg2, newline)
    
    # Check result against expected value
    lw a0, 4(sp)               
    flw fa2, 0(a0)             
    fmv.s fa0, fa1             
    fmv.s fa1, fa2             
    jal check_result           
    
    lw ra, 16(sp)
    addi sp, sp, 20
    ret

# Function to check if result is within 0.1% of expected value
# Parameters: fa0 - actual result, fa1 - expected result
check_result:
    addi sp, sp, -16
    sw ra, 12(sp)
    fsw fa0, 8(sp)             # Save actual result
    fsw fa1, 4(sp)             # Save expected result
    
    # Check if actual equals expected
    feq.s t0, fa0, fa1         # t0 = 1 if actual == expected, else 0
    bnez t0, test_pass         # If equal, test passes immediately
    
    # Load constants
    la t0, tolerance
    flw ft2, 0(t0)             # ft2 = 0.1% tolerance (0.001)
    la t0, hundred
    flw ft5, 0(t0)             # ft5 = 100.0
    
    # Calculate absolute error
    fsub.s ft0, fa0, fa1       # ft0 = actual - expected
    fabs.s ft0, ft0            # ft0 = |error|
    
    # Calculate relative error percentage: (|error| / |expected|) * 100
    fabs.s ft1, fa1            # ft1 = |expected|
    fmul.s ft3, ft1, ft2       # ft3 = |expected| * 0.001 (allowed error)
    
    # Compare error with tolerance
    flt.s t0, ft0, ft3         # t0 = 1 if error < tolerance, else 0
    
    # Calculate and print error percentage for information
    fdiv.s ft4, ft0, ft1       # ft4 = |error| / |expected|
    fmul.s ft4, ft4, ft5       # ft4 = error percentage
    
    PRINT_STRING(error_rate_msg)
    PRINT_FLOAT(ft4)
    PRINT_STRING(percent_msg)
    PRINT_STRING(newline)
    
    beqz t0, test_fail         # If error >= tolerance, test fails
    
test_pass:
    PRINT_STRING(test_passed_msg)
    PRINT_STRING(newline)
    j check_done
    
test_fail:
    PRINT_STRING(test_failed_msg)
    PRINT_STRING(newline)
    
check_done:
    lw ra, 12(sp)
    addi sp, sp, 16
    ret