/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
//**********************************
// * Залипание SHIFT
//************************************
#ifndef SHIFT_ONY
#define SHIFT_ONY

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

class Shift_ony
{
private:
    Display *display;
    int fShift = 0;
public:
    Shift_ony();
    ~Shift_ony();
   virtual void On();
   virtual void Reshim(char key, bnlinux_ony *bnlinux);
    
};


#endif 