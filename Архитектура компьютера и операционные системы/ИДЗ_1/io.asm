# Яровицына Наталья БПИ244
# io.asm - Input/Output Module

.text

# Subroutine: remove_newline
# Removes newline character from null-terminated string by replacing it with null terminator
# Parameters: a0 - pointer to input string (modified in place)
# Uses: t0 (string pointer), t1 (current character), t2 (newline constant)
remove_newline:
    mv t0, a0              # Copy string pointer to temporary register for iteration
scan_string_loop:
    lb t1, (t0)            # Load current character from string
    beqz t1, string_cleaned # If null terminator found, exit loop (end of string)
    li t2, '\n'            # Load newline character constant for comparison
    beq t1, t2, found_newline # If current character is newline, process replacement
    addi t0, t0, 1         # Move to next character in string
    j scan_string_loop     # Continue scanning through string
    
found_newline:
    sb zero, (t0)          # Replace newline character with null terminator
    
string_cleaned:
    ret                    # Return to caller (string modified in place)

# Subroutine: read_file
# Reads file content into memory buffer using RARS system calls
# Parameters: 
#   a0 - file path string (null-terminated)
#   a1 - buffer address for storing file data
#   a2 - buffer size in bytes (maximum bytes to read)
# Returns: a0 - number of bytes read (positive) or -1 on error
# Uses: s0 (file descriptor), s1 (buffer pointer preservation)
# Stack: Allocates 16 bytes, saves ra, s0, s1
read_file:
    addi sp, sp, -16       # Allocate stack frame (4 words)
    sw ra, 12(sp)          # Save return address on stack
    sw s0, 8(sp)           # Save s0 register (file descriptor storage)
    sw s1, 4(sp)           # Save s1 register (buffer pointer preservation)
    
    mv s1, a1              # Preserve buffer pointer in s1 across system calls

    # Open file for reading (RARS system call 1024)
    li a7, 1024            # System call code: open file
    li a1, 0               # File open flags: 0 = read-only mode
    ecall                  # Execute system call, returns file descriptor in a0
    mv s0, a0              # Save file descriptor in s0 for later operations
    bltz a0, read_error    # If file descriptor < 0, jump to error handling

    # Read file content into buffer (RARS system call 63)
    li a7, 63              # System call code: read from file
    mv a0, s0              # File descriptor from open operation
    mv a1, s1              # Buffer address for storing read data
    # Note: a2 already contains buffer size from caller parameters
    ecall                  # Execute system call, returns bytes read in a0
    mv t0, a0              # Temporarily store bytes read count in t0
    
    # Close file (RARS system call 57)
    li a7, 57              # System call code: close file
    mv a0, s0              # File descriptor to close
    ecall                  # Execute system call
    
    mv a0, t0              # Return bytes read count to caller
    j file_read_complete   # Jump to cleanup section

read_error:
    li a0, -1              # Return -1 indicating file operation error
    
file_read_complete:
    lw s1, 4(sp)           # Restore s1 register from stack
    lw s0, 8(sp)           # Restore s0 register from stack
    lw ra, 12(sp)          # Restore return address from stack
    addi sp, sp, 16        # Deallocate stack frame
    ret                    # Return to caller with result in a0

# Subroutine: parse_file_data
# Parses array data from file content (size followed by elements)
# File format: <size> <element1> <element2> ... <elementN>
# Parameters:
#   a0 - pointer to file data string (null-terminated)
#   a1 - pointer to destination integer array
#   a2 - maximum allowed array size (for bounds checking)
# Returns: a0 - actual array size (positive) or -1 on size validation error
# Uses: s0 (data pointer), s1 (array pointer), s2 (max size), s3 (actual size)
# Stack: Allocates 20 bytes, saves ra, s0-s3
parse_file_data:
    addi sp, sp, -20       # Allocate stack frame (5 words)
    sw ra, 16(sp)          # Save return address
    sw s0, 12(sp)          # Save s0 (file data pointer)
    sw s1, 8(sp)           # Save s1 (destination array pointer)
    sw s2, 4(sp)           # Save s2 (maximum size constraint)
    sw s3, 0(sp)           # Save s3 (actual parsed array size)
    
    mv s0, a0              # Store file data pointer in s0
    mv s1, a1              # Store destination array pointer in s1
    mv s2, a2              # Store maximum allowed size in s2

    # Parse array size (first integer in file data)
    mv a0, s0              # Pass current data pointer to parse_int
    jal parse_int           # Call integer parsing subroutine
    mv s3, a0              # Store parsed array size in s3
    mv s0, a1              # Update data pointer to position after size value

    # Validate array size against constraints
    bgt s3, s2, size_error_return # Error if parsed size > maximum allowed
    blez s3, size_error_return    # Error if parsed size <= 0 (invalid)
    
    j parse_elements       # Proceed to parse array elements if size valid

