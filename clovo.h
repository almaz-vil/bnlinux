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
		int Add(int ch);
		int Count();
		char *alfavid;// = new char[141];
		void LoadAlfavid();
		std::string clov ="";
	private:
		char ENinRus(int ch);
		int nomer = -1;
		Lang lang = Lang::RUS;
};

#endif