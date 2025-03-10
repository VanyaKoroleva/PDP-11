#include <stdio.h>
#define N 5

int main(){
    int x, reg1, reg2, data, i;
    unsigned char registers[4] = {0};
    while(1){
        scanf("%d", &x);
        switch (x){
            case 0: // HLT
                return 0;
            case 1: // ADD
                scanf("%d%d", &reg1, &reg2);
                registers[reg1 - N] += registers[reg2 - N];
                break;
            case 2: // SUB
                scanf("%d%d", &reg1, &reg2);
                registers[reg1 - N] -= registers[reg2 - N];
                break;
            case 3: // MOV
                scanf("%d%d", &reg1, &data);
                registers[reg1 - N] = data;
                break;
            case 4: // IR
               for(i = 0; i < 4; i ++){
                    printf("%d ", registers[i]);
               }
               printf("\n");
               break;
            default:
                break;
        }
    }
}