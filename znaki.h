/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//**********************************
// * Реакция на знаки припенания
//************************************
#ifndef ZNAK
#define ZNAK


#ifndef CLOVO
#include "clovo.h"
#endif
#ifndef ZNAK_ONY
#include "znaki_ony.h"
#endif

#include <X11/Xlib.h>


class Znak:public Znak_ony
{
    private:
        int fProbel=0;
        int fBolBukva=0;
        int fBolBukvaListSelect=0;
        Display *display;
        void bolch(int i, Clovo *clovo);
    public:
        Znak();
        ~Znak();
        void obrabotka(int chr, Clovo *clovo) override;
        void probel() override;
        void Uppad(Clovo *clovo) override;
};

#endif 