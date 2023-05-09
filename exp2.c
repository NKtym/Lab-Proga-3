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

uint32_t decode_varint(const uint8_t** bufp)
{
    const uint8_t* cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    size_t shift = 7;
    while (byte >= 0x80) {
        byte = *cur++;
        value += (byte & 0x7f) << shift;
        shift += 7;
    }
    *bufp = cur;
    return value;
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
    uncompressed = fopen("uncompressed.dat", "rb");
    FILE *compressed;
    compressed = fopen("compressed.dat", "rb");
    fseek(compressed, 0, SEEK_END);
    int size = ftell(compressed);
    fseek(compressed, 0, SEEK_SET);
    uint32_t uncomp[1000000];
    uint8_t *comp = malloc(size);
    fread(comp, 1, size, compressed);
    fread(uncomp, 4, 1000000, uncompressed);
    const uint8_t *cur_comp = comp;
    int count = 0;
    for (int i = 0; i < 1000000; i++)
    {
//	printf("%x\n",decode_varint(&cur_comp));
        if (decode_varint(&cur_comp) == uncomp[i])
            count++;
    }
    if (count == 1000000)
        return 0;
    else
        return -1;
    fclose(compressed);
    fclose(uncompressed);
}
