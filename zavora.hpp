#ifndef ZAVORA_HPP
#define ZAVORA_HPP

class Zavora {
    public:
        Zavora(unsigned char BIT, unsigned short PORT);
        ~Zavora();
        bool DorazDosazen(void) const;       
        
    private:        
        const unsigned char BIT; // na jakem bitu je zapojena zavora
        const unsigned short PORT; // z jakeho portu se maji cist vstupy
};
#endif