#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>

// Zapojeni
/*
    Robot ma vstupy i vystupy, nicmene jej zapojime tak, aby
    si vahy bitu odpovidaly s danym vstupem i vystupem
    Priklad: 
    BIT_ZAKLADNA = 0 -> motor zakladny je zapojen na VYSTUPNIM portu na bitu 0
                     -> zavora zakladny je zapojena na VSTUPNIM portu na bitu 0 
*/
const unsigned char BIT_ZAKLADNA = 0;
const unsigned char BIT_HLAVNI_RAMENO = 1;
const unsigned char BIT_RAMENO_CELISTI = 2;
const unsigned char BIT_CELIST = 3;
const unsigned char BIT_SMER = 4;
const unsigned char BIT_TAKT = 5;

// konstrukcni konstanty
const unsigned short PORT_OUT = 0x300; // Port P1
const unsigned short PORT_IN = 0x300;  // Port P3
const unsigned short MAX_RYCHLOST_ROBOTA = 450; // Hz

// pomocne konstanty
const unsigned char VSE_VYPNUTO = 0xFF; // periferie jsou aktivni v logicke nule
const unsigned char ZAVORY_NEAKTIVNI = 0xFF; // zavory jsou aktivni v logicke nule
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
}; // Jina klavesa -> STOP

// nastavitelne parametry:
const unsigned short RYCHLOST_ROBOTA = 450; // Hz

// podminka behu programu
bool program_run = true;

// deklarace funkci
void initPoloha(unsigned char* output);
void initRameneChapadla(unsigned char* output);
void manualControl(unsigned char* output);

// function pointer
void (*rezimProvozu)(unsigned char*);

int main(void) {
    // priprava programu
    
    // reset po predchozim behu programu
    clrscr(); // vycisti plochu
    unsigned char output_buffer = VSE_VYPNUTO; 

    outportb(PORT_OUT, output_buffer); // motory vypnuty   

    // prevedeni rychlosti v Hz na ms
    const unsigned int perioda_taktu_ms = RYCHLOST_ROBOTA < MAX_RYCHLOST_ROBOTA ? 
        0.5*(SEC_TO_MILISEC_COEFFICIENT/RYCHLOST_ROBOTA) :      // true
        0.5*(SEC_TO_MILISEC_COEFFICIENT/MAX_RYCHLOST_ROBOTA);   // false - vyšší rychlost robot neumi
    
    // uvedeni programu do pocatecniho stavu
    rezimProvozu = initPoloha;
    
    // nekonecny cyklus - hlavni cast programu
    do {
       
        // vykonani inicializacniho behu nebo rizeni z klavesnice
        rezimProvozu(&output_buffer);
        
        output_buffer ^= 1<<BIT_TAKT; // Toggle taktovaciho bitu 

        outportb(PORT_OUT, output_buffer); // odeslani zpracovanych dat na vystupni piny portu
        delay(perioda_taktu_ms); // Zpozdeni pro generovani taktu

    } while(program_run); /*End of Main Infinite Loop*/ 
    
    printf("Program vypnut \n");
    return 0;
}

void initPoloha(unsigned char* output) {
    // roztoc motory
    *output &= ~(1<<BIT_ZAKLADNA) & ~(1<<BIT_HLAVNI_RAMENO) & ~(1<<BIT_CELIST) & ~(1<<BIT_SMER);
    
    // nacti vstupy
    unsigned char input = inportb(PORT_IN);


    
}

void initRameneChapadla(unsigned char* output) {

}

void manualControl(unsigned char* output) {
        
    // kontrola zda byla stisknuta klavesa
    if(kbhit()) {
        const int pressed_key = getch(); // vyzvednuti z bufferu
            
        *output = VSE_VYPNUTO; // reset vystupu
            
        // reakce na dane stiskle klavesy
        switch(pressed_key) {
            case control_keys[0]:       // Vypnuti programu
                program_run = false;
                break;      
            case control_keys[1]:       // Otoceni zakladny proti smeru hodinovych rucicek
                *output &= ~(1<<BIT_ZAKLADNA) & ~(1<<BIT_SMER);
                break;
            case control_keys[2]:       // Otoceni zakladny po smeru hodinovych rucicek
                *output &= ~(1<<BIT_ZAKLADNA);
                break;
            case control_keys[3]:       // Zvednuti hlavniho ramene
                *output &= ~(1<<BIT_HLAVNI_RAMENO) & ~(1<<BIT_SMER);
                break;
            case control_keys[4]:       // Snizeni hlavniho ramene
                *output &= ~(1<<BIT_HLAVNI_RAMENO);
                break;
            case control_keys[5]:       // Zvednuti ramene celisti
                *output &= ~(1<<BIT_RAMENO_CELISTI);
                break;
            case control_keys[6]:       // Snizeni ramene celisti
                *output &= ~(1<<BIT_RAMENO_CELISTI) & ~(1<<BIT_SMER);
                break;      
            case control_keys[7]:       // Zavreni celisti
                *output &= ~(1<<BIT_CELIST);
                break;
            case control_keys[8]:       // Otevreni celisti
                *output &= ~(1<<BIT_CELIST) & ~(1<<BIT_SMER);
                break;
            default: 
                // vsechny motory budou vypnute
                continue;
        } /*End of Switch*/
        displayStatus(); // Zobraz stav programu
    }
    // Kontrola zavor
    const unsigned char input = inportb(PORT_IN); // Precteni stavu zavor
    
    // vypni motor pokud byla dosazena jeho zavora
    // zavora aktivni -> log. 0 => negace = 1 => nahraje se do motoru => vypne jej
    // bit 4-7 na vstupu nezapojeny => po negaci log. 0 => neovlivni vystup
    *output |= ~(input);
}
