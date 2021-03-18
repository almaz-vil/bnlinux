/*
  Copyleft (ɔ) 2009 Kernc
  This program is free software. It comes with absolutely no warranty whatsoever.
  See COPYING for further information.
  
  Project homepage: https://github.com/kernc/logkeys
Изменял Поляков Дмитрий Владимирович <almaz-vil@list.ru>
10.03.2021
*/

#include <cstdio>
#include <cerrno>
#include <cwchar>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <error.h>
#include <netdb.h>
#include <unistd.h>
#include <getopt.h>
#include <wctype.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <linux/input.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <linux/kd.h>
#include <linux/keyboard.h>
#include <unistd.h>

#include <getopt.h>

#include <cassert>
#include <signal.h>

#include<iostream>
#include <iostream>


#include <fcntl.h>
#include <linux/input.h>

#include <dirent.h>

#ifndef TYPEE
#include "typee.h"
#endif

#ifndef BNLINUX
#include "bnlinux.h"
#endif
#ifndef BNLINUX_ONY
#include "bnlinux_ony.h"
#endif
#ifndef SHIFT
#include "shift.h"
#endif
#ifndef SHIFT_ONY
#include "shift_ony.h"
#endif
#ifndef SOUND
#include "sound.h"
#endif
#ifndef SOUND_ONY
#include "sound_ony.h"
#endif
bnlinux * bnl=nullptr;
Shift_ony  * shift=nullptr;
Sound_ony * sound=nullptr;
bnlinux * bnl_temp=nullptr;
Shift  * shift_temp=nullptr;
Sound * sound_temp=nullptr;

#ifdef HAVE_CONFIG_H
# include <config.h>  // include config produced from ./configure
#endif

#ifndef  PACKAGE_VERSION
# define PACKAGE_VERSION "0.1.2"  // if PACKAGE_VERSION wasn't defined in <config.h>
#endif

// the following path-to-executable macros should be defined in config.h;
#ifndef  EXE_PS
# define EXE_PS "/bin/ps"
#endif

#ifndef  EXE_GREP
# define EXE_GREP "/bin/grep"
#endif

#ifndef  EXE_DUMPKEYS
# define EXE_DUMPKEYS "/opt/bnlinux/bin/dumpkeys"
//# define EXE_DUMPKEYS "/opt/bnlinux/bin/dumpkeys"
#endif

#define COMMAND_STR_DUMPKEYS ( EXE_DUMPKEYS " -n | " EXE_GREP " '^\\([[:space:]]shift[[:space:]]\\)*\\([[:space:]]altgr[[:space:]]\\)*keycode'" )
#define COMMAND_STR_GET_PID  ( (std::string(EXE_PS " ax | " EXE_GREP " '") + program_invocation_name + "' | " EXE_GREP " -v grep").c_str() )
#define COMMAND_STR_CAPSLOCK_STATE ("{ { xset q 2>/dev/null | grep -q -E 'Caps Lock: +on'; } || { setleds 2>/dev/null | grep -q 'CapsLock on'; }; } && echo on")
#define COMMAND_STR_RUS_EN_STATE ("{ { xset q 2>/dev/null | grep -q -E 'Group 2: +on'; } || { setleds 2>/dev/null | grep -q 'Group 2 on'; }; } && echo on")

//#define COMMAND_STR_PID		"ps -e | grep 'BNLinux'| grep  -v 'defunct' | awk '{print $1}'"

#define INPUT_EVENT_PATH  "/dev/input/"  // standard path
#define PID_FILE          "/var/run/bnlinux.pid"


#define LANG_ENG	102
#define LAND_RUS    101


//#include "keytables.cpp"  // character and function key tables and helper functions

struct arguments args = {0};  // default all args to 0x0 or ""

void usage();

