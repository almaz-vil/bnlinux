/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
//**********************************
// * Залипание SHIFT
//************************************
#ifndef SHIFT
#define SHIFT

#include <X11/Xlib.h>

#ifndef BNLINUX_ONY
#include "bnlinux_ony.h"
#endif

#ifndef ZNAK
#include "znaki.h"
#endif

#ifndef CLOVO
#include "clovo.h"
#endif

#ifndef SHIFT_ONY
#include "shift_ony.h"
#endif

class Shift: public Shift_ony
{
private:
    Display *display;
    int fShift = 0;
public:
    Shift();
    ~Shift();
    void On() override;
    void Reshim(char key, bnlinux_ony *bnlinux) override;
    
};


#endif 