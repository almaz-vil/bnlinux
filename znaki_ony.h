/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//**********************************
// * Реакция на знаки припенания
//************************************
#ifndef ZNAK_ONY
#define ZNAK_ONY


#ifndef CLOVO
#include "clovo.h"
#endif

class Znak_ony
{
    private:
    public:
        Znak_ony();
        ~Znak_ony();
        virtual void obrabotka(int chr, Clovo *clovo);
        virtual void probel();
        virtual void Uppad(Clovo *clovo);
};

#endif 