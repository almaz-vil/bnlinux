/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//***********************
//**класс список слов
//*******************


#include <string>

#include "typee.h"
#include "listbox.h"
#include "clovo.h"
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


ListBox::ListBox(){
	char *display_name=nullptr;
	if (display_name == NULL) display_name = XDisplayName (NULL) ;
	if ((display=XOpenDisplay(display_name))==nullptr)	printf("XOpen error!");
}


void ListBox::Clear(){
	 string cmdf="truncate -s 0 /tmp/bnlinux_sv; echo -n \" ";
	cmdf.append(" \" >> /tmp/bnlinux_sv ");
	system(cmdf.c_str());
	for (int i=0; i<this->count_item_label; i++){
		this->ListClov[i]="";
	    }
	this->count_item_label=0;
}
//****************

 string ListBox::ClovoPrintUTFS(int n,  string sn){
	char buf[4], in[2] = {0, 0};
        *in = sn[n];
        cp1251_to_utf8(in, buf);
         string s;
        s.append(buf);
       return s;

}
/**
 * Добавления слова и номера к номера
 */
void ListBox::Add (const char *sText)
{
	 string sn="";
	sn.append(sText);
	int len=sn.length();
	 string m;
	for(unsigned i = 0; i < len; ++i) {
		m.append(ClovoPrintUTFS(i,sn));
	}
    string cmdf="echo -n \" ";
	cmdf.append( to_string(this->count_item_label)+". ");
	cmdf.append(m);
	cmdf.append(" \" >> /tmp/bnlinux_sv ");
	system(cmdf.c_str());
	this->ListClov[this->count_item_label].append(sText);
	this->count_item_label=this->count_item_label+1;
}

 void ListBox::Select(int n, Clovo *clovo){
	 int id= n-48;
	 if (!((id>-1)&&(id<6))){
		 this->Clear();
		 return;
	 }
 	 string sn=this->ListClov[id];
 	int len=sn.length();
	 string m;
	for(unsigned i = clovo->Count(); i < len; ++i) {
		m.append(ClovoPrintUTFS(i,sn));
	}
 	//for (int i = 0; i<clovo->Count()+1 ;i++){
		 key_del(display);
 	//}
 	//GtkClipboard *clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
 	//gtk_clipboard_set_text(clip,m.c_str(),-1);
	       string cmdf="echo -n \"";
		  cmdf.append(m.c_str());
		  cmdf.append(" ");//+ пробел
      cmdf.append("\" |xclip -selection c");
	key_pavse(cmdf.c_str(), display);
	this->Clear();//Очистка после пробела.
	clovo->Clear();
}
