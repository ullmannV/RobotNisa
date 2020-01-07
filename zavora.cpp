#include <iostream>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include "zavora.hpp"

Zavora::Zavora(unsigned char BIT, unsigned short PORT) : BIT(BIT), PORT(PORT) {
    printf("Vytvořen nový objekt Zavora \n");
}

Zavora::~Zavora() {
    printf("Destruktor objektu Zavora \n");
}

bool Zavora::DorazDosazen(void) const {
    inportb(this->PORT, this->BIT);
}