#include <stdio.h>
#include "motor.hpp"

Motor::Motor(unsigned char BIT) : BIT(BIT) {
    printf("Vytvořen nový objekt Motoru \n");
}

Motor::~Motor() {
    printf("Destruktor objektu Motor \n");
}

void Motor::Otoc(void) {

}

void Motor::ObratOtacky(void) {
    if(this->smer_otaceni)
        this->smer_otaceni = false;
    else 
        this->smer_otaceni = true;
}

bool Motor::GetSmerOtaceni(void) const {
    return this->smer_otaceni;
}



