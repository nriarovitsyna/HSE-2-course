# Яровицына Наталья БПИ244
# main.asm - Main Program Module

.data
# String constants for user interface
prompt:         .asciz "Enter file path: "
buffer:         .space 256
file_buffer:    .space 256
array_A:        .word 0:10
array_B:        .word 0:10
size_A:         .word 0
size_B:         .word 0
error_file:     .asciz "ERROR: The file cannot be opened!\n"
error_size:     .asciz "ERROR: Invalid array size (allowed size 1-10)!\n"
msg_A:          .asciz "A: "
msg_B:          .asciz "B: "
msg_n:          .asciz "N = "
newline:        .asciz "\n"
space:          .asciz " "
msg_interactive: .asciz "\n=== USER MODE ===\n"

.text
.globl main

# Include macro library for simplified function calls
.include "macrolib.asm"

# Main Program: main
# Program entry point - executes automated tests then enters interactive mode
# Flow: Tests → Interactive file processing → Array transformation → Output
# Requirements: 10 points level (file I/O, macros, modular design)
main:
    # Execute automated test suite using macro wrapper
    RUN_TESTS
    
    # Display mode transition message
    PRINT_STR(msg_interactive)

# Interactive Mode: interactive_mode
# Handles user interaction: file input, processing, and result display
# Features: File path input, error handling, array processing, formatted output
interactive_mode:
    # Prompt user for file path input
    PRINT_STR(prompt)
    # Clear input buffer before receiving new input
    # Prevents contamination from previous inputs
    la t0, buffer          # Load buffer start address
    li t1, 256             # Set buffer size counter
    
clear_buffer:
    beqz t1, buffer_ready  # Exit loop when buffer fully cleared
    sb zero, 0(t0)         # Store null terminator at current position
    addi t0, t0, 1         # Move to next buffer position
    addi t1, t1, -1        # Decrement remaining bytes counter
    j clear_buffer         # Continue clearing buffer
    
buffer_ready:
    # Receive file path input from user
    INPUT_STR(buffer, 256)
    
    # Remove newline character from input using macro wrapper
    REMOVE_NEWLINE(buffer)
    
    # Read file content (direct subroutine call - no macro available)
    la a0, buffer          # File path string pointer
    la a1, file_buffer     # Buffer for file content storage
    li a2, 256             # Maximum bytes to read
    jal read_file          # Call file reading subroutine
    bltz a0, file_error    # Jump to error handling if read failed
    
    # Parse array data from file content (direct subroutine call)
    la a0, file_buffer     # Pointer to file data
    la a1, array_A         # Destination array for parsed data
    li a2, 10              # Maximum allowed array size
    jal parse_file_data    # Call data parsing subroutine
    sw a0, size_A, t0      # Store parsed array size in memory
    
    # Validate array size constraints (1-10 elements)
    lw t0, size_A          # Load parsed array size
    li t1, 1               # Minimum allowed size
    blt t0, t1, size_error # Error if size < 1
    li t1, 10              # Maximum allowed size
    bgt t0, t1, size_error # Error if size > 10
    
    j process_arrays       # Proceed to array processing if validation passed

# Error Handling: file_error
# Handles file reading errors (file not found, permission issues, etc.)
# Displays error message and returns to interactive mode for retry
file_error:
    PRINT_STR(error_file)  # Display file error message
    PRINT_STR(newline)     # Add newline for formatting
    
    # Clear file buffer after error to prevent data contamination
    la t0, file_buffer     # Load file buffer start address
    li t1, 256             # Set buffer size counter
    
clear_file_buffer:
    beqz t1, after_file_error # Exit when buffer cleared
    sb zero, 0(t0)         # Store null terminator
    addi t0, t0, 1         # Move to next position
    addi t1, t1, -1        # Decrement counter
    j clear_file_buffer    # Continue clearing buffer
    
after_file_error:
    
    j interactive_mode     # Return to interactive mode for retry


# Error Handling: size_error
# Handles array size validation errors (outside 1-10 range)
# Displays error message and returns to interactive mode for retry
size_error:
    PRINT_STR(error_size)  # Display size error message
    PRINT_STR(newline)     # Add newline for formatting
    
    # Clear file buffer after size error
    la t0, file_buffer     # Load file buffer start address
    li t1, 256             # Set buffer size counter
    
clear_after_size_error:
    beqz t1, after_size_error # Exit when buffer cleared
    sb zero, 0(t0)         # Store null terminator
    addi t0, t0, 1         # Move to next position
    addi t1, t1, -1        # Decrement counter
    j clear_after_size_error # Continue clearing buffer
    
after_size_error:
    
    j interactive_mode     # Return to interactive mode for retry

# Array Processing: process_arrays
# Core processing logic: displays input, transforms array, displays output
# Uses macro wrappers for standardized function calls
process_arrays:
    # Display array size information
    PRINT_STR(msg_n)       # "N = " prefix
    lw a0, size_A          # Load array A size
    PRINT_INT(a0)          # Print size value
    PRINT_STR(newline)     # Newline for formatting
    
    # Display original array A
    PRINT_STR(msg_A)       # "A: " prefix
    lw t0, size_A          # Load array A size
    PRINT_ARRAY(array_A, t0) # Print array A using macro wrapper
    
    # Transform array A into array B according to variant rules
    lw t0, size_A          # Load source array size
    FORM_ARRAY_B(array_A, array_B, t0) # Call array transformation macro
    sw a0, size_B, t0      # Store resulting array B size
    
    # Display transformed array B
    PRINT_STR(msg_B)       # "B: " prefix
    lw t0, size_B          # Load array B size
    PRINT_ARRAY(array_B, t0) # Print array B using macro wrapper

# Program Termination: exit
# Clean program exit using macro wrapper
exit:
    EXIT                   # Terminate program using macro
    
# Include modular program components
.include "io.asm"          # Input/Output subroutines
.include "array_processing.asm" # Array transformation logic
.include "test.asm"        # Automated testing framework