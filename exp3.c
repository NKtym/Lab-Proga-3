#include <stdio.h>
#include "coder.h"
#include "command.h"
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 4)//относительный путь тоже идет в счёт
    {
        printf("Неверное количество аргументов\n");
        return -1;
    }
    char *command = argv[1];
    char *in_file_name = argv[2];
    char *out_file_name = argv[3];
    if (strcmp(command, "encode") == 0)
    {
        encode_file(in_file_name, out_file_name);
    }
    else if (strcmp(command, "decode") == 0)
    {
        decode_file(in_file_name, out_file_name);
    }
    else{
	printf("Неверно написана команда кодирования/декодирования\n");
        return -1;
    }
    return 0;
}