size_error_return:
    li a0, -1              # Return -1 indicating size validation error
    j parse_done           # Jump to cleanup and return

parse_elements:
    li t0, 0               # Initialize element counter to 0
    
process_elements_loop:
    bge t0, s3, parse_done # Exit loop when all expected elements processed
    
# Skip whitespace characters before each element
skip_spaces_loop:
    lb t1, 0(s0)           # Load current character from data
    beqz t1, parse_done    # Exit if null terminator (end of data)
    li t2, ' '             # Check for space character
    beq t1, t2, skip_space
    li t2, '\n'            # Check for newline character
    beq t1, t2, skip_space
    li t2, '\r'            # Check for carriage return
    beq t1, t2, skip_space
    li t2, '\t'            # Check for tab character
    beq t1, t2, skip_space
    j parse_next_element   # Non-whitespace found, begin element parsing
    
skip_space:
    addi s0, s0, 1         # Move data pointer past whitespace character
    j skip_spaces_loop     # Continue checking for more whitespace

parse_next_element:
    mv a0, s0              # Pass current data position to parse_int
    jal parse_int           # Parse next integer element from data
    sw a0, 0(s1)           # Store parsed integer in destination array
    mv s0, a1              # Update data pointer to position after parsed element
    
    addi s1, s1, 4         # Advance array pointer to next element position
    addi t0, t0, 1         # Increment element counter
    j process_elements_loop # Continue parsing next element

parse_done:
    mv a0, s3              # Return actual array size to caller
    lw s3, 0(sp)           # Restore s3 register
    lw s2, 4(sp)           # Restore s2 register
    lw s1, 8(sp)           # Restore s1 register
    lw s0, 12(sp)          # Restore s0 register
    lw ra, 16(sp)          # Restore return address
    addi sp, sp, 20        # Deallocate stack frame
    ret                    # Return to caller

# Subroutine: parse_int
# Parses integer from string with sign support and digit processing
# Handles: optional sign, decimal digits, leading/trailing whitespace
# Parameters: a0 - pointer to string containing integer representation
# Returns: a0 - parsed integer value, a1 - pointer to character after parsed number
# Uses: t0 (string pointer), t1 (current char), t2 (value accumulator), 
#       t3 (radix multiplier), t4 (negative flag)
# Stack: Allocates 20 bytes, saves ra, t0-t3
parse_int:
    addi sp, sp, -20       # Allocate stack frame (5 words)
    sw ra, 16(sp)          # Save return address
    sw t0, 12(sp)          # Save t0 (string pointer)
    sw t1, 8(sp)           # Save t1 (current character)
    sw t2, 4(sp)           # Save t2 (value accumulator)
    sw t3, 0(sp)           # Save t3 (radix multiplier)
    
    mv t0, a0              # Store string pointer in t0 for iteration
    li t2, 0               # Initialize value accumulator to 0
    li t3, 10              # Set radix multiplier to 10 (decimal)
    li t4, 0               # Initialize negative flag (0 = positive)
    
# Skip leading whitespace characters
skip_leading_spaces:
    lb t1, (t0)            # Load current character from string
    beqz t1, parse_int_done # Exit if null terminator (end of string)
    li a0, ' '             # Check for space character
    beq t1, a0, move_past_char
    li a0, '\n'            # Check for newline
    beq t1, a0, move_past_char
    li a0, '\r'            # Check for carriage return
    beq t1, a0, move_past_char
    li a0, '\t'            # Check for tab character
    beq t1, a0, move_past_char
    j check_sign_parse     # Non-whitespace found, check for sign
    
