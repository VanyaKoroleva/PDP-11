#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word address;
#define MEMSIZE 64*1024
byte mem[MEMSIZE];

typedef enum {
    ERROR,
    WARNING,
    INFO,
    TRACE,
    DEBUG
} LogLevel;

static LogLevel log_level = INFO;

LogLevel set_log_level(LogLevel level) {
    LogLevel old_log_level = log_level;
    log_level = level;
    return old_log_level;
}

void log(LogLevel level, const char *format, ...) {
    if (level <= log_level) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(ERROR, format, args);
    va_end(args);
}

void warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(WARNING, format, args);
    va_end(args);
}

void info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(INFO, format, args);
    va_end(args);
}

void trace(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(TRACE, format, args);
    va_end(args);
}

void debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(DEBUG, format, args);
    va_end(args);
}

void b_write (address adr, byte val){
    mem[adr] = val;
}

byte b_read (address adr){
    return mem[adr];
}

void w_write (address adr, word val){
    assert(adr % 2 == 0);
    mem[adr + 1] = (byte)(val >> 8);
    mem[adr] = (byte)val;
}

word w_read (address adr){
    assert(adr % 2 == 0);
    word w = mem[adr+1] << 8;
    w = w | mem[adr];
    return w & 0xFFFF;
} 

void test_mem()
{
    address a;
    word w, wres;
    byte b1, b0, bres;
    
    //пишем и читаем байт   
    b0 = 0x0a;
    b_write(2, b0);
    bres = b_read(2);
    printf("b0=%02x  bres=%02x\n", b0, bres);
    assert(b0 == bres);

    //два байта -> слово
    b0 = 0x0a;
    b1 = 0xcb;
    w = 0xcb0a;
    a = 4;  
    b_write(a, b0);
    b_write(a + 1, b1);
    wres = w_read(a);
    printf("wres=%04hx b1 bo=%02x %02x\n", wres, b1, b0);
    assert(w == wres);

    //пишем и читаем слово
    w = 0xcb0a;
    a = 6;
    w_write(a, w);
    wres = w_read(a);
    printf("w=%04hx    wres=%04hx\n", w, wres);
    assert(w == wres);

    //слово -> два байта
    w = 0xcb0a;
    a = 0;
    w_write(a, w);
    b0 = b_read(a);
    b1 = b_read(a + 1);
    printf("w=%04hx   b1 bo=%02x %02x\n", w, b1, b0);
}

void load_data(FILE * fin){
    unsigned int n, i, adr, val;
    while(2 == fscanf(fin, "%x%x", &adr, &n)) {
        for (i = 0; i < n; i++){
            fscanf(fin, "%x", &val);
            b_write(adr + i, val);
        }
    }
}

void mem_dump(address adr, int size){
    word w;
    for( ; size > 0; size -= 2, adr += 2){
        w = w_read(adr);
        printf("%06o: %06o %04x\n", adr, w, w);
    }
}

void load_file(const char * filename){
    FILE * fin = fopen(filename, "r");
    if (fin == NULL) {
        perror(filename);
        exit(errno);
    }
    // char str[1001];
    // fscanf(fin, "%s", str);
    // printf("%s\n", str);
    load_data(fin);
    fclose(fin);
}

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