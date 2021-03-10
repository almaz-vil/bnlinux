/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
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
		bool claer=true;
	public:
		ListBox();
		int Add(const char *sText) override;
		void Clear() override;
		void ClearOn() override;
		bool Select(int n, Clovo *clovo) override;
		std::string ClovoPrintUTFS(int n,  std::string sn) override;
		
};

#endif