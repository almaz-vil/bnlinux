/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
#pragma once
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
		virtual int Add(const char *sText);
		virtual void Clear();
		virtual void ClearOn();
		virtual bool Select(int n, Clovo *clovo);
		virtual std::string ClovoPrintUTFS(int n,  std::string sn);

};

#endif