/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
#ifndef CLOVO
#define CLOVO

#ifndef TYPEE
#include "typee.h"
#endif

#include <string>

class Clovo{
	public:
		Clovo(Lang lang);
		void SetLang(Lang lang);
		void Clear();
		bool Add(int ch);
		int Add(std::string str);
		int Count();
		char *alfavid;// = new char[141];
		void LoadAlfavid();
		void Upda();
		void Add_Probel();
		bool Probel();
		void Clear_Probel(bool clear=true);
		std::string clov ="";
	private:
		/*Наличие пробела в слове*/
		bool probel=false;
		char ENinRus(int ch);
		int nomer = -1;
		Lang lang = Lang::RUS;
};

#endif