void process_command_line_arguments(int argc, char **argv)
{
  struct option long_options[] = {
    {"bn",              no_argument,       0, 'b'},//Список слов (БН)
    {"orfo",            no_argument,       0, 'f'},//Орфо, реакция на опечатку
    {"sound",           no_argument,       0, 's'}, //Звуковая клавиатура
    {"shift",           no_argument,       0, 'p'},//Залипание Shift
    {"znak",            no_argument,       0, 'z'}, //Реакция на знаки припенания
    {"kill",            no_argument,       0, 'k'},
    {"device",          required_argument, 0, 'd'},
    {"name_device",     required_argument, 0, 'n'},
    {"help",            no_argument,       0, '?'},
    {"version",         no_argument,       0, 'v'},
    {0}
  };
  
  int c;
  int option_index;
  
  while ((c = getopt_long(argc, argv, "rbfspzkdn:?v", long_options, &option_index)) != -1)
  {
    switch (c) 
    {
      case 'r': args.reshim  = true;        break;  
      case 'b': args.bn  = true;            break;  
      case 'f': args.orfo = true;           break;
      case 's': args.sound = true;          break;
      case 'p': args.shift = true;          break;
      case 'z': args.znak = true;           break;
      case 'k': args.kill = true;           break;
      case 'n': {
                std::string s;
                s.append(optarg);
                struct dirent **namelist;
                int n= scandir("/dev/input/", &namelist, 0, alphasort);
                if (n < 0)
                    perror("scandir");
                else {
                    if(s.length()==0)   printf("*Список устройств*\n");         
                    while(n--) {
                      std::string namefile;
                      namefile.append(namelist[n]->d_name);
                      if (namefile.find("event")!=std::string::npos){
                        if(s.length()==0){
                              ListDriver(namelist[n]->d_name);
                              } 
                          else{
                            if(FindDriver(namelist[n]->d_name,optarg)){
                                std::string f="/dev/input/";
                                f.append(namelist[n]->d_name);
                                args.device.append(f);
                                break;   
                              }
                            }
                      }         
                    } 
                   if(s.length()==0)    printf("\n******************\n");  
                }      
                if (args.device.length()==0)
                  exit(EXIT_SUCCESS);
                break;}
      case 'd': args.device = optarg;       break;
      case '?': usage(); exit(EXIT_SUCCESS);
      case 'v': fprintf(stderr,"Version:0.0.7\n"); exit(EXIT_SUCCESS);
      default : usage(); exit(EXIT_FAILURE);
    }
  } // while
  
  while(optind < argc)
    error(0, 0, "Non-option argument %s", argv[optind++]);
}

void usage()
{
  fprintf(stderr,
"Usage: bnlinux [OPTION]...\n"
"bnlinux\n"
"Для выхода необходимо нажать <Pause> и затем <F12>\nвкл/выкл режимов ЗНАК-<F9> SHIFT-<F8> ОРФО-<F7> ЗК-<F6> БН-<F5>\n\n"
"  -b,\tактивация режима автозавершение слов (БН)\n"
"  -f,\tактивация режима опечатка (ОРФО)\n"
"  -s,\tактивация режима звуковая клавиатура (ЗК)\n"
"  -p,\tактивация режима залипания <Shift>(ввода знаков ! и т.д.) (SHIFT)\n"
"  -z,\tактивация режима реакция на .!? (ЗНАК)\n"  
"  -r,\tВключение переключение режимов без перезапуска"
"  -k,\tзавершает программу bnlinux\n"
"  -d,\tустройство ввода\n"
"  -n,\tимя устройство ввода или -n\"\" вывод списка устройств ввода\n"
"  -?,\tэта справка пробнее (man bnlinux)\n"
  );
}

