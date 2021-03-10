/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
#ifndef SOUND_ONY
#define SOUND_ONY


#ifndef TYPEE
#include "typee.h"
#endif

class Sound_ony{
    private:
    public:
        Sound_ony();
        virtual void play(int key, Lang lang);

};

#endif