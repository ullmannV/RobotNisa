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

// nastavitelne parametry:
const unsigned short RYCHLOST_ROBOTA = 450; // Hz

// podminka behu programu
bool program_run = true; // C++ kompilator => netřeba definovat bool

// deklarace funkci
void initPoloha(unsigned char* output);
void initRameneChapadla(unsigned char* output);
void manualControl(unsigned char* output);
void testZavory(const unsigned char input, unsigned char* output, const unsigned char bit);

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
    const unsigned char input = inportb(PORT_IN);

    // testy dorazů
    testZavory(input, output, BIT_ZAKLADNA);
    testZavory(input, output, BIT_HLAVNI_RAMENO);
    testZavory(input, output, BIT_CELIST);


    // kalibrace vsech poloh dokoncena?
    if(!(input & ~(~(1<<BIT_ZAKLADNA) & ~(1<<BIT_HLAVNI_RAMENO) & ~(1<<BIT_CELIST))))
        rezimProvozu = initRameneChapadla;
    
}

void initRameneChapadla(unsigned char* output) {
    
    // roztoc motory
    *output &= ~(1<<BIT_RAMENO_CELISTI);
    
    // nastav smer otaceni
    *output |= 1<<BIT_SMER;
    
    // nacti vstupy
    const unsigned char input = inportb(PORT_IN);
    
    // test dorazu
    testZavory(input, output, BIT_RAMENO_CELISTI);

    // kalibrace polohy dokoncena
    if(!(input & 1<<BIT_RAMENO_CELISTI)) 
        rezimProvozu = manualControl;

}

void manualControl(unsigned char* output) {
        
    // kontrola zda byla stisknuta klavesa
    if(kbhit()) {
        const int pressed_key = getch(); // vyzvednuti z bufferu
        
        // reset motorů a smeru  
        *output |= 1<<BIT_ZAKLADNA | 1<<BIT_HLAVNI_RAMENO | 1<<BIT_RAMENO_CELISTI | 1<<BIT_CELIST | 1<<BIT_SMER; 
            
        // reakce na dane stiskle klavesy
        switch(pressed_key) {
            case 'q':       // Vypnuti programu
                program_run = false;
                break;      
            case 'a':       // Otoceni zakladny proti smeru hodinovych rucicek
                *output &= ~(1<<BIT_ZAKLADNA) & ~(1<<BIT_SMER);
                break;
            case 'd':       // Otoceni zakladny po smeru hodinovych rucicek
                *output &= ~(1<<BIT_ZAKLADNA);
                break;
            case 'w':       // Zvednuti hlavniho ramene
                *output &= ~(1<<BIT_HLAVNI_RAMENO) & ~(1<<BIT_SMER);
                break;
            case 's':       // Snizeni hlavniho ramene
                *output &= ~(1<<BIT_HLAVNI_RAMENO);
                break;
            case 'r':       // Zvednuti ramene celisti
                *output &= ~(1<<BIT_RAMENO_CELISTI);
                break;
            case 'f':       // Snizeni ramene celisti
                *output &= ~(1<<BIT_RAMENO_CELISTI) & ~(1<<BIT_SMER);
                break;      
            case 'x':       // Zavreni celisti
                *output &= ~(1<<BIT_CELIST);
                break;
            case 'c':       // Otevreni celisti
                *output &= ~(1<<BIT_CELIST) & ~(1<<BIT_SMER);
                break;
            default: 
                // vsechny motory budou vypnute
                continue;
        } /*End of the Switch*/
    }
    
    // Kontrola zavor
    const unsigned char input = inportb(PORT_IN); // Precteni stavu zavor    
    
    // pokud smer je v log. 0
    if(!(*output & 1<<BIT_SMER)) {
        
        // testy dorazů
        testZavory(input, output, BIT_ZAKLADNA);
        testZavory(input, output, BIT_HLAVNI_RAMENO);
        testZavory(input, output, BIT_CELIST);
    }
    else {
        
        // test dorazu
        testZavory(input, output, BIT_RAMENO_CELISTI);
    }
}

void testZavory(const unsigned char input, unsigned char* output, const unsigned char bit) {
    // Pokud je zavora aktivni 
    if(!(input & 1<<bit))
        *output |= 1<<bit; // vypni motor
}
