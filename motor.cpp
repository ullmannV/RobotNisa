#include <iostream>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include "motor.h"

Motor::Motor(unsigned char BIT) : BIT(BIT) {
    printf("Vytvořen nový objekt motoru \n");
}

Motor::~Motor() {
    printf("Destruktor objektu motor \n");
}

void Motor::Nastartuj(void) {
    this->chod = true;
}

void Motor::Vypni(void) {
    this->chod = false;
}

void Motor::ObratOtacky(void) {
    if(this->smer_otaceni) {
        this->smer_otaceni = false;
    }
    else {
        this->smer_otaceni = true;
    }
}

void Motor::GetSmerOtaceni(void) const {
    return this->smer_otaceni;
}

bool Motor::JeZapnut(void) const {
    return this->chod;
}
