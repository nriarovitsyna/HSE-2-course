# Яровицына Наталья БПИ244
# Macro Library: macrolib.asm

# Macro: REMOVE_NEWLINE
# Wrapper for remove_newline subroutine - removes newline from string
# Parameters: %str - label of string to process
# Uses: a0 (string pointer)
.macro REMOVE_NEWLINE(%str)
    la a0, %str            # Load address of input string into a0
    jal remove_newline      # Call remove_newline subroutine
.end_macro

# Macro: PRINT_ARRAY
# Wrapper for print_array subroutine - prints array with formatting
# Parameters: 
#   %arr - label of array to print
#   %size - register or immediate value containing array size
# Uses: a0 (array pointer), a1 (array size)
.macro PRINT_ARRAY(%arr, %size)
    la a0, %arr            # Load address of array into a0
    mv a1, %size           # Move array size value into a1
    jal print_array        # Call print_array subroutine
.end_macro

# Macro: FORM_ARRAY_B  
# Wrapper for build_array_B subroutine - forms array B from array A
# Parameters:
#   %arrA - label of source array A
#   %arrB - label of destination array B  
#   %sizeA - register or immediate value containing size of array A
# Uses: a0 (array A pointer), a1 (array B pointer), a2 (array A size)
# Returns: a0 contains size of newly formed array B
.macro FORM_ARRAY_B(%arrA, %arrB, %sizeA)
    la a0, %arrA           # Load address of source array A into a0
    la a1, %arrB           # Load address of destination array B into a1
    mv a2, %sizeA          # Move size of array A into a2
    jal build_array_B       # Call build_array_B subroutine
.end_macro

# Macro: RUN_TESTS
# Wrapper for run_tests subroutine - executes automated test suite
# Parameters: None
# Uses: Preserves all registers through subroutine call
.macro RUN_TESTS
    jal run_tests          # Call automated test runner subroutine
.end_macro

# Macro: RUN_TEST
# Wrapper for individual test execution - runs single test case
# Parameters:
#   %arrA - label of test input array A
#   %sizeA - immediate value for size of test array A
#   %arrB - label of expected output array B
#   %expected_size - immediate value for expected size of array B
# Uses: a0 (test array A), a1 (test size), a2 (expected array B), a3 (expected size)
.macro RUN_TEST(%arrA, %sizeA, %arrB, %expected_size)
    la a0, %arrA           # Load address of test input array A
    li a1, %sizeA          # Load immediate value for array A size
    la a2, %arrB           # Load address of expected output array B
    li a3, %expected_size  # Load immediate value for expected array B size
    jal run_test           # Call individual test runner subroutine
.end_macro

# Macro: PRINT_STR
# Direct system call wrapper for string output
# Parameters: %str - label of string to print
# Uses: a0 (string pointer), a7 (system call number)
# System Call: 4 - print string
.macro PRINT_STR(%str)
    la a0, %str            # Load address of string to print
    li a7, 4               # System call code: print string
    ecall                  # Execute system call
.end_macro

# Macro: PRINT_INT
# Direct system call wrapper for integer output
# Parameters: %reg - register containing integer value to print
# Uses: a0 (integer value), a7 (system call number)
# System Call: 1 - print integer
.macro PRINT_INT(%reg)
    mv a0, %reg            # Move integer value from source register to a0
    li a7, 1               # System call code: print integer
    ecall                  # Execute system call
.end_macro

# Macro: INPUT_STR
# Direct system call wrapper for string input
# Parameters:
#   %buf - label of buffer to store input string
#   %size - immediate value for buffer size
# Uses: a0 (buffer pointer), a1 (buffer size), a7 (system call number)
# System Call: 8 - read string
.macro INPUT_STR(%buf, %size)
    la a0, %buf            # Load address of input buffer
    li a1, %size           # Load immediate value for buffer size
    li a7, 8               # System call code: read string
    ecall                  # Execute system call
.end_macro

# Macro: EXIT
# Direct system call wrapper for program termination
# Parameters: None
# Uses: a7 (system call number)
# System Call: 10 - exit program

.macro EXIT
    li a7, 10              # System call code: exit program
    ecall                  # Execute system call (terminates program)
.end_macro