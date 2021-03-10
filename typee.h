/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
02.02.2021
*/
#ifndef TYPEE
#define TYPEE


#include <string>
#include <X11/Xlib.h>
#include <syslog.h>

struct arguments
{ 
   bool reshim; //Включение переключение режимов без перезапуска
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
/**/
void UnicodeB(Display *dpy);
void UnicodeTochka(Display *dpy);
void UnicodeVosklik(Display *dpy);
void UnicodeVopros(Display *dpy);
void UnicodeProbel(Display *dpy);
void info(std::string s);
void command_shell(std::string s);
void info(arguments args);
void key_del(Display *display);
//void key_pavse(const char *cmdl,  Display *display);
void press_keys(const char* string,  Display *display);
void xtest_key_press(unsigned char letter, Display *dpy);
void cp1251_to_utf8(char *str, char *res);
void ClearLog();
void Log(const std::string msg, bool error=false);
void Log1(const std::string msg, bool error=false);
void Log(std::string msg, int key, bool error=false);
void Log1(std::string msg, int key, bool error=false);


#endif