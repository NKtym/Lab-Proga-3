#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
