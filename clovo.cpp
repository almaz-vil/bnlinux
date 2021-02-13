/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//****************************************
//**класс для формирования списока слов***
//****************************************

#include <iostream>
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
	Log("Загрузка файла алфавита");
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
	Log("Конструктор класса Clovo");
	this->lang=lang;
	
}

void Clovo::SetLang(Lang lang){
	Log("Язык ввода изменилься");
	this->lang=lang;
}

int Clovo::Count(){
	Log("Возврат количества символов в слове");
	return this->clov.length();
}

void Clovo::Clear(){
	Log("Очистка слова");
	this->clov.clear();
}

int Clovo::Add(std::string str){
	Log(" Добавления слова к слову");
	this->clov.append(str);
}

bool Clovo::Add(int ch){
	if (ch==8){		
		Log("Удаление символа с конца слова");
		if(this->probel)this->Clear_Probel(false);
		if (this->Count())
			this->clov.erase(this->clov.end()-1);
		return this->Count()<1?true:false;
	}
	//     								              Ф         Я          ф         я         х          Х        ъ          Ъ        Ж           ж        Э         э         Б         б         Ю          ю
	if ((!(((ch>64)&&(ch<91))||((ch>96)&&(ch<123))||(ch==123)||(ch==91)||(ch==125)||(ch==93)||(ch==58)||(ch==59)||(ch==34)||(ch==39)||(ch==60)||(ch==44)||(ch==62)||(ch==46)||(ch==126)||(ch==45)||(ch==96)))){
		this->Clear();
		return this->nomer<1?true:false;
		}
	// символы ъ Ъ - не могут быть первыми	
	if(((!(this->Count()))||this->probel)&&((ch==125)||(ch==93)||(ch==45))){
		if(this->probel)
			this->Clear_Probel();
		this->Clear();
		return this->nomer<1?true:false;
		
	}
	if (this->lang==RUS) {
		Log(" Добавления символа в слово");
		if(this->probel)
			this->Clear_Probel();
		this->clov+=Clovo::alfavid[ch];
	}
	return this->Count()<1?true:false;
}

void Clovo::Upda(){
	
	Log("Изменение регистра с Заглавного на строчные");
	this->CharUp(this->clov.front());		
	this->CharUp(this->clov.back());		
	
}	

void Clovo::CharUp(char& chas){
	for (int i = 0; i < 140; i++){            
		if(chas==this->alfavid[i]){
        	if ((i>65)&&(i<92)){
				chas=this->alfavid[i+32];
				return;
				}
			switch (i){
				case 126: // Ё->ё
					chas=this->alfavid[96];
					break;
				case 123: //Х->х
					chas=this->alfavid[91];
					break;             
				case 58: //Ж->ж
					chas=this->alfavid[59];
					break;                
				case 34: //Э-э
					chas=this->alfavid[39];
					break;                
				case 62: //Ю->ю
					chas=this->alfavid[46];
					break;
				case 60: //Б->б
					chas=this->alfavid[44];
					break;
			}
			return;
        }
	}
}
bool Clovo::Probel(){
	Log("Запрос  пробела");
	return this->probel;
}

/*Добавления пробела в слово*/
void Clovo::Add_Probel(){
	Log(" Добавления пробела в слово");
	this->clov+=this->alfavid[1];
	this->probel=true;
}

/*Пробел отключение*/
void Clovo::Clear_Probel(bool clear){
	Log("Пробел отключение");
	if(clear)
		this->Clear();
	this->probel=false;
}
