#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include "motor.hpp"
#include "zavora.hpp"
#include "ullmann.hpp"



int main(void) {
    // inicializace
    clrscr(); // vycisti plochu

    bool program_run = true;

    // nekonecny cyklus
    while(program_run) {
        
        // zmena chovani programu pomoci vstupu z klavesnice
        switch() { // TODO vstup klavesy
            case 'q': {
                program_run = false;
                break;
            }
        } 
    }
    printf("Program vypnut \n");
    return 0;
}