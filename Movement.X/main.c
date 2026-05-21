#include "xc.h"
#include "BOARD.h"
#include "peashooter.h"
#include <stdio.h>

#define DELAY(x) for(delay = 0; delay < (x); delay++) {asm("nop");}

int main(void)
{
    unsigned int delay;

    BOARD_Init();
    PS_Init();
    
    printf("Beginning motor test...\r\n");
    int num = 99;
    printf("testing at %d\n", num);
    while (1) {
        
        PS_Forward(num, 10);

        DELAY(1000000);

        PS_Backward(num, 10);

        DELAY(1000000);

        PS_TurnRight(num);
        
        DELAY(1000000);

        PS_TurnLeft(num);

        DELAY(1000000);

    }
}