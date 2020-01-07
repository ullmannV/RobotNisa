#ifndef MOTOR_HPP
#define MOTOR_HPP

class Motor {
    public:
        Motor(unsigned char BIT);
        ~Motor();
        void Nastartuj(void);
        void Vypni(void);
        void ObratOtacky(void);
        bool JeZapnut(void) const;
        void GetSmerOtaceni(void) const;
        
    private:
        bool smer_otaceni;       // smysl otaceni motoru
        bool chod;               // stav zda se motor otaci
        const unsigned char BIT; // na jakem bitu je zapojen motor
};
#endif