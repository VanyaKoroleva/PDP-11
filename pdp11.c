#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "memory.h"
#include "logger.h"

#define pc reg[7]

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
    char params;
} Command;

typedef struct {
    word value;     // значение (что)
    address adr;    // адрес (куда)
} Arg;

Arg ss, dd;

enum command_params {
    NO_PARAMS = 0,
    HAS_DD    = 1,
    HAS_SS    = 2,
    HAS_R     = 4,
    HAS_NN    = 8
};

void print_usage(const char *program_name) {
    printf("Использование: %s [-t] <путь_к_файлу>\n", program_name);
    printf("  -t          Включить режим трассировки\n");
    printf("  <путь_к_файлу>  Путь к файлу с данными (если не указан, данные читаются из stdin)\n");
}
 
void reg_dump(){
    logger(TRACE, "\n");
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

Command command[] = {
    {0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
    {0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0000000, 0000000, "unknown", do_nothing, NO_PARAMS} // LAST
};

word read_cmd() {
    word w;
    logger(TRACE, "%06o ", pc);
    w = w_read(pc);
    pc += 2;
    logger(TRACE, "%06o : ", w);
    return w;
}

int r, nn;

Command parse_cmd(word cmd) {
    for (int i = 0; ; i++) {
        if ((cmd & command[i].mask) == command[i].opcode) {
            logger(TRACE, "%s ", command[i].name);
            if (command[i].params & HAS_SS) {
                ss = get_mr(cmd >> 6);
            } 
            if (command[i].params & HAS_DD) {
                dd = get_mr(cmd);
            }
            if (command[i].params & HAS_R) {
                r = (cmd >> 6) & 7;
                logger(TRACE, "R%d ", r);
            } 
            if (command[i].params & HAS_NN) {
                nn = cmd & 077;
                logger(TRACE, "#%o ", pc - (nn << 1));
            }
            return command[i];
        }
    }
}

void run()
{
    pc = 01000;
    Command cmd;
    while(1) {
        cmd = parse_cmd(read_cmd()); // читаем слово и разбираем команду
        cmd.do_command();            // выполняем команду
        logger(TRACE, "\n");
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