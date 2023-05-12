#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

int main(){
    uint32_t value = 0xf17ac;
    uint8_t buf[4] = {};
    size_t size=0;
    uint8_t comressed[100]={};
    uint8_t *cur=comressed;
    size=encode_varint(value,buf);
    for (int j = 0; j < size; j++){
    	*cur = buf[j];
     	cur++;
    }
    for(int i=0; i < size; i++){
	printf("%x", buf[i]);
    }
    printf("\n");
    const uint8_t *cur_comp = comressed;
    printf("%x\n",decode_varint(&cur_comp));
    return 0;
}
