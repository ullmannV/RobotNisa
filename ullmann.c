#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include "ullmann.h"

// pomocne konstanty
const unsigned char VSE_VYPNUTO = 0xFF; // periferie jsou aktivni v logicke nule
const unsigned short SEC_TO_MILISEC_COEFFICIENT = 1000; // 1 s = 1000 ms

// pole klaves k ovladani
const char control_keys[] = 
{
    'q', // Vypnuti programu
    'a', // Otoceni zakladny proti smeru hodinovych rucicek
    'd', // Otoceni zakladny po smeru hodinovych rucicek
    'w', // Zvednuti hlavniho ramene
    's', // Snizeni hlavniho ramene
    'r', // Zvednuti ramene celisti
    'f', // Snizeni ramene celisti
    'x', // Zavreni celisti
    'c'  // Otevreni celisti
};

// Zapojeni
/*
    Robot ma vstupy i vystupy, nicmene jej zapojime tak, aby
    si vahy bitu odpovidaly s danym vstupem i vystupem
    Priklad: 
    BIT_ZAKLADNA = 0 -> motor zakladny je zapojen na VYSTUPNIM portu na bitu 0
                     -> zavora zakladny je zapojena na VSTUPNIM portu na bitu 0 
*/
const unsigned char BIT_ZAKLDNA = 0;
const unsigned char BIT_HLAVNI_RAMENO = 1;
const unsigned char BIT_RAMENO_CELISTI = 2;
const unsigned char BIT_CELIST = 3;
const unsigned char BIT_TAKT = 4;
const unsigned char BIT_SMER = 5;

// konstrukcni konstanty
const unsigned short PORT_OUT = 0x300; // Port P1
const unsigned short PORT_IN = 0x300;  // Port P3
const unsigned short MAX_RYCHLOST_ROBOTA = 450; // Hz

// volitelne parametry:
const unsigned short RYCHLOST_ROBOTA = 100; // Hz

int main(void) {
    // priprava programu
    
    // reset po predchozim behu programu
    clrscr(); // vycisti plochu

    unsigned char output_buffer = VSE_VYPNUTO; 

    outportb(port_vystupy, output_buffer); // motory vypnuty

    // podminka behu programu
    bool program_run = true;

    // prevedeni rychlosti v Hz na ms
    const unsigned int perioda_taktu_ms = taktovaci_frekvence_robota < MAX_RYCHLOST_ROBOTA ? SEC_TO_MILISEC_COEFFICIENT/taktovaci_frekvence_robota : SEC_TO_MILISEC_COEFFICIENT/MAX_RYCHLOST_ROBOTA; 
    
    // nekonecny cyklus - hlavni cast programu
    do {
       
        // kontrola zda byla stisknuta klavesa
        if(kbhit()) {
            int pressed_key = getch(); // vyzvednuti z bufferu

            // reakce na dane stiskle klavesy
            switch(pressed_key) {
                case control_keys[0]:       // Vypnuti programu
                    program_run = false;
                    break;      
                case control_keys[1]:       // Otoceni zakladny proti smeru hodinovych rucicek
                    break;
                case control_keys[2]:       // Otoceni zakladny po smeru hodinovych rucicek
                    break;
                case control_keys[3]:       // Zvednuti hlavniho ramene
                    break;
                case control_keys[4]:       // Snizeni hlavniho ramene
                    break;
                case control_keys[5]:       // Zvednuti ramene celisti
                    break;
                case control_keys[6]:       // Snizeni ramene celisti
                    break;      
                case control_keys[7]:       // Zavreni celisti
                    break;
                case control_keys[8]:       // Otevreni celisti
                    break;
                default: 
                    printf("Stisknuta neznama klavesa.\n");
                    continue;
            }
        } /*End of Switch*/
        
        output_buffer ^= 1<<BIT_TAKT; // Toggle taktovaciho bitu 

        outportb(port_vystupy, output_buffer); // odeslani zpracovanych dat na vystupni piny portu
        delay(perioda_taktu_ms); // Zpozdeni pro generovani taktu

    } while(program_run); /*End of Main Infinite Loop*/ 
    
    printf("Program vypnut \n");
    return 0;
}