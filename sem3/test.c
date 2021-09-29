#include <limits.h>
#include <stdint.h>
//#include <inttypes.h>
//
//#define ITYPE int
extern void sum(ITYPE first, ITYPE second, ITYPE* res, int* CF)
{
    ITYPE result = 0;
    int carry = 0;
    int i = 0;
    while (first > 0 || second > 0) {
        result |= ((first & 1u) ^ (second & 1u) ^ carry) << i;
        carry = ((first & second) | (first & carry) | (carry & second)) & 1u;
        second >>= 1;
        first >>= 1;
        ++i;
    }
    result |= carry << i;
    *res = result;
    if (result < first || result < second) {
        *CF = 1;
    } else {
        *CF = 0;
    }
}

// int main() {
//    ITYPE res;
//    int CF;
//    ITYPE a, b;
//    scanf("%d%d", &a, &b);
//    sum(a, b, &res, &CF);
//    printf("%d  %d\n", res, CF);
//    return 0;
// }
