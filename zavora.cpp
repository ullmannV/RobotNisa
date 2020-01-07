#include <stdio.h>
#include <dos.h>
#include "zavora.hpp"

Zavora::Zavora(unsigned char BIT, unsigned short PORT) : BIT(BIT), PORT(PORT) {
    printf("Vytvořen nový objekt Zavora \n");
}

Zavora::~Zavora() {
    printf("Destruktor objektu Zavora \n");
}

bool Zavora::DorazDosazen(void) const {
    if(inportb(this->PORT) & 1<<this->BIT) 
        return false;
    else
        return true;    
}