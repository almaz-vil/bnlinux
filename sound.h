/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/
#ifndef SOUND
#define SOUND


#ifndef TYPEE
#include "typee.h"
#endif
#ifndef SOUND_ONY
#include "sound_ony.h"
#endif


class Sound: public Sound_ony{
    private:
        std::string cmd="play /opt/bnlinux/local/sound/";
        std::string cmdend=".wav 2>/dev/null";
        //std::string cmdend=".wav";
    public:
        Sound();
        void play(int key, Lang lang) override;

};

#endif