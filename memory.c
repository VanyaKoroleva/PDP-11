#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

byte mem[MEMSIZE];

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

