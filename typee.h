/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
#ifndef TYPEE
#define TYPEE



#include <string>
#include <X11/Xlib.h>
struct arguments
{ 
   bool orfo; //Орфо, реакция на опечатку
   bool znak; //Реакция на знаки припенания
   bool shift; //Залипание Shift
  bool sound; //звуковая клавиатура
  bool bn;          // список слов и автозавершение
  bool kill;           // stop keylogger, -k switch
  bool us_keymap;      // use default US keymap, -u switch
  std::string keymap;       // user-specified keymap file, -m switch or --export-keymap
  std::string device;       // user-specified input event device, given with -d switch
 int flags;           // holds the following option flags
#define FLAG_NO_DAEMON       0x40  // don't daemonize process, stay in foreground, --no-daemon switch
};
enum Lang {RUS, EN};
void info(std::string s);
void info(arguments args);
void key_del(Display *display);
void key_pavse(const char *cmdl,  Display *display);
void cp1251_to_utf8(char *str, char *res);

#endif