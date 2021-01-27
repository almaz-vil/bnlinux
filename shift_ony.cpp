/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//**********************************
// * Залипание SHIFT
//************************************


#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <X11/Xlib.h>


#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <linux/kd.h>
#include <linux/keyboard.h>


#include "shift_ony.h"


#ifndef BNLINUX_ONY
#include "bnlinux_ony.h"
#endif

#ifndef CLOVO
#include "clovo.h"
#endif

#ifndef ZNAK
#include "znaki.h"
#endif

#ifndef CLOVO
#include "clovo.h"
#endif

Shift_ony::Shift_ony(){

    //fprintf(stderr, "conctuctor Shift_ony\t");
}

Shift_ony::~Shift_ony(){

}
void Shift_ony::On(){

  //  fprintf(stderr, "Shift_ony fuction On\t");
}
void Shift_ony::Reshim(char key, bnlinux_ony *bnlinux){

//    fprintf(stderr, "Shift_ony function Reshim\t");
}
    