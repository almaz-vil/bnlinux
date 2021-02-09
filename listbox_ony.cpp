/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//***********************
//**класс список слов
//*******************


#include <string>

#include "typee.h"

#include "listbox_ony.h"
#ifndef CLOVO
#include "clovo.h"
#endif
#ifndef ZNAK
#include "znaki.h"
#endif


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <linux/kd.h>
#include <linux/keyboard.h>



using namespace std;


ListBox_ony::ListBox_ony(){
}


void ListBox_ony::Clear(){

	 string cmdf="truncate -s 0 /tmp/bnlinux_sv; echo -n \" ";
	cmdf.append(" \" >> /tmp/bnlinux_sv ");
	command_shell(cmdf.c_str());
}

int ListBox_ony::Add (const char *sText){
}

void ListBox_ony::Select(int n, Clovo *clovo){
}

std::string ListBox_ony::ClovoPrintUTFS(int n,  std::string sn){

}
