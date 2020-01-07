#ifndef MOTOR_HPP
#define MOTOR_HPP

class Motor {
    public:
        Motor(unsigned char BIT);
        ~Motor();
        void Otoc(void);
        void ObratOtacky(void);
        bool JeZapnut(void) const;
        bool GetSmerOtaceni(void) const;
        
    private:
        bool smer_otaceni;       // smysl otaceni motoru                     
        const unsigned char BIT; // na jakem bitu je zapojen motor
};
#endif