move_past_char:
    addi t0, t0, 1         # Advance pointer past whitespace character
    j skip_leading_spaces  # Continue checking for leading whitespace
    
check_sign_parse:
    # Check for negative sign indicator
    lb t1, (t0)            # Load current character (after whitespace)
    li a0, '-'             # Check for minus sign character
    bne t1, a0, parse_digits # If not '-', begin digit parsing
    li t4, 1               # Set negative flag to 1 (true)
    addi t0, t0, 1         # Advance pointer past '-' character
    
parse_digits:
    lb t1, (t0)            # Load current character for digit processing
    beqz t1, end_parse_int # Exit if null terminator (end of string)
    
    # Validate character is ASCII digit ('0'-'9')
    li a0, '0'
    blt t1, a0, end_parse_int # Exit if character < '0' (non-digit)
    li a0, '9'
    bgt t1, a0, end_parse_int # Exit if character > '9' (non-digit)
    
    # Convert ASCII digit to numerical value (0-9)
    addi t1, t1, -48       # Subtract ASCII '0' to get numerical value
    
    # Accumulate value: current_value * 10 + new_digit
    mul t2, t2, t3         # Multiply current value by radix (10)
    add t2, t2, t1         # Add new digit to accumulated value
    
    addi t0, t0, 1         # Advance to next character in string
    j parse_digits         # Continue processing digits

end_parse_int:
    # Apply negative sign if flag was set
    beqz t4, positive_int  # Skip negation if positive number
    neg t2, t2             # Negate the accumulated value (two's complement)
    
positive_int:
    mv a0, t2              # Return parsed integer value in a0

parse_int_done:
    mv a1, t0              # Return updated string pointer in a1
    lw t3, 0(sp)           # Restore t3 register
    lw t2, 4(sp)           # Restore t2 register
    lw t1, 8(sp)           # Restore t1 register
    lw t0, 12(sp)          # Restore t0 register
    lw ra, 16(sp)          # Restore return address
    addi sp, sp, 20        # Deallocate stack frame
    ret                    # Return to caller with parsed integer

# Subroutine: print_array
# Prints integer array to console with formatted output (space-separated)
# Parameters: a0 - array pointer, a1 - array size (number of elements)
# Uses: s0 (array pointer), s1 (array size), s2 (element counter)
# Output: Elements printed as "elem1 elem2 ... elemN\n"
# Stack: Allocates 16 bytes, saves ra, s0-s2
print_array:
    addi sp, sp, -16       # Allocate stack frame (4 words)
    sw ra, 12(sp)          # Save return address
    sw s0, 8(sp)           # Save s0 (array pointer)
    sw s1, 4(sp)           # Save s1 (array size)
    sw s2, 0(sp)           # Save s2 (element counter)
    
    mv s0, a0              # Store array pointer in s0
    mv s1, a1              # Store array size in s1
    li s2, 0               # Initialize element counter to 0
    
    beqz s1, print_done    # Skip printing if array is empty (size = 0)

print_loop:
    bge s2, s1, print_done # Exit loop when all elements printed
    
    # Print current array element as integer
    lw a0, 0(s0)           # Load current element from array
    li a7, 1               # System call code: print integer
    ecall                  # Execute system call (prints element)
    
    # Print space separator (except after last element)
    addi t0, s2, 1         # Calculate next element index (current + 1)
    bge t0, s1, continue_printing # Skip space if this is last element
    
    la a0, space           # Load space separator string
    li a7, 4               # System call code: print string
    ecall                  # Execute system call (prints space)
    
continue_printing:
    addi s0, s0, 4         # Advance array pointer to next element
    addi s2, s2, 1         # Increment element counter
    j print_loop           # Continue printing next element

print_done:
    # Print newline after all elements for proper formatting
    la a0, newline         # Load newline string
    li a7, 4               # System call code: print string
    ecall                  # Execute system call (prints newline)
    
    lw s2, 0(sp)           # Restore s2 register
    lw s1, 4(sp)           # Restore s1 register
    lw s0, 8(sp)           # Restore s0 register
    lw ra, 12(sp)          # Restore return address
    addi sp, sp, 16        # Deallocate stack frame
    ret                    # Return to caller