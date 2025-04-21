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

typedef struct {
    word value;     // значение (что)
    address adr;    // адрес (куда)
} Arg;

Arg ss, dd;

void print_usage(const char *program_name) {
    printf("Использование: %s [-t] <путь_к_файлу>\n", program_name);
    printf("  -t          Включить режим трассировки\n");
    printf("  <путь_к_файлу>  Путь к файлу с данными (если не указан, данные читаются из stdin)\n");
}
 
void reg_dump(){
    int i;
    for (i = 0; i < 8; i ++)
        logger(TRACE, "r%d:%o ", i, reg[i]);
}

void do_halt()
{
    reg_dump();
    logger(TRACE, "\nTHE END!!!\n");
    exit(0);
}

void do_add() {
    w_write(dd.adr, ss.value + dd.value);
}
void do_mov() {
    w_write(dd.adr, ss.value);
}
void do_nothing() {}

Arg get_mr(word w)
{
    Arg res;
    int r = w & 7;          // номер регистра
    int m = (w >> 3) & 7;
    switch (m) {
        // мода 0, R1
        case 0:
            res.adr = r;        // адрес - номер регистра
            res.value = reg[r];   // значение - число в регистре
            logger(TRACE, "R%d ", r);
            break;
    
        // мода 1, (R1)
        case 1:
            res.adr = reg[r];           // в регистре адрес
            res.value = w_read(res.adr);  // по адресу - значение
            logger(TRACE, "(R%d) ", r);
            break;
    
        // мода 2, (R1)+ или #3
        case 2:
            res.adr = reg[r];           // в регистре адрес
            res.value = w_read(res.adr);  // по адресу - значение
            reg[r] += 2;                // TODO: +1
            // печать разной мнемоники для PC и других регистров
            if (r == 7)
                logger(TRACE, "#%o ", res.value);
            else
                logger(TRACE, "(R%d)+ ", r);
            break;

        default:
            logger(ERROR, "Mode %d not implemented yet!\n", m);
            exit(1);
    }
    return res;
}


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
        printf( "%06o %06o: ", pc, w);
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
    char *file_path = NULL;

    if (argc == 1) {
        print_usage(argv[0]);
        return 1;
    }
    file_path = argv[argc-1];
    
    for (int argi = 1; argi < argc - 1; argi++) {
    
        if (strcmp(argv[argi], "-t") == 0) {
                set_log_level(TRACE);
        } else {
            fprintf(stderr, "Unknown option %s\n", argv[argi]);
            print_usage(argv[0]);
            return 1;
        }
    }

    load_file(file_path);
    run();

    return 0;
}