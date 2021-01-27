/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
#ifndef LIST_BOX
#define LIST_BOX

#include <string.h>
#include <X11/Xlib.h>
#ifndef LIST_BOX_ONY
#include "listbox_ony.h"
#endif

#ifndef CLOVO
#include "clovo.h"
#endif

class ListBox:public ListBox_ony
 {
	private:
		std::string ListClov[7] = {};
		int count_item_label=0;
		Display *display=nullptr;
	public:
		ListBox();
		void Add(const char *sText) override;
		void Clear() override;
		void Select(int n, Clovo *clovo) override;
		std::string ClovoPrintUTFS(int n,  std::string sn);
};

#endif