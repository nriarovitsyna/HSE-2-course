# arccos.asm
# The module for calculating the arccosine using the arcsin by the Taylor series
# Implements arccos(x) function via identity: arccos(x) = pi/2 - arcsin(x)
# Includes range validation and special case handling for x = +/-1.0

# Constants
.data
    epsilon:        .float 0.000001     
    zero_float:     .float 0.0          
    one_float:      .float 1.0          
    pi:             .float 3.14159265   
    pi_half:        .float 1.57079633   

.text

# Range validation function
# Parameter: fa0 = x value to check
# Return: a0 = 1 if valid (|x| <= 1), 0 otherwise
check_range:
    addi sp, sp, -8
    sw ra, 4(sp)
    
    fabs.s ft0, fa0           # ft0 = |x|
    la t0, one_float
    flw ft1, 0(t0)            # ft1 = 1.0
    fle.s a0, ft0, ft1        # a0 = (|x| <= 1.0) ? 1 : 0
    
    lw ra, 4(sp)
    addi sp, sp, 8
    ret

# Main arccos calculation function
# Parameter: fa0 = x value in range [-1, 1]
# Return: fa0 = arccos(x) in radians
calc_arccos:
    addi sp, sp, -24
    sw ra, 20(sp)
    sw s0, 16(sp)             # Store sign flag
    fsw fs0, 12(sp)           # Store |x|
    fsw fs1, 8(sp)            # Store intermediate result
    
    # Check x = 1.0 special case
    la t0, one_float
    flw ft1, 0(t0)
    feq.s t1, fa0, ft1        # t1 = (x == 1.0)
    bnez t1, one_case         # If x == 1.0, jump to one_case
    
    # Check x = -1.0 special case
    fneg.s ft1, ft1           # ft1 = -1.0
    feq.s t1, fa0, ft1        # t1 = (x == -1.0)
    bnez t1, minus_one_case   # If x == -1.0, jump to minus_one_case
    
    # Determine sign and jump to appropriate case
    la t0, zero_float
    flw ft0, 0(t0)
    flt.s t1, fa0, ft0        # t1 = 1 if x < 0 (negative)
    bnez t1, negative_case    # If negative -> negative_case
    j positive_case           # If positive -> positive_case

one_case:
    # Handle x = 1.0
    la t0, zero_float
    flw fa0, 0(t0)            # Return 0 for x = 1.0
    j calc_arccos_end

minus_one_case:
    # Handle x = -1.0
    la t0, pi
    flw fa0, 0(t0)            # Return pi for x = -1.0
    j calc_arccos_end

positive_case:
    # For positive values: arccos(x) = pi/2 - arcsin(x)
    fmv.s fs0, fa0            # fs0 = x (already positive)
    
    # Calculate arcsin(x)
    # Parameter passing: fs0 (x value) -> fa0 for calc_arcsin
    # Actual parameter: x value stored in fs0
    fmv.s fa0, fs0            # Move x to fa0 as parameter for calc_arcsin
    jal calc_arcsin           # Function call: calc_arcsin(x)
    # Return value transfer: fa0 (arcsin result) -> fs1
    fmv.s fs1, fa0            # Store arcsin(x) result in fs1
    
    # Calculate arccos(x) = pi/2 - arcsin(x)
    la t0, pi_half
    flw ft0, 0(t0)
    fsub.s fa0, ft0, fs1      # fa0 = pi/2 - arcsin(x)
    j calc_arccos_end

negative_case:
    # For negative values: arccos(x) = pi - arccos(|x|)
    fabs.s fs0, fa0           # fs0 = |x|
    
    # Calculate arccos(|x|) = pi/2 - arcsin(|x|)
    # Parameter passing: fs0 (|x| value) -> fa0 for calc_arcsin
    # Actual parameter: absolute value of x stored in fs0
    fmv.s fa0, fs0            # Move |x| to fa0 as parameter for calc_arcsin
    jal calc_arcsin           # Function call: calc_arcsin(|x|)
    # Return value transfer: fa0 (arcsin result) -> fs1
    fmv.s fs1, fa0            # Store arcsin(|x|) result in fs1
    
    # Calculate arccos(|x|) = pi/2 - arcsin(|x|)
    la t0, pi_half
    flw ft0, 0(t0)
    fsub.s fs1, ft0, fs1      # fs1 = arccos(|x|) = pi/2 - arcsin(|x|)
    
    # Calculate arccos(x) = pi - arccos(|x|)
    la t0, pi
    flw ft1, 0(t0)
    fsub.s fa0, ft1, fs1      # fa0 = pi - arccos(|x|)
    j calc_arccos_end
    
