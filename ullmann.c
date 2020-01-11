#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include "booleanDOS.h"
#include "ullmann.h"

// pomocne konstanty
const unsigned char VSE_VYPNUTO = 0xFF; // periferie jsou aktivni v logicke nule
const unsigned short SEC_TO_MILISEC_COEFFICIENT = 1000; // 1 s = 1000 ms

// Zapojeni
const unsigned char BIT_ZAKLDNA;
const unsigned char BIT_HLAVNI_RAMENO;
const unsigned char BIT_RAMENO_CELISTI;
const unsigned char BIT_CELIST;
const unsigned char BIT_TAKT = 5;
const unsigned char BIT_SMER;

// konstrukcni konstanty
const unsigned short PORT_OUT = 0x300; // P1
const unsigned short PORT_IN = 0x300;  // P3
const unsigned short RYCHLOST_ROBOTA = 100; // Hz
const unsigned short MAX_RYCHLOST_ROBOTA = 450; // Hz

int main(void) {
    // priprava programu
    
    // reset po predchozim behu programu
    clrscr(); // vycisti plochu

    unsigned char output_buffer = VSE_VYPNUTO; 

    outportb(port_vystupy, output_buffer); // motory vypnuty

    // podminka behu programu
    BOOL program_run = true;

    // prevedeni rychlosti v Hz na ms
    const unsigned int perioda_taktu_ms = taktovaci_frekvence_robota < MAX_RYCHLOST_ROBOTA ? SEC_TO_MILISEC_COEFFICIENT/taktovaci_frekvence_robota : SEC_TO_MILISEC_COEFFICIENT/MAX_RYCHLOST_ROBOTA; 
    
    // nekonecny cyklus - hlavni cast programu
    do {
       
        // kontrola zda byla stisknuta klavesa
        if(kbhit()) {
            int pressed_key = getch(); // vyzvednuti z bufferu

            // reakce na dane stiskle klavesy
            switch(pressed_key) {
                case 'q':       // Vypnuti programu
                    program_run = false;
                    break;      
                case 'a':       // Otoceni zakladny proti smeru hodinovych rucicek
                    break;
                case 'd':       // Otoceni zakladny po smeru hodinovych rucicek
                    break;
                case 'w':       // Zvednuti hlavniho ramene
                    break;
                case 's':       // Snizeni hlavniho ramene
                    break;
                case 'r':       // Zvednuti ramene celisti
                    break;
                case 'f':       // Snizeni ramene celisti
                    break;      
                case 'x':       // Zavreni celisti
                    break;
                case 'c':       // Otevreni celisti
                    break;
                default: 
                    printf("Stisknuta neznama klavesa.\n");
                    break;
            }
        }
        
        output_buffer ^= 1<<BIT_TAKT; // Toggle taktovaciho bitu 

        outportb(port_vystupy, output_buffer); // odeslani zpracovanych dat na vystupni piny portu
        delay(perioda_taktu_ms); // Zpozdeni pro generovani taktu

    } while(program_run);
    
    printf("Program vypnut \n");
    return 0;
}