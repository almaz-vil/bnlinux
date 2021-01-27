/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//gh****************
//hv* основной класс прослойка
// vc**************************

#include <string>
#include <fstream>
#include <unistd.h>

#include "bnlinux.h"
#ifndef LIST_BOX
#include "listbox.h"
#endif
#ifndef LIST_BOX_ONY
#include "listbox_ony.h"
#endif
#ifndef CLOVO
#include "clovo.h"
#endif
#ifndef ZNAK
#include "znaki.h"
#endif
#ifndef ZNAK_ONY
#include "znaki_ony.h"
#endif
#ifndef TYPEE
#include "typee.h"
#endif


#define PATHRUS "/opt/bnlinux/local/russ.txt"



using namespace std;

bnlinux::bnlinux(struct arguments args)
{    
      ifstream file(PATHRUS);//создаем объект потока istream  по имени file
      string str;         //переменная стринг для строки
      while(getline(file,str)) //getline(istream & is, string &s,char c='\n'),читает из потока is, в строку s пока
      {                        //не встретит символ c (без этого символа до новой строки)
                              // возвращает свой объект istream, в условии проверяется состояние iostate флагa, значение этого флага будет ложным, если достигнет конца файла, или будет ошибка ввода или читаемого типа
          listrus.push_back(str);//ной строки
      }
      file.close();
      clovo = new Clovo(RUS);
      clovo->LoadAlfavid();
      if(args.bn)
		listbox=new ListBox();
	  else
	  	listbox=new ListBox_ony();
	  if(args.znak)
	    znak = new Znak();
	  else
	    znak = new Znak_ony();	  	
	  if(args.orfo)
	  	orfo=0;
	  else 
		orfo=1;		  	
	
}

bnlinux::~bnlinux()
{    
	listrus.clear();
}

void bnlinux::Reshim(struct arguments args){
	  if(args.bn)
		listbox=new ListBox();
	  else
	  	listbox=new ListBox_ony();
	  if(args.znak)
	    znak = new Znak();
	  else
	    znak = new Znak_ony();	  	
	  if(args.orfo)
	  	orfo=0;
	  else 
		orfo=1;		  	
	
}

void bnlinux::Print(int key_char, Lang lang){
	if(lang==EN){
	  listbox->Clear();
	 return;
	}
	if (clovo->Count())
		if ((key_char>47)&&(key_char<58)) {
			znak->probel();
			listbox->Select(key_char, clovo);
			return;
		}
	znak->obrabotka(key_char, clovo);

	int nomer=clovo->Add(key_char);
	
	if (nomer<1){
		listbox->Clear();
	 	return;
	 }
	znak->Uppad(clovo);
	listbox->Clear();
	int sound_not_clovo=this->orfo;
	int item_for_exit=0;
    	
	for(auto iter = listrus.begin(); iter != listrus.end(); ++iter)
	{
	    string string_is_list=*iter;
		size_t up=string_is_list.find(clovo->clov);
		if((up!= string::npos)&&(up==0)){
				string_is_list.erase(string_is_list.end()-1);
				listbox->Add(string_is_list.c_str());
             	sound_not_clovo=1;
				for(auto iter1 = iter; iter1 != listrus.end(); ++iter1)
      			{
					string s_is_list=*iter1;
					s_is_list.erase(s_is_list.end()-1);
					listbox->Add(s_is_list.c_str());
					item_for_exit=item_for_exit+1;
					if (item_for_exit==5)return;
				}
		}
	}
	//Если слово отсутствует в словоре опечатка!
	if(sound_not_clovo==0)
    if(clovo->Count()>1){
      system("play /opt/bnlinux/local/sound/VClovar.wav 2>/dev/null");				
      info("ОПЕЧАТКА");
	 }

}