calc_arccos_end:
    flw fs1, 8(sp)
    flw fs0, 12(sp)
    lw s0, 16(sp)
    lw ra, 20(sp)
    addi sp, sp, 24
    ret

# Taylor series calculation for arcsin(x)
# Parameter: fa0 = x value (0 <= x <= 1)
# Return: fa0 = arcsin(x) calculated via Taylor series
calc_arcsin:
    addi sp, sp, -36
    sw ra, 32(sp)
    sw s0, 28(sp)             # Counter n
    fsw fs0, 24(sp)           # x
    fsw fs1, 20(sp)           # Current sum (arcsin(x))
    fsw fs2, 16(sp)           # Current term value
    fsw fs4, 12(sp)           # Coefficient product
    fsw fs5, 8(sp)            # x^(2n+1)
    
    # Parameter storage: fa0 (input x) -> fs0
    # Actual parameter: x value passed in fa0
    fmv.s fs0, fa0            # Store input parameter x in fs0
    
    # Initialize variables
    li s0, 1                  # n counter starting from 1
    la t0, one_float
    flw fs4, 0(t0)            # fs4 = 1.0 (initial coefficient product)
    la t0, epsilon
    flw ft10, 0(t0)           # ft10 = epsilon for convergence check
    
    # First term of series: x
    fmv.s fs1, fs0            # sum = x
    fmv.s fs5, fs0            # x^(2n+1) = x
    fmv.s fs2, fs0            # current term = x

calc_arcsin_loop:
    # Update power: multiply by x^2
    fmul.s fs5, fs5, fs0
    fmul.s fs5, fs5, fs0      # fs5 = x^(2n+1)
    
    # Calculate coefficient: (2n-1)/(2n)
    li t2, 2
    mul t3, s0, t2            # t3 = 2n
    addi t4, t3, -1           # t4 = 2n-1
    
    fcvt.s.w ft1, t3          # ft1 = 2n (float)
    fcvt.s.w ft2, t4          # ft2 = 2n-1 (float)
    fdiv.s ft3, ft2, ft1      # ft3 = (2n-1)/(2n)
    
    # Update coefficient product
    fmul.s fs4, fs4, ft3      # fs4 *= (2n-1)/(2n)
    
    # Calculate denominator: 2n+1
    addi t5, t3, 1            # t5 = 2n+1
    fcvt.s.w ft4, t5          # ft4 = 2n+1 (float)
    
    # Calculate current term: product * x^(2n+1) / (2n+1)
    fdiv.s ft5, fs5, ft4      # ft5 = x^(2n+1) / (2n+1)
    fmul.s ft6, fs4, ft5      # ft6 = coefficient * term
    
    # Add to sum
    fadd.s fs1, fs1, ft6      # fs1 += current term
    
    # Check convergence condition
    fabs.s ft7, ft6           # ft7 = |current term|
    flt.s t6, ft7, ft10       # t6 = (|term| < epsilon)
    bnez t6, calc_arcsin_end  # Exit if term is negligible
    
    # Increment counter and check maximum iterations
    addi s0, s0, 1
    li t0, 1000
    blt s0, t0, calc_arcsin_loop   # Continue if under iteration limit

calc_arcsin_end:
    # Return pure arcsin(x)
    # Return value: arcsin stored in fs1
    fmv.s fa0, fs1            # Move result to fa0 for return
    
    flw fs5, 8(sp)
    flw fs4, 12(sp)
    flw fs2, 16(sp)
    flw fs1, 20(sp)
    flw fs0, 24(sp)
    lw s0, 28(sp)
    lw ra, 32(sp)
    addi sp, sp, 36
    ret