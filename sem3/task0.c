#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

// get i th bit of number
static inline uint8_t get_ith_bit(ITYPE number, uint64_t i)
{
    return (number >> i) & 1;
}

// find sum of a and b bits with carry
static inline uint8_t sum_bits(uint8_t a, uint8_t b, int carry, int* new_carry)
{
    *new_carry = (a & b) | ((a | b) & (uint8_t)carry);

    return a ^ b ^ (uint8_t)carry;
}

// sum unsigned integers
void sum(ITYPE first, ITYPE second, ITYPE* res, int* CF)
{
    uint64_t num_bits = sizeof(ITYPE) * CHAR_BIT;

    *res = 0;
    *CF = 0;
    // iterate of bits
    for (uint64_t i = 0; i < num_bits; ++i) {
        uint8_t sum =
            sum_bits(get_ith_bit(first, i), get_ith_bit(second, i), *CF, CF);

        assert(*CF == 1 || *CF == 0);

        *res |= (ITYPE)sum << i;
    }
}
