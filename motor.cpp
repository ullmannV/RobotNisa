#include <stdio.h>
#include "motor.hpp"

Motor::Motor(unsigned char BIT) : BIT(BIT) {
    printf("Vytvořen nový objekt Motoru \n");
}

Motor::~Motor() {
    printf("Destruktor objektu Motor \n");
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

