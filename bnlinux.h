/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
#pragma once
#ifndef BNLINUX
#define BNLINUX

#include <string>
#include <vector>

#ifndef LIST_BOX_ONY
#include "listbox_ony.h"
#endif
#ifndef CLOVO
#include "clovo.h"
#endif
#ifndef TYPEE
#include "typee.h"
#endif
#ifndef BNLINUX_ONY
#include "bnlinux_ony.h"
#endif
#ifndef ZNAK_ONY
#include "znaki_ony.h"
#endif




class bnlinux: public bnlinux_ony
{
private:
    ListBox_ony * listbox=nullptr;
    Clovo * clovo=nullptr;
    Znak_ony * znak=nullptr;
     int  orfo;
    std::vector <std::string> listrus;
    int Find();
public:
    bnlinux(struct arguments args);
    ~bnlinux();
    void Print(int key, Lang lang = RUS) override;
    void Reshim(struct arguments args);
    void Add() ;
};


#endif 