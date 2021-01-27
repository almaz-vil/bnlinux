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
   	char *display_name=nullptr;
	if (display_name == NULL) display_name = XDisplayName (NULL) ;
	if ((display=XOpenDisplay(display_name))==nullptr)	printf("XOpen error!");
}

void Znak::probel(){
    this->fProbel=1;
}

void Znak::obrabotka(int chr, Clovo *clovo){
    if (((chr==47)||(chr==33)||(chr==38))){
        this->fBolBukva=1;
        if(this->fProbel){
            this->fProbel=0;
            key_del(this->display);    
            key_del(this->display);
            string cmdf="echo -n \"";
            if(chr==47)cmdf.append(". ");
            if(chr==33)cmdf.append("! ");
            if(chr==38)cmdf.append("? ");
            cmdf.append("\" |xclip -selection c");
            key_pavse(cmdf.c_str(), display);  
            this->fBolBukva=1;
        }
        else {
            if (clovo->Count()<2)
                return;
            string cmdf="echo -n \"";
            cmdf.append(" ");
            cmdf.append("\" |xclip -selection c");
            key_pavse(cmdf.c_str(), display);  
            this->fBolBukva=1;
           
        }
    }
}

void Znak::bolch(int i, Clovo *clovo){
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

void Znak::Uppad(Clovo *clovo){
    this->fProbel=0;
        
    if(this->fBolBukva){
        this->fBolBukva=0;
        this->fBolBukvaListSelect=1;
        char r[2]={0,0};
        for (int i = 0; i < 140; i++)
        {            
            string ch="";
            string cha="";
            char a[2]={0,0};
            *a=clovo->clov[0];
            cha.append(a);
            char b[2]={0,0};
            *b=clovo->alfavid[i];
             ch.append(b);
            if (ch==cha){
                if ((i>97)&&(i<124)){
                    this->bolch(i-32, clovo);
                }
                switch (i)
                {
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
                break;
            }
           
        }
    }
 
}