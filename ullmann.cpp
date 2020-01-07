#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include "motor.hpp"
#include "zavora.hpp"
#include "ullmann.hpp"






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
    
    // Promenne "stopek" pro generovani taktu
    clock_t last_time = clock();
    clock_t now_time;

    // nekonecny cyklus - hlavni cast programu
    while(program_run) {
        
        // taktovaci bit - generovani signalu 
        now_time = clock(); // aktualizace informace o case

        // vyhodnoceni zda ma dojit k preklopeni taktu
        if((((double) now_time - last_time) / CLOCKS_PER_SEC) * SEC_TO_MILISEC_COEFFICIENT >= perioda_taktu_ms) {
            last_time = now_time;
            output_buffer ^= 1<<BIT_TAKT;   // vytvoreni 1:1 (50%) stridy 
        }
        
        // zmena chovani programu pri vstupu z klavesnice
        int key = getch(); // stiskla klavesa
        
        // Reaguj pouze pokud je stiskla nejaka klavesa
        if(key != EOF) {
            
            // reakce pouze na pozadovane klavesy
            switch(key) {
                case 'q': {
                    program_run = false;
                    break;
                }
                default: {
                    printf("Stisknuta nic nedelajici klavesa \n");
                    continue;
                }
            } 
        }
        else {
            // Pokud neni nic stisknuto ustal robota
            
            // TODO Vypnuti motoru
        }
        
    }
    printf("Program vypnut \n");
    return 0;
}