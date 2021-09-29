    .text
    .global f
f:
    // x0 = A, x1 = B, x2 = C, x3 = x
    // y = A*x*x + B*x + C

    // calculate A*x*x
    mul     x0, x0, x3  // x0 = A*x
    mul     x0, x0, x3  // x0 = A*x*x
    mul     x1, x1, x3  // x1 = B*x
    add     x0, x0, x1  // x0 = A*x*x + B*x
    add     x0, x0, x2  // x0 = A*x*x + B*x + C

    // return from function (ret address stored in lr)
    ret

