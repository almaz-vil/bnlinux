/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
//gh****************
//hv* основной класс прослойка
// vc**************************

#include <string>
#include <fstream>
#include <unistd.h>
#include <algorithm>

#ifndef BNLINUX
#include "bnlinux.h"
#endif
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
	  Log("Загрузка словаря");
      ifstream file(PATHRUS);
      string str;        
      while(getline(file,str)) 
      {                      
                             
          listrus.push_back(str);
      }
      file.close();
	  listrus.shrink_to_fit();
      Log("Словарь загружен");
	  clovo = new Clovo(RUS);
      clovo->LoadAlfavid();
      Reshim(args);	  	
	
}

bnlinux::~bnlinux()
{    
	listrus.clear();
}

void bnlinux::Reshim(struct arguments args){
	  Log("Выбор режимов работы");
	  if(args.bn)
		listbox=new ListBox(); 
	  else
	  	listbox=new ListBox_ony();
	  if(args.znak)
	    znak = new Znak();
	  else
	    znak = new Znak_ony();	  	
	  if(args.orfo)
	  	{orfo=1;		Log("ОРФО режим включён");}
	  else 
		{orfo=0;		  	Log("ОРФО режим отключён");}
	listbox->ClearOn();
}

void bnlinux::Print(int key_char, Lang lang){
	Log("Press ", key_char);
	if(lang==EN){
	  //Log("Символ английкого языка");
	  listbox->Clear();
	  return;
	}
	Log("clovo->Count=", clovo->Count());
	if (clovo->Count()&&(key_char>47)&&(key_char<58)) {
		Log("Символ от 47 до 58, выбор слова из словоря");
		if(listbox->Select(key_char, clovo)){
			Log("Слово есть +пробел!!");
			znak->Probel();
		}
		else{
			Log("Общий сброс!!");
			znak->BolBukvaClear();
			znak->ProbelClear();
			clovo->Clear();
		}
		
		return;
	}

	if((key_char>47)&&(key_char<58)){
		znak->BolBukvaClear();
	}
	znak->obrabotka(key_char, clovo);
	if (clovo->Add(key_char)){
		znak->ProbelClear();
		listbox->Clear();
	 	return;
	 }
	znak->Uppad(clovo);
	listbox->Clear();
	
	if((!this->Find())){	
		Log("(1)В словаре нет слова");
		clovo->Upda();
		if(!this->Find()){
			Log("(2)В словаре нет слова");
			if(this->orfo||clovo->Count()){
				Log("Информация об опечатке!!");
				command_shell("play /opt/bnlinux/local/sound/VClovar.wav 2>/dev/null");				
				string s="ОПЕЧАТКА (добавьте слово '";
				for(unsigned i = 0; i < clovo->clov.length(); ++i) {
					s.append(listbox->ClovoPrintUTFS(i,clovo->clov));
				}					
				s.append("' в словарь <F4>)");
				info(s);
				listbox->ClearOn();
				}
		}
	}

}

int bnlinux::Find(){
	Log("Поиск слова в словаре");
	int not_clovo=0;
	for(auto iter = listrus.begin(); iter != listrus.end(); ++iter)
		{
			string string_is_list=*iter;
			size_t up=string_is_list.find(clovo->clov);
			if((up!= string::npos)&&(up==0)){
				not_clovo=1;
				string_is_list.erase(string_is_list.end()-1);
				listbox->Add(string_is_list.c_str());
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
	Log("Добавление словарь");
	if(this->orfo)
		if(clovo->Count()>1){
			Log("Слово можно добавлять");
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
			Log("Слово добавил");
			}
	
}