#include "coder.h"
#include "command.h"
#include <inttypes.h>
#include <stdio.h>

int encode(uint32_t code_point, CodeUnits *code_units)
{
    if (code_point < 0x80)//занимает 7
    {
        code_units->length = 1;
        code_units->code[0] = code_point;
    }

    else if (code_point < 0x800)//11 знач разрядов
    {
        code_units->length = 2;
        code_units->code[0] = 0xc0 | (code_point >> 6);
        code_units->code[1] = 0x80 | (code_point & 0x3f);//отбрасывание всего до 6 бит + 100000
    }
    else if (code_point < 0x10000)//16
    {
        code_units->length = 3;
        code_units->code[0] = 0xe0 | (code_point >> 12);
        code_units->code[1] = 0x80 | ((code_point >> 6) & 0x3f);
        code_units->code[2] = 0x80 | (code_point & 0x3f);
    }
    else if (code_point < 0x200000)//21
    {
        code_units->length = 4;
        code_units->code[0] = 0xf0 | (code_point >> 18);
        code_units->code[1] = 0x80 | ((code_point >> 12) & 0x3f);
        code_units->code[2] = 0x80 | ((code_point >> 6) & 0x3f);
        code_units->code[3] = 0x80 | (code_point & 0x3f);
    }
    else
        return -1;
    return 0;
}

uint32_t decode(const CodeUnits *code_unit)
{
    uint32_t value = 0;
    if (code_unit->code[0] < 0x80)//первый байт массива код, если не соответсвует маске то ошибка
    {
        value = code_unit->code[0];
    }
    else if (code_unit->code[0] < 0xE0)
    {
        value = ((code_unit->code[0] & 0x1f) << 6) | (code_unit->code[1] & 0x3f);
    }
    else if (code_unit->code[0] < 0xF0)
    {
        value = ((code_unit->code[0] & 0x0f) << 12) | ((code_unit->code[1] & 0x3f) << 6) | (code_unit->code[2] & 0x3f);
    }
    else if (code_unit->code[0] < 0xF8)
    {
        value = ((code_unit->code[0] & 0x07) << 18) | ((code_unit->code[1] & 0x3f) << 12) | ((code_unit->code[2] & 0x3f) << 6) | (code_unit->code[3] & 0x3f);
    }
    else
        return -1;
    return value;
}

int read_next_code_unit(FILE *in, CodeUnits *code_units)
{
    fread(&(code_units->code[0]), 1, 1, in);
    if (code_units->code[0] < 0x80)
    {
        code_units->length = 1;
        printf("%" PRIx8, code_units->code[0]);
        printf("\n");
        return 0;
    }
    else if (code_units->code[0] < 0xE0)
    {
        code_units->length = 2;
        fread(&(code_units->code[1]), 1, 1, in);
        if (code_units->code[1] >= 0x80 && code_units->code[1] <= 0xBF)
        {
            for (int i = 0; i < code_units->length; i++)
                printf("%" PRIx8, code_units->code[i]);
            printf("\n");
            return 0;
        }
        else
            return -1;
    }
    else if (code_units->code[0] < 0xF0)
    {
        int count = 0;
        code_units->length = 3;
        for (int i = 1; i < 3; i++)
        {
            fread(&(code_units->code[i]), 1, 1, in);
            if (code_units->code[i] >= 0x80 && code_units->code[1] <= 0xBF)
                count++;
        }
        if (count == 2)
        {
            for (int i = 0; i < code_units->length; i++)
                printf("%" PRIx8, code_units->code[i]);
            printf("\n");
            return 0;
        }
        else
            return -1;
    }
    else if (code_units->code[0] < 0xF8)
    {
        int count = 0;
        code_units->length = 4;
        for (int i = 1; i < 4; i++)
        {
            fread(&(code_units->code[i]), 1, 1, in);
            if (code_units->code[i] >= 0x80 && code_units->code[1] <= 0xBF)
                count++;
        }
        if (count == 3)
        {
            for (int i = 0; i < code_units->length; i++)
                printf("%" PRIx8, code_units->code[i]);
            printf("\n");
            return 0;
        }
        else
            return -1;
    }
    else
        return -1;
    return 0;
}

int write_code_unit(FILE *out, const CodeUnits *code_units)//пишет в файл одно закодированное число(CodeUnit(а))
{
    int result = fwrite(code_units->code, 1, code_units->length, out);//откуда,блоками по 1 байту, сколько раз, куда
    return result;
}
