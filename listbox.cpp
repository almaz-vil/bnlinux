/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
//***********************
//**класс список слов
//*******************


#include <string>
#include <cstdlib>

#ifndef TYPEE
#include "typee.h"
#endif
#ifndef LIST_BOX
#include "listbox.h"
#endif
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


ListBox::ListBox(){
	Log("Создание объекта список ListBox");
	char *display_name=nullptr;
	if (display_name == NULL) display_name = XDisplayName (NULL) ;
	if ((display=XOpenDisplay(display_name))==nullptr)	Log("XOpen error!");
}

/*Необходимость очистить вывод перед записью в вывод*/
void ListBox::ClearOn(){
	this->claer=true;
}

void ListBox::Clear(){
	if(this->claer){
		this->claer=false;
		Log("Очистка списка слов");
		ClearLog();
		for (int i=0; i<this->count_item_label; i++){
			this->ListClov[i]="";
			}
		this->count_item_label=0;
	}
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
int ListBox::Add (const char *sText)
{
//	Log(" Добавления слова и номера");
	this->ClearOn();
	 string sn= string(sText);
	 string m;
	for(unsigned i = 0; i < sn.length(); ++i) {
		m.append(ClovoPrintUTFS(i,sn));
	}
    string cmdf="echo -n \" ";
	cmdf.append( to_string(this->count_item_label)+". ");
	cmdf.append(m);
	cmdf.append(" \" >> /tmp/bnlinux_sv ");
	command_shell(cmdf.c_str());
	this->ListClov[this->count_item_label].append(sText);
	return this->count_item_label++;
}
/*return 
	false -пробел не добавлял
	true -пробел добавлял*/
bool ListBox::Select(int n, Clovo *clovo){
	 Log("Выбор слова из словоря");
	 if(!this->count_item_label)
	 	return false;
	 int id= n-48;
	 if (!((id>-1)&&(id<7))){ //!(n>47)&&(n<58))
		 this->Clear();
		 return false;
	 }
 	 string sn=this->ListClov[id];
 	int len=sn.length();
	 string m;
	for(unsigned i = clovo->Count(); i < len; ++i) {
		m.append(ClovoPrintUTFS(i,sn));
	}
	string temp;
	int lenn = sn.length();
	for(int j=clovo->Count(); j < lenn; ++j){
		for (int i = 0; i < 140; i++){
			if (sn[j]==clovo->alfavid[i]){
				temp+=clovo->alfavideng[i];
			}
		}
	}
	temp+=' ';
	clovo->Clear();
	clovo->Add(sn);
 	key_del(display);
	string cmdf="echo -n \"";
		cmdf.append(m.c_str());
		  cmdf.append(" ");//+ пробел
      cmdf.append("\" |xclip -selection c");
	//key_pavse(cmdf.c_str(), display);
	press_keys(temp.c_str(), display);
	this->Clear();//Очистка после пробела.
	clovo->Add_Probel();
	Log("Выбор слова из словоря END");

	return true;
}