bool shift_flag = false;
Display *display=nullptr;
int PID=-1;
namespace logkeys {
//*********************************
// these are ordered default US keymap keys
wchar_t char_keys[49] =  L"1234567890-=qwertyuiop[]asdfghjkl;'`\\zxcvbnm,./<";
wchar_t shift_keys[49] = L"!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\"~|ZXCVBNM<>?>";
wchar_t altgr_keys[49] = {0}; // old, US don't use AltGr key: L"\0@\0$\0\0{[]}\\\0qwertyuiop\0~asdfghjkl\0\0\0\0zxcvbnm\0\0\0|";  // \0 on no symbol; as obtained by `loadkeys us`
// TODO: add altgr_shift_keys[] (http://en.wikipedia.org/wiki/AltGr_key#US_international)

wchar_t func_keys[][8] = {
  L"<Esc>", L"<BckSp>", L"<Tab>", L"<Enter>", L"<LCtrl>", L"<LShft>", L"<RShft>", L"<KP*>", L"<LAlt>", L" ", L"<CpsLk>", L"<F1>", L"<F2>", L"<F3>", L"<F4>", L"<F5>",
  L"<F6>", L"<F7>", L"<F8>", L"<F9>", L"<F10>", L"<NumLk>", L"<ScrLk>", L"<KP7>", L"<KP8>", L"<KP9>", L"<KP->", L"<KP4>", L"<KP5>", L"<KP6>", L"<KP+>", L"<KP1>",
  L"<KP2>", L"<KP3>", L"<KP0>", L"<KP.>", /*"<",*/ L"<F11>", L"<F12>", L"<KPEnt>", L"<RCtrl>", L"<KP/>", L"<PrtSc>", L"<AltGr>", L"<Break>" /*linefeed?*/, L"<Home>", L"<Up>", L"<PgUp>", 
  L"<Left>", L"<Right>", L"<End>", L"<Down>", L"<PgDn>", L"<Ins>", L"<Del>", L"<Pause>", L"<LMeta>", L"<RMeta>", L"<Menu>"
};

const char char_or_func[] =  // c = character key, f = function key, _ = blank/error ('_' is used, don't change); all according to KEY_* defines from <linux/input.h>
  "_fccccccccccccff"
  "ccccccccccccffcc"
  "ccccccccccfccccc"
  "ccccccffffffffff"
  "ffffffffffffffff"
  "ffff__cff_______"
  "ffffffffffffffff"
  "_______f_____fff";
#define N_KEYS_DEFINED 106  // sum of all 'c' and 'f' chars in char_or_func[]

inline bool is_char_key(unsigned int code)
{
  assert(code < sizeof(char_or_func));
  return (char_or_func[code] == 'c');
}

inline bool is_func_key(unsigned int code)
{
  assert(code < sizeof(char_or_func));
  return (char_or_func[code] == 'f');
}

inline bool is_used_key(unsigned int code)
{
  assert(code < sizeof(char_or_func));
  return (char_or_func[code] != '_');
}

// translates character keycodes to continuous array indexes
inline int to_char_keys_index(unsigned int keycode)
{
  if (keycode >= KEY_1 && keycode <= KEY_EQUAL)  // keycodes 2-13: US keyboard: 1, 2, ..., 0, -, =
    return keycode - 2;
  if (keycode >= KEY_Q && keycode <= KEY_RIGHTBRACE)  // keycodes 16-27: q, w, ..., [, ]
    return keycode - 4;
  if (keycode >= KEY_A && keycode <= KEY_GRAVE)  // keycodes 30-41: a, s, ..., ', `
    return keycode - 6;
  if (keycode >= KEY_BACKSLASH && keycode <= KEY_SLASH)  // keycodes 43-53: \, z, ..., ., /
    return keycode - 7;
  
  if (keycode == KEY_102ND) return 47;  // key right to the left of 'Z' on US layout
  
  return -1;  // not character keycode
}

// translates function keys keycodes to continuous array indexes
inline int to_func_keys_index(unsigned int keycode)
{
  if (keycode == KEY_ESC)  // 1
    return 0;
  if (keycode >= KEY_BACKSPACE && keycode <= KEY_TAB)  // 14-15
    return keycode - 13;
  if (keycode >= KEY_ENTER && keycode <= KEY_LEFTCTRL)  // 28-29
    return keycode - 25;
  if (keycode == KEY_LEFTSHIFT) return keycode - 37;  // 42
  if (keycode >= KEY_RIGHTSHIFT && keycode <= KEY_KPDOT)  // 54-83
    return keycode - 48;
  if (keycode >= KEY_F11 && keycode <= KEY_F12)  // 87-88
    return keycode - 51;
  if (keycode >= KEY_KPENTER && keycode <= KEY_DELETE)  // 96-111
    return keycode - 58;
  if (keycode == KEY_PAUSE)  // 119
    return keycode - 65;
  if (keycode >= KEY_LEFTMETA && keycode <= KEY_COMPOSE)  // 125-127
    return keycode - 70;
  
  return -1;  // not function key keycode
}

//******************************************


struct key_state_t {
  wchar_t key;
  unsigned int repeats;  // count_repeats differs from the actual number of repeated characters! afaik, only the OS knows how these two values are related (by respecting configured repeat speed and delay)
  bool repeat_end;
  input_event event;
  bool scancode_ok;
  bool capslock_in_effect = false;
  bool shift_in_effect = false;
  bool altgr_in_effect = false;
  bool ctrl_in_effect = false;  // used for identifying Ctrl+C / Ctrl+D
} key_state;
void kill_existing_process();

// executes cmd and returns string ouput
std::string execute(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
      error(EXIT_FAILURE, errno, "Pipe error");
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    pclose(pipe);
    return result;
}

void Resim(arguments args){

  if(args.shift){
    shift= new Shift();
    }
  else{
    shift= new Shift_ony(); 
    }
  if (args.sound){
    sound= new Sound();
    }
  else{
    sound= new Sound_ony();  
  }
  bnl->Reshim(args);
  info(args);
}

// Русский язык да или нет
Lang Rus_or_En()
{
 // Log("Узнаём текущий язык");	    
  return execute(COMMAND_STR_RUS_EN_STATE).size() >= 2 ? RUS:EN;
 
}
// Отправка сигнала номер введенного символа
void SendKeySignal(int key){
 // Log("Вызов sound->play");	    
	sound->play(key, Rus_or_En());
  if(shift_flag||(key==20)) return; //Вставка знака препинания или Caps Lock
//  Log("Вызов bnl->Print");	    
  bnl->Print(key, Rus_or_En());
}

int input_fd = -1;  // input event device file descriptor; global so that signal_handler() can access it

void signal_handler(int signal)
{
  if (input_fd != -1)
    close(input_fd);  // closing input file will break the infinite while loop
}

void set_utf8_locale()
{
  // set locale to common UTF-8 for wchars to be recognized correctly
  if(setlocale(LC_CTYPE, "en_US.UTF-8") == NULL) { // if en_US.UTF-8 isn't available
    char *locale = setlocale(LC_CTYPE, "");  // try the locale that corresponds to the value of the associated environment variable LC_CTYPE
    if (locale != NULL && 
        (strstr(locale, "UTF-8") != NULL || strstr(locale, "UTF8") != NULL ||
         strstr(locale, "utf-8") != NULL || strstr(locale, "utf8") != NULL) )
      ;  // if locale has "UTF-8" in its name, it is cool to do nothing
    else
      error(EXIT_FAILURE, 0, "LC_CTYPE locale must be of UTF-8 type, or you need en_US.UTF-8 availabe");
  }
}


void create_PID_file()
{
  // create temp file carrying PID for later retrieval
  int pid_fd = open(PID_FILE, O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (pid_fd != -1) {
    char pid_str[16] = {0};
    sprintf(pid_str, "%d", getpid());
    if (write(pid_fd, pid_str, strlen(pid_str)) == -1)
      error(EXIT_FAILURE, errno, "Error writing to PID file '" PID_FILE "'");
    close(pid_fd);
  }
  else {
    if (errno == EEXIST)  // This should never happen
         error(EXIT_FAILURE, errno, "Another process already running? Quitting. (" PID_FILE ")");
    else error(EXIT_FAILURE, errno, "Error opening PID file '" PID_FILE "'");
  }
}

void kill_existing_process()
{
  Log("Завершение программы");    
  pid_t pid;
  bool via_file = true;
  bool via_pipe = true;
  FILE *temp_file = fopen(PID_FILE, "r");
  
  via_file &= (temp_file != NULL);
  
  if (via_file) {  // kill process with pid obtained from PID file
    via_file &= (fscanf(temp_file, "%d", &pid) == 1);
    fclose(temp_file);
  }
  
  if (!via_file) {  // if reading PID from temp_file failed, try ps-grep pipe
    via_pipe &= (sscanf(execute(COMMAND_STR_GET_PID).c_str(), "%d", &pid) == 1);
    via_pipe &= (pid != getpid());
  }
  
  if (via_file || via_pipe) {
    remove(PID_FILE);
    info("Работа bnlinux заверщена!");
    kill(pid, SIGINT);
    exit(EXIT_SUCCESS);  // process killed successfully, exit

  }
  error(EXIT_FAILURE, 0, "No process killed");
}

void set_signal_handling()
{ // catch SIGHUP, SIGINT and SIGTERM signals to exit gracefully
  struct sigaction act = {{0}};
  act.sa_handler = signal_handler;
  sigaction(SIGHUP,  &act, NULL);
  sigaction(SIGINT,  &act, NULL);
  sigaction(SIGTERM, &act, NULL);
  // prevent child processes from becoming zombies
  act.sa_handler = SIG_IGN;
  sigaction(SIGCHLD, &act, NULL);
}

void determine_command_shell_keymap()
{
  // custom map will be used; erase existing US keymapping
  memset(char_keys,  '\0', sizeof(char_keys));
  memset(shift_keys, '\0', sizeof(shift_keys));
  memset(altgr_keys, '\0', sizeof(altgr_keys));
  
  // get keymap from dumpkeys
  // if one knows of a better, more portable way to get wchar_t-s from symbolic keysym-s from `dumpkeys` or `xmodmap` or another, PLEASE LET ME KNOW! kthx
  std::stringstream ss, dump(execute(COMMAND_STR_DUMPKEYS));  // see example output after i.e. `loadkeys slovene`
  std::string line;

  unsigned int i = 0;   // keycode
  int index;
  int utf8code;      // utf-8 code of keysym answering keycode i
  
  while (std::getline(dump, line)) {
    ss.clear();
    ss.str("");
    utf8code = 0;
    
    // replace any U+#### with 0x#### for easier parsing
    index = line.find("U+", 0);
    while (static_cast<std::string::size_type>(index) != std::string::npos) {
      line[index] = '0'; line[index + 1] = 'x';
      index = line.find("U+", index);
    }
    
    if (++i >= sizeof(char_or_func)) break;  // only ever map keycodes up to 128 (currently N_KEYS_DEFINED are used)
    if (!is_char_key(i)) continue;  // only map character keys of keyboard
    
    assert(line.size() > 0);
    if (line[0] == 'k') {  // if line starts with 'keycode'
      index = to_char_keys_index(i);
      
      ss << &line[14];  // 1st keysym starts at index 14 (skip "keycode XXX = ")
      ss >> std::hex >> utf8code;
      // 0XB00CLUELESS: 0xB00 is added to some keysyms that are preceeded with '+'; I don't really know why; see `man keymaps`; `man loadkeys` says numeric keysym values aren't to be relied on, orly?
      if (line[14] == '+' && (utf8code & 0xB00)) utf8code ^= 0xB00; 
      char_keys[index] = static_cast<wchar_t>(utf8code);
      
      // if there is a second keysym column, assume it is a shift column
      if (ss >> std::hex >> utf8code) {
        if (line[14] == '+' && (utf8code & 0xB00)) utf8code ^= 0xB00;
        shift_keys[index] = static_cast<wchar_t>(utf8code);
      }
      
      // if there is a third keysym column, assume it is an altgr column
      if (ss >> std::hex >> utf8code) {
        if (line[14] == '+' && (utf8code & 0xB00)) utf8code ^= 0xB00;
        altgr_keys[index] = static_cast<wchar_t>(utf8code);
      }
      
      continue;
    }
    
    // else if line starts with 'shift i'
    index = to_char_keys_index(--i);
    ss << &line[21];  // 1st keysym starts at index 21 (skip "\tshift\tkeycode XXX = " or "\taltgr\tkeycode XXX = ")
    ss >> std::hex >> utf8code;
    if (line[21] == '+' && (utf8code & 0xB00)) utf8code ^= 0xB00;  // see line 0XB00CLUELESS
    
    if (line[1] == 's')  // if line starts with "shift"
      shift_keys[index] = static_cast<wchar_t>(utf8code);
    if (line[1] == 'a')  // if line starts with "altgr"
      altgr_keys[index] = static_cast<wchar_t>(utf8code);
  } // while (getline(dump, line))
}

void determine_input_device()
{
  // better be safe than sory: while running other programs, switch user to nobody
  if(setegid(65534)==-1){return;}
  if(seteuid(65534)==-1){return;}
  
  // extract input number from /proc/bus/input/devices (I don't know how to do it better. If you have an idea, please let me know.)
  // The compiler automatically concatenates these adjacent strings to a single string.
  const char* cmd = EXE_GREP " -E 'Handlers|EV=' /proc/bus/input/devices | "
    EXE_GREP " -B1 'EV=1[02]001[3Ff]' | "
    EXE_GREP " -Eo 'event[0-9]+' ";
  std::stringstream output(execute(cmd));
  
  std::vector<std::string> results;
  std::string line;
  
  while(std::getline(output, line)) {
    std::string::size_type i = line.find("event");
    if (i != std::string::npos) i += 5; // "event".size() == 5
    if (i < line.size()) {
      int index = atoi(&line.c_str()[i]);
      
      if (index != -1) {
        std::stringstream input_dev_path;
        input_dev_path << INPUT_EVENT_PATH;
        input_dev_path << "event";
        input_dev_path << index;

        results.push_back(input_dev_path.str());
      }
    }
  }
  
  if (results.size() == 0) {
    error(0, 0, "Couldn't determine keyboard device. :/");
    error(EXIT_FAILURE, 0, "Please post contents of your /proc/bus/input/devices file as a new bug report. Thanks!");
  }

  args.device = results[0];  // for now, use only the first found device
  
  // now we reclaim those root privileges
  if(setegid(0)==-1){return;}
  if(seteuid(0)==-1){return;}
  
}

bool update_key_state()
{
// these event.value-s aren't defined in <linux/input.h> ?
#define EV_MAKE   1  // when key pressed
#define EV_BREAK  0  // when key released
#define EV_REPEAT 2  // when key switches to repeating after short delay

  if (read(input_fd, &key_state.event, sizeof(struct input_event)) <= 0) {
    return false;
  }
  if (key_state.event.type != EV_KEY)
    return update_key_state();  // keyboard events are always of type EV_KEY

  unsigned short scan_code = key_state.event.code; // the key code of the pressed key (some codes are from "scan code set 1", some are different (see <linux/input.h>)
  
  key_state.repeat_end = false;
  if (key_state.event.value == EV_REPEAT) {
    key_state.repeats++;
    return true;
  }
  else if (key_state.event.value == EV_BREAK) {
    if (scan_code == KEY_LEFTSHIFT || scan_code == KEY_RIGHTSHIFT)
      key_state.shift_in_effect = false;
    else if (scan_code == KEY_RIGHTALT)
      key_state.altgr_in_effect = false;
    else if (scan_code == KEY_LEFTCTRL || scan_code == KEY_RIGHTCTRL)
      key_state.ctrl_in_effect = false;

    key_state.repeat_end = key_state.repeats > 0;
    if (key_state.repeat_end)
      return true;
    else
      return update_key_state();
  }
  key_state.repeats = 0;

  key_state.scancode_ok = scan_code < sizeof(char_or_func);
  if (!key_state.scancode_ok)
    return true;

  key_state.key = 0;

  if (key_state.event.value != EV_MAKE)
    return update_key_state();

  switch (scan_code) {
  case KEY_CAPSLOCK:
    key_state.capslock_in_effect = !key_state.capslock_in_effect;
    break;
  case KEY_LEFTSHIFT:
  case KEY_RIGHTSHIFT:
    key_state.shift_in_effect = true;
    break;
  case KEY_RIGHTALT:
    key_state.altgr_in_effect = true;
    break;
  case KEY_LEFTCTRL:
  case KEY_RIGHTCTRL:
    key_state.ctrl_in_effect = true;
    break;
  default:
    if (is_char_key(scan_code)) {
      wchar_t wch;
      if (key_state.altgr_in_effect) {
        wch = altgr_keys[to_char_keys_index(scan_code)];
        if (wch == L'\0') {
          if(key_state.shift_in_effect)
            wch = shift_keys[to_char_keys_index(scan_code)];
          else
            wch = char_keys[to_char_keys_index(scan_code)];
        }
      }

      else if (key_state.capslock_in_effect && iswalpha(char_keys[to_char_keys_index(scan_code)])) { // only bother with capslock if alpha
        if (key_state.shift_in_effect) // capslock and shift cancel each other
          wch = char_keys[to_char_keys_index(scan_code)];
        else
          wch = shift_keys[to_char_keys_index(scan_code)];
        if (wch == L'\0')
          wch = char_keys[to_char_keys_index(scan_code)];
      }

      else if (key_state.shift_in_effect) {
        wch = shift_keys[to_char_keys_index(scan_code)];
        if (wch == L'\0')
          wch = char_keys[to_char_keys_index(scan_code)];
      }
      else  // neither altgr nor shift are effective, this is a normal char
        wch = char_keys[to_char_keys_index(scan_code)];

      key_state.key = wch; 
	  SendKeySignal(wch);

    }
    
  }
  return true;
}

// returns output file in case a new one was created so caller can close it properly
void log_loop()
{
  while (update_key_state()) {
	unsigned short scan_code = key_state.event.code;
	switch (scan_code) {
	  case KEY_PAUSE:{
      Log("Press: ", scan_code);  
	    if(shift_flag) {shift_flag=false;Log("Деактивирован флажок shift_flag");}
	    else{ 
        shift->On();		
    		shift_flag=true;		 
        Log("Активирован флажок shift_flag");
        SendKeySignal(12);
        std::string s="Для выхода нажмите <F12>.";
        if(args.reshim){
          s.append(" Или режимы:");
          if (args.sound)s.append(" ЗК(выкл)<F6>,");
          else s.append(" ЗК(вкл)<F6>,");
          if(args.bn)s.append(" БН(выкл)<F5>,");
          else s.append(" БН(вкл)<F5>,");
          if(args.znak)s.append(" ЗНАК(выкл)<F9>,");
          else s.append(" ЗНАК(вкл)<F9>,");
          if(args.orfo)s.append(" ОРФО(выкл)<F7>,");
          else s.append(" ОРФО(вкл)<F7>,");
          if(args.shift)s.append(" SHIFT(выкл)<F8>.  Продолжайте работу <1>..<0>");
          else s.append(" SHIFT(вкл)<F8>");
        }
        else{
          if(args.shift)s.append(" Продолжайте работу <1>..<0>");
          else s.append("Режим Shift не активен.(параметр -р для активации)");          
        }
        
        info(s);}
		  break;}
	  case KEY_BACKSPACE:{
		  SendKeySignal(8);
	  	break;}
	  case KEY_CAPSLOCK:{
      SendKeySignal(20);
      break;}
	  case KEY_ESC:{
		  SendKeySignal(27);
      break;}
    case KEY_F10:{
      break;
    }  
	  case KEY_TAB:{
		  SendKeySignal(9);
	  	break;}
	  case KEY_ENTER:{
		  SendKeySignal(13);
		  break;}
	  case KEY_SPACE:{
		  SendKeySignal(32);
	  	break;}
    case KEY_F4:{
      Log("Press: ", scan_code);  
	    bnl->Add();
		  break;}
    case KEY_F5:{
      Log("Press: ", scan_code);  
	    if((shift_flag)&&(args.reshim)){
        shift_flag=false;
        if(args.bn) args.bn=false;
	      else args.bn=true;
        Resim(args);
        if(args.bn)command_shell("play /opt/bnlinux/local/sound/info/bn_on.wav 2>/dev/null");				
        else command_shell("play /opt/bnlinux/local/sound/info/bn_off.wav 2>/dev/null");				
      }
		  break;}
    case KEY_F6:{
      Log("Press: ", scan_code);  
      if((shift_flag)&&(args.reshim)){
        shift_flag=false;
        if(args.sound) args.sound=false;
	      else args.sound=true;
        Resim(args);
        if(args.sound)command_shell("play /opt/bnlinux/local/sound/info/sound_on.wav 2>/dev/null");				
        else command_shell("play /opt/bnlinux/local/sound/info/sound_off.wav 2>/dev/null");				
       }
		  break;}
    case KEY_F7:{
      Log("Press: ", scan_code);  
	    if((shift_flag)&&(args.reshim)){
        shift_flag=false;
        if(args.orfo) args.orfo=false;
	      else args.orfo=true;
        Resim(args);
        if(args.orfo)command_shell("play /opt/bnlinux/local/sound/info/opfo_on.wav 2>/dev/null");				
        else command_shell("play /opt/bnlinux/local/sound/info/opfo_off.wav 2>/dev/null");				
        }
		  break;}
    case KEY_F8:{
      Log("Press: ", scan_code);  
      if((shift_flag)&&(args.reshim)){
        shift_flag=false;
        if(args.shift) args.shift=false;
	      else args.shift=true;
        Resim(args);
        if(args.shift)command_shell("play /opt/bnlinux/local/sound/info/shift_on.wav 2>/dev/null");				
        else command_shell("play /opt/bnlinux/local/sound/info/shift_off.wav 2>/dev/null");				  
      }
		  break;}
    case KEY_F9:{
      if((shift_flag)&&(args.reshim)){
        shift_flag=false;
        if(args.znak) args.znak=false;
	      else args.znak=true;
        Resim(args);
        if(args.znak)command_shell("play /opt/bnlinux/local/sound/info/znaki_on.wav 2>/dev/null");				
        else command_shell("play /opt/bnlinux/local/sound/info/znaki_of.wav 2>/dev/null");				  
      }
		  break;}
  	case KEY_F12:{
      Log("Press: ", scan_code);  
	     if(shift_flag){
        command_shell("play /opt/bnlinux/local/sound/info/exit.wav 2>/dev/null");				
        kill_existing_process(); 
      }
      else
      {
        info("Для выхода нажмите <Pause> затем <F12>");
      }
		  break;}
    default:
      if(shift_flag){
        shift_flag=false;
        shift->Reshim(key_state.key, bnl);
      }
	
	}
 }  
}


int main(int argc, char **argv)
{  
 

  process_command_line_arguments(argc, argv);
  
  // kill existing logkeys process
  if (args.kill) kill_existing_process();
  if (!(args.shift)&&!(args.sound)&&!(args.bn)&&!(args.orfo)&&!(args.znak)) {usage(); exit(0);}
  if (geteuid()) error(EXIT_FAILURE, errno, "Для работы необходимы права (sudo)! sudo bnlinux");
 
  Log("Запуск",true);  
  if(args.shift)
    shift= new Shift();
  else
    shift= new Shift_ony(); 
  
  if (args.sound)
    sound= new Sound();
  else
    sound= new Sound_ony();  

    bnl= new bnlinux(args);

  info(args);

  
    int noclose = 1;  // don't close streams (stderr used)
    if (daemon(0, noclose) == -1)  // become daemon
      error(EXIT_FAILURE, errno, "Failed to become daemon");
  if(setegid(0)==-1){    error(EXIT_FAILURE, errno, "Failed to setegid 0");}
  if(seteuid(0)==-1){    error(EXIT_FAILURE, errno, "Failed to seteuid 0");}
  
    create_PID_file();
  
 
  
  //--------------------------------------------------
   set_utf8_locale();

    determine_command_shell_keymap();
  
  if (args.device.empty()) {  // no device given with -d switch
    determine_input_device();
  } 
  else {  // event device supplied as -d argument
    std::string::size_type i = args.device.find_last_of('/');
    args.device = (std::string(INPUT_EVENT_PATH) + args.device.substr(i == std::string::npos ? 0 : i + 1));
  }
  
  set_signal_handling();
  
  close(STDIN_FILENO);
  // open input device for reading
  input_fd = open(args.device.c_str(), O_RDONLY);
  if (input_fd == -1) {
    error(EXIT_FAILURE, errno, "Error opening input event device '%s'", args.device.c_str());
  }
  key_state.capslock_in_effect = execute(COMMAND_STR_CAPSLOCK_STATE).size() >= 2;

  log_loop();

  remove(PID_FILE);
  
  exit(EXIT_SUCCESS);
} // main()

} // namespace logkeys

int main(int argc, char** argv)
{
  return logkeys::main(argc, argv);
}

