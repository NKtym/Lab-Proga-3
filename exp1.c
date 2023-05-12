#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t encode_varint(uint32_t value, uint8_t* buf)
{
    assert(buf != NULL);
    uint8_t* cur = buf;
    while (value >= 0x80)/*128=1000 0000*/ {
        const uint8_t byte = (value & 0x7f) | 0x80; /*127=0111 1111*/
        *cur = byte;
        value >>= 7;
        ++cur;
    }
    *cur = value;
    ++cur;
    return cur - buf;//сколько занимает число в байтах
}

uint32_t generate_number()
{
    const int r = rand(); //max 2147483647, на каждый байт один служебный бит в varint(число может стать больше при сжатии)
    const int p = r % 100;
    if (p < 90)
    {
        return r % 128;
    }
    if (p < 95)
    {
        return r % 16384;
    }
    if (p < 99)
    {
        return r % 2097152;
    }
    return r % 268435455;
}

int main(){
    FILE *uncompressed;
    uncompressed = fopen("uncompressed.dat", "wb");
    FILE *compressed;
    compressed = fopen("compressed.dat", "wb");
    uint32_t *values;
    values = malloc(1000000 * sizeof(uint32_t));
    for (int i = 0; i < 1000000; i++)
    {
        values[i] = generate_number();
    }
    fwrite(values, sizeof(uint32_t), 1000000, uncompressed);
    uint8_t buf[4] = {};
    uint8_t *cur = malloc(1000000 * 4);
    uint8_t *start = cur;
    size_t size;
    for (int i = 0; i < 1000000; i++)
    {
        size = encode_varint(values[i], buf);
        for (int j = 0; j < size; j++)
        {
            *cur = buf[j];
            cur++;
        }
    }
    fwrite(start, sizeof(uint8_t), cur - start, compressed);
    fclose(compressed);
    fclose(uncompressed);
}
