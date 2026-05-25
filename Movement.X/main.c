#include "xc.h"
#include "BOARD.h"
#include "peashooter.h"
#include <stdio.h>

#define TEST 1
#define TAPE_TEST 0
#define MOTOR_TEST 1

#define DELAY(x) for(delay = 0; delay < (x); delay++) {asm("nop");}

#if TEST
int main(void)
{
    BOARD_Init();
    PS_Init();

    unsigned int delay;
    
    #if MOTOR_TEST
    printf("Beginning motor test...\r\n");
    int num = 99;
    printf("testing at %d\n", num);
    while (1) {
        
        PS_Forward(num);

        DELAY(1000000);

        PS_Backward(num);

        DELAY(1000000);

    }
    #endif


    #if TAPE_TEST
    printf("Beginning tape sensor test...\r\n");
    while (1) {
        unsigned int tapeVal = PS_ReadLeftTape();
        printf("%d, %d, %d, read tape value: %d\r\n", PS_ReadLeftTape(), PS_ReadMidTape(), PS_ReadRightTape(), PS_ReadTape());
        //all=0 none=7, left =6, middle is 5, right is 3
        DELAY(1000000);
    }
    #endif
}
#endif