#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "memory.h"
#include "logger.h"

word reg[8];
#define pc reg[7]

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
} Command;

void print_usage(const char *program_name) {
    printf("Использование: %s [-t] <путь_к_файлу>\n", program_name);
    printf("  -t          Включить режим трассировки\n");
    printf("  <путь_к_файлу>  Путь к файлу с данными (если не указан, данные читаются из stdin)\n");
}
 
void do_halt()
{
    printf("THE END!!!\n");
    exit(0);
}

void do_add() {}
void do_mov() {}
void do_nothing() {}

void run()
{
    pc = 01000;
    unsigned int i;
    word w; 
    Command command[] = {
        {0170000, 0060000, "add", do_add},
        {0170000, 0010000, "mov", do_mov},
        {0177777, 0000000, "halt", do_halt},
        {0000000, 0000000, "unknown", do_nothing}
    };

    while(1) {
        w = w_read(pc);
        printf("%06o %06o: ", pc, w);
        pc += 2;
        for(i = 0; ; i ++){
            if ((w & command[i].mask) == command[i].opcode) {
                printf("%s \n", command[i].name);
                command[i].do_command();
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *file = stdin;
    char *file_path = NULL;

    if (argc > 1) {
        if (argc == 3 && strcmp(argv[1], "-t") == 0) {
            set_log_level(TRACE);
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
    run();
    fclose(file);

    return 0;
}