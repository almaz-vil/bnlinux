/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//**********************************
// * Реакция на знаки припенания
//************************************

// !-33; .-46; ?-63

#include <string.h>
#include <X11/Xlib.h>

#include "znaki.h"

#ifndef TYPEE
#include "typee.h"
#endif

using namespace std;

Znak::Znak(){
    Log("Констуктор класса Знак");
   	char *display_name=nullptr;
	if (display_name == NULL) display_name = XDisplayName (NULL) ;
	if ((display=XOpenDisplay(display_name))==nullptr)	printf("XOpen error!");
}
/*Активация флажка пробел*/
void Znak::Probel(){
    Log("Активация флажка пробел");
    this->fProbel=1;
}

void Znak::BolBukvaClear(){
    Log("Деактивация флажка большая буква");
    this->fBolBukva=0;
}

void Znak::ProbelClear(){
    Log("Деактивация флажка пробел");
    this->fProbel=0;
}

void Znak::obrabotka(int chr, Clovo *clovo){
    Log("Обработка в классе Знак");
    if (((chr==47)||(chr==33)||(chr==38))){
        if(clovo->Count()>1){
            Log("Активация флажка большая буква");
            this->fBolBukva=1;
        }
        if(this->fProbel){
            this->ProbelClear();
            key_del(this->display);    
            key_del(this->display);
            string cmdf="echo -n \"";
            if(chr==47)cmdf.append(". ");
            if(chr==33)cmdf.append("! ");
            if(chr==38)cmdf.append("? ");
            cmdf.append("\" |xclip -selection c");
            key_pavse(cmdf.c_str(), display);  
            return;
        }
        else {
            if (clovo->Count()<2)
                return;
            string cmdf="echo -n \"";
            cmdf.append(" ");
            cmdf.append("\" |xclip -selection c");
            key_pavse(cmdf.c_str(), display);                
            return;
        }
    }
}

void Znak::bolch(int i, Clovo *clovo){
    Log("Замена большой буквы строчной");
    char r[2]={0,0};
    *r=clovo->alfavid[i];
    char buf[4];
    cp1251_to_utf8(r, buf);
    string cmdf="echo -n \"";
    cmdf.append(buf);
    cmdf.append("\" |xclip -selection c");
    key_del(display);
    key_pavse(cmdf.c_str(), display);  
}

/*Преобразование заглавной буквы строчной.*/
void Znak::Uppad(Clovo *clovo){
    Log("Преобразование заглавной буквы строчной. ");
    this->ProbelClear();
    if(this->fBolBukva){
       this->BolBukvaClear();
        if (clovo->Count()){
            this->CharDn(clovo->clov.front(), clovo);
        }
    }
 
}

void Znak::CharDn(char& chas, Clovo* clovo){
    for (int i = 0; i < 140; i++){
        if (chas==clovo->alfavid[i]){
            if ((i>97)&&(i<124)){
                this->bolch(i-32, clovo);
                return;
                }
            switch (i){
                case 96: // ё->Ё
                    this->bolch(126, clovo);
                    break;
                case 91: //х->Х
                    this->bolch(123, clovo);
                    break;             
                case 59: //ж->Ж
                    this->bolch(58, clovo);
                    break;                
                case 39: //э->Э
                    this->bolch(34, clovo);
                    break;                
                case 46: //ю->Ю
                    this->bolch(62, clovo);
                    break;
                case 44: //б->Б
                    this->bolch(60, clovo);
                    break;
            }
            return;
        }
    }    
}