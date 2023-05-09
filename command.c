#include "command.h"
#include "coder.h"

int encode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;
    in = fopen(in_file_name, "r");
    if (!in)
    {
        return -1;
    }
    out = fopen(out_file_name, "wb");
    if (!out)
    {
        return -1;
    }
    int count = 0;
    for (char c = getc(in); c != EOF; c = getc(in))
    {
        if (c == '\n')
            count++;
    }
    uint32_t code_point;
    CodeUnits code_unit;
    fseek(in, 0, SEEK_SET);
    for (int i = 0; i < count + 1; i++)
    {
        fscanf(in, "%" SCNx32, &code_point);
        printf("%" PRIx32, code_point);
        printf("\n");
        if (encode(code_point, &code_unit) != -1)
            write_code_unit(out, &code_unit);
        else
        {
            printf("Не удалось закодировать число\n");
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}
int decode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;
    in = fopen(in_file_name, "rb");
    if (!in)
    {
        return -1;
    }
    out = fopen(out_file_name, "w");
    if (!out)
    {
        return -1;
    }
    fseek(in, 0, SEEK_END);
    int size = ftell(in);
    fseek(in, 0, SEEK_SET);
    while (ftell(in) < size)
    {
        CodeUnits code_unit;
        if (read_next_code_unit(in, &code_unit) != -1)
            fprintf(out, "%" PRIx32, decode(&code_unit));
        else
        {
            printf("Не удалось считать закодированное число\n");
            return -1;
        }
        fprintf(out, "\n");
    }
    fclose(in);
    fclose(out);
    return 0;
}
