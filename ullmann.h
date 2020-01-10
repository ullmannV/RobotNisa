#ifndef ULLMANN_H
#define ULLMANN_H

#define MAX_RYCHLOST_ROBOTA 450 // Hz
#define VSE_VYPNUTO 0xFF // periferie jsou aktivni v logicke nule
#define SEC_TO_MILISEC_COEFFICIENT 1000 // 1 s = 1000 ms
#define BIT_TAKT 5

const unsigned short port_vystupy = 0x300; // P1
const unsigned short port_vstupy = 0x300;  // P3
const unsigned short taktovaci_frekvence_robota = 100; // Hz

#endif