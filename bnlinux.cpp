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
#include <algorithm>

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
	  listrus.shrink_to_fit();
      clovo = new Clovo(RUS);
      clovo->LoadAlfavid();
      Reshim(args);	  	
	
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
	  	orfo=1;
	  else 
		orfo=0;		  	
	
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
	
	if((!this->Find())){	
		clovo->Upda();
		if(!this->Find()){
			if(this->orfo)
				if(clovo->Count()>1){
					system("play /opt/bnlinux/local/sound/VClovar.wav 2>/dev/null");				
					int len=clovo->clov.length();
					string m;
					for(unsigned i = 0; i < len; ++i) {
						m.append(listbox->ClovoPrintUTFS(i,clovo->clov));
					}
					string s="ОПЕЧАТКА (добавьте слово '";
					s.append(m);
					s.append("' в словарь <F4>)");
					info(s);
					}
		}
	}

}

int bnlinux::Find(){
	int not_clovo=0;
	for(auto iter = listrus.begin(); iter != listrus.end(); ++iter)
		{
			string string_is_list=*iter;
			size_t up=string_is_list.find(clovo->clov);
			if((up!= string::npos)&&(up==0)){
				not_clovo=1;
				for(auto iter1 = iter; iter1 != listrus.end(); ++iter1){
					string s_is_list=*iter1;
					s_is_list.erase(s_is_list.end()-1);
					if (listbox->Add(s_is_list.c_str())==6)return not_clovo;
				}
			}
		}
	return not_clovo;	
}

void bnlinux::Add(){
	if(this->orfo)
		if(clovo->Count()>1){
			info("Ждите...");
			clovo->clov+=clovo->alfavid[1];
			listrus.push_back(clovo->clov);
			std::sort(listrus.begin(), listrus.end());
			listrus.shrink_to_fit();
			ofstream file;
			file.open(PATHRUS);
			for(int i=0;i<listrus.size();++i){
				file<<listrus[i]<<endl;
			}
			file.close();
			info("Добавлено!");
			}
	
}