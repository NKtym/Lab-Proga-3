#include <inttypes.h>
#include <stdio.h>
#pragma once

enum//перечислимый тип - именованная константа(макрос менее безопасный - нельзя отследить ошибки в момент компиляции)
{
    MaxCodeLength = 4
};

typedef struct
{
    uint8_t code[MaxCodeLength];//массив 8 битных беззнаковых элементов - закодированное число
    size_t length;//размер - длинна закодированного числа
} CodeUnits;

int encode(uint32_t code_point, CodeUnits *code_units);

uint32_t decode(const CodeUnits *code_unit);

int read_next_code_unit(FILE *in, CodeUnits *code_units);

int write_code_unit(FILE *out, const CodeUnits *code_unit);
