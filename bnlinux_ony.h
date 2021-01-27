/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
#ifndef BNLINUX_ONY
#define BNLINUX_ONY

#ifndef TYPEE
#include "typee.h"
#endif


class bnlinux_ony
{
private:
    /* data */
public:
    bnlinux_ony();
    ~bnlinux_ony();
   virtual void Print(int key, Lang lang = RUS);
   
};


#endif 