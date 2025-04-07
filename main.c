#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "memory.h"
#include "logger.h"

void print_usage(const char *program_name) {
    printf("Использование: %s [-t] <путь_к_файлу>\n", program_name);
    printf("  -t          Включить режим трассировки\n");
    printf("  <путь_к_файлу>  Путь к файлу с данными (если не указан, данные читаются из stdin)\n");
}
 

int main(int argc, char *argv[])
{
    FILE *file = stdin;
    char *file_path = NULL;

    if (argc > 1) {
        if (argc == 3 && strcmp(argv[1], "-t") == 0) {
            file_path = argv[2];
        } 
        else if (argc == 2) {
            file_path = argv[1];
        } 
        else {
            print_usage(argv[0]);
            return 1;
        }

        if (file_path) {
            file = fopen(file_path, "r");
            if (file == NULL) {
                perror(file_path);
                exit(errno);
            }
        }
    }

    load_data(file);
    mem_dump(0x40, 20);
    printf("\n");
    mem_dump(0x200, 0x26);

    fclose(file);

    return 0;
}