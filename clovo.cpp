/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//****************************************
//**класс для формирования списока слов***
//****************************************


#include <string>
#include <fstream>

#ifndef TYPEE
#include "typee.h"
#endif
#ifndef CLOVO
#include "clovo.h"
#endif
#ifndef ZNAK
#include "znaki.h"
#endif

#define OTP 1

#ifdef OTP
	#define PATHRUSALFAVIT "/opt/bnlinux/local/alfavid.txt"
#else
	#define PATHRUSALFAVIT "/usr/local/alfavid.txt"
#endif


using namespace std;



void  Clovo::LoadAlfavid(){
	Clovo::alfavid = new char[141];
	 ifstream file(PATHRUSALFAVIT);//создаем объект потока istream  по имени file
		if(!file.is_open()) {
			printf("not file\n");
			return ;
		}
		 int n = 140;
		//Создаем буффер для чтения
		 Clovo::alfavid[n]=0;
		 file.get(Clovo::alfavid,n);
		file.close();

}


Clovo::Clovo(Lang lang){
	this->lang=lang;
	
}

void Clovo::SetLang(Lang lang){
	this->lang=lang;
}

int Clovo::Count(){
	return this->clov.length();
}

void Clovo::Clear(){
	this->clov.clear();
}

int Clovo::Add(int ch){
	if (ch==8){		
		if (this->Count())
			this->clov.erase(this->clov.end()-1);
		return this->Count();
	}
	//     								              Ф         Я          ф         я         х          Х        ъ          Ъ        Ж           ж        Э         э         Б         б         Ю          ю
	if ((!(((ch>64)&&(ch<91))||((ch>96)&&(ch<123))||(ch==123)||(ch==91)||(ch==125)||(ch==93)||(ch==58)||(ch==59)||(ch==34)||(ch==39)||(ch==60)||(ch==44)||(ch==62)||(ch==46)||(ch==126)||(ch==96)))){
		this->Clear();
		return this->nomer;
		}
	
	if (this->lang==RUS) {
		this->clov+=Clovo::alfavid[ch];
			}
	return this->Count();
}
