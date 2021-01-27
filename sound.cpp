/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
//*********************
//**Звуковая клавиатура
//*********************

#include <string.h>
#include <fstream>

#include "sound.h"

#ifndef TYPEE
#include "typee.h"
#endif

Sound::Sound()
{
}


void Sound::play(int n, Lang lang){
 std::string cmd_temp;
 cmd_temp.append(this->cmd);   
 if ((n>96) && (n<123)) n=n-32;
if ((lang==EN)&&(((n>64) && (n<91)))){        
    cmd_temp.append("ang");
    }
else {
    if ((n==91) || (n==123)) n=219;
    if ((n==93) || (n==125)) n=221;
    if ((n==59) || (n==58)) n=186;
    if ((n==46) || (n==62)) n=190;  
    if ((n==44) || (n==60)) n=188;
    if ((n==39) || (n==34)) n=222;
    if ((n==96) || (n==126)) n=192;
    if (n==63) n=44;
    if (n==47) n=46;
    if (n==38) n=39;//?
    if (n==94) n=38;//:
    if (n==64) n=34;//"
    }    
cmd_temp.append(std::to_string(n));
cmd_temp.append(this->cmdend);
system(cmd_temp.c_str());

}