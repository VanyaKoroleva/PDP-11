#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

byte mem[MEMSIZE];
word reg[8];

void b_write (address adr, byte val){
    if (adr < 8){
        reg[adr] = (val & 0x100) == 0x100 ? val | 0xFF : val & 0xF;
    }
    else {
        mem[adr] = val;
    }
}

byte b_read (address adr){
    return adr < 8 ? reg[adr] & 0xFF : mem[adr];
}

void w_write (address adr, word val){
    if (adr < 8) {
        reg[adr] = val;
    } else {
        mem[adr] = val & 255;
        mem[adr + 1] = val >> 8;
    }
}

word w_read (address adr){
    if (adr < 8) {
        return reg[adr];
    } else {
        word w = mem[adr + 1] << 8;
        w |= mem[adr] & 255;
        return w & 0xFFFF;
    }
} 

void mem_dump(address adr, int size){
    word w;
    for( ; size > 0; size -= 2, adr += 2){
        w = w_read(adr);
        printf("%06o: %06o %04x\n", adr, w, w);
    }
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

void load_file(const char * filename){
    FILE * fin;
    if (filename == NULL) 
        fin = stdin;
    else {
        fin = fopen(filename, "r");
        if (fin == NULL) {
            perror(filename);
            exit(errno);
        }
    }
    // char str[1001];
    // fscanf(fin, "%s", str);
    // printf("%s\n", str);
    load_data(fin);
    if (fin != stdin)
        fclose(fin);
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

