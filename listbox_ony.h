/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
#ifndef LIST_BOX_ONY
#define LIST_BOX_ONY

#include <string.h>
#include <X11/Xlib.h>

#ifndef CLOVO
#include "clovo.h"
#endif

class ListBox_ony{
	public:
		ListBox_ony();
		virtual void Add(const char *sText);
		virtual void Clear();
		virtual void Select(int n, Clovo *clovo);
		std::string ClovoPrintUTFS(int n,  std::string sn);

};

#endif