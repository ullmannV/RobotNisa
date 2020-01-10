#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include "ullmann.h"






int main(void) {
    // priprava programu
    
    // reset po predchozim behu programu
    clrscr(); // vycisti plochu

    unsigned char output_buffer = VSE_VYPNUTO;

    outportb(port_vystupy, output_buffer);

    // podminka behu programu
    bool program_run = true;

    // prevedeni rychlosti v Hz na ms
    const double perioda_taktu_ms = taktovaci_frekvence_robota < MAX_RYCHLOST_ROBOTA ? SEC_TO_MILISEC_COEFFICIENT/taktovaci_frekvence_robota : SEC_TO_MILISEC_COEFFICIENT/MAX_RYCHLOST_ROBOTA; 
    
    // nekonecny cyklus - hlavni cast programu
    do {
       
        if(kbhit()) {
            int pressed_key = getch();
        }
        
        outportb(port_vystupy, output_buffer); // odeslani zpracovanych dat na vystupni piny portu

    } while(program_run);
    
    printf("Program vypnut \n");
    return 0;
}