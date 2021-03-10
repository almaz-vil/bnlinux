/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
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

#ifndef SHIFT
#include "shift.h"
#endif

#ifndef BNLINUX_ONY
#include "bnlinux_ony.h"
#endif

#ifndef CLOVO
#include "clovo.h"
#endif

Shift::Shift(){
    Log("Конструктор класса Shift");
    char *display_name=nullptr;
	if (display_name == NULL) display_name = XDisplayName (NULL) ;
	if ((display=XOpenDisplay(display_name))==nullptr)	printf("XOpenDisplay error!");

}

Shift::~Shift(){
    Log("Деструктор класса Shift");
    XCloseDisplay(this->display);	
}
void Shift::On(){
    Log("Активация флажка Shift");
    this->fShift=1;
}

void Shift::Reshim(char key , bnlinux_ony *bnlinux){
    if (this->fShift){
        this->fShift=0;
        Log("Деактивация флажка Shift. Программное нашатие клавиш!");
        int kc=XKeysymToKeycode(this->display, XK_BackSpace);
        XTestFakeKeyEvent(this->display, kc, True, 0);
        XFlush(this->display);
        XTestFakeKeyEvent(this->display, kc, False, 0);
        XFlush(this->display);
        kc = XKeysymToKeycode(this->display, XK_Shift_L);
        XTestFakeKeyEvent(this->display, kc, True, 0);
        XFlush(this->display);
        sleep(1);
        kc = XKeysymToKeycode(this->display, key);
        XTestFakeKeyEvent(this->display, kc, True, 0);
        XFlush(this->display);
        XTestFakeKeyEvent(this->display, kc, False, 0);
        XFlush(this->display);
        sleep(1);
        kc = XKeysymToKeycode(this->display, XK_Shift_L);
        XTestFakeKeyEvent(this->display, kc, False, 0);
        XFlush(this->display);
        sleep(1);
        XFlush(this->display);
         //if (key==49) bnlinux->Print(33);// Нажал ! 
    	if (key==55) bnlinux->Print(38);// Нажал ? 
        else bnlinux->Print(key-16);   	
    }
}