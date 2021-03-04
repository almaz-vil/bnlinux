/*
Поляков Дмитрий Владимирович <almaz-vil@list.ru>
25.01.2021
*/
#include "typee.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sstream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <linux/kd.h>
#include <linux/keyboard.h>

/*Очистить апплет*/
void ClearLog(){
	Log("Чистка апплета");
	std::string cmdf="truncate -s 0 /tmp/bnlinux_sv; echo -n \" \" >> /tmp/bnlinux_sv ";
    command_shell(cmdf.c_str());

}

void info(std::string s){
	Log("Вывод информации для апплета в файл /tmp/bnlinux_sv");
      std::string cmd="truncate -s 0 /tmp/bnlinux_sv; echo -n \"";
	  cmd.append(s);
      cmd.append(" \" >> /tmp/bnlinux_sv ");
    command_shell(cmd);
	
}
void info(arguments args){
	std::string ino="Активные режимы: ";
	if (args.bn) 
		ino.append("БН ");
	if (args.orfo) 
		ino.append("ОРФО ");
	if (args.shift) 
		ino.append("SHIFT ");
	if (args.znak) 
		ino.append("ЗНАК ");
	if (args.sound)
		ino.append("ЗК ");
	ino.append("; Для завершения нажмите <Pause> затем <F12>.");	
	info(ino);
}

/*Удаление одного символа т.е. press key <BASKSPACE>*/
void key_del(Display *display){
		Log("Программное нажатие клавиш для удаления символа");
		int kc;
 		kc = XKeysymToKeycode(display, 0xff08);
 		XTestFakeKeyEvent(display, kc, True, 0);
 		XFlush(display);
		kc = XKeysymToKeycode(display, 0xff08);
 		XTestFakeKeyEvent(display, kc, False, 0);
 		XFlush(display);
		
	}



void key_pavse(const char *cmdl,  Display *display){      
	
	if(system(cmdl)<1) {
	//	return;
	}

	Log("Программное нажатие клавиш");
 	int kc;
	kc = XKeysymToKeycode(display, 0xffe4);
	XTestFakeKeyEvent(display, kc, True, 0);
	XFlush(display);
	kc = XKeysymToKeycode(display, 0x0056);
	XTestFakeKeyEvent(display, kc, True, 0);
	XFlush(display);
	XTestFakeKeyEvent(display, kc, False, 0);
	XFlush(display);
	kc = XKeysymToKeycode(display, 0xffe4);
	XTestFakeKeyEvent(display, kc, False, 0);
	XFlush(display);


}


void command_shell(std::string s){
//Log("Активация commamd_shell");
	if(system(s.c_str())<1) {
	}
//Log("Деактивация commamd_shell");
}

void cp1251_to_utf8(char *str, char *res) {
    	static const long utf[ 256 ] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
		31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,
		59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,
		87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,
		111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,1026,1027,8218,
		1107,8222,8230,8224,8225,8364,8240,1033,8249,1034,1036,1035,1039,1106,8216,8217,
		8220,8221,8226,8211,8212,8250,8482,1113,8250,1114,1116,1115,1119,160,1038,1118,1032,
		164,1168,166,167,1025,169,1028,171,172,173,174,1031,176,177,1030,1110,1169,181,182,
		183,1105,8470,1108,187,1112,1029,1109,1111,1040,1041,1042,1043,1044,1045,1046,1047,
		1048,1049,1050,1051,1052,1053,1054,1055,1056,1057,1058,1059,1060,1061,1062,1063,
		1064,1065,1066,1067,1068,1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,
		1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,
		1096,1097,1098,1099,1100,1101,1102,1103
	};

	int cnt = strlen( str );
	int i = 0, j = 0;

	for(; i < cnt; ++i ) {
		long c = utf[ (unsigned char) str[ i ] ];
		if( c < 0x80 ) {
			res[ j++ ] = c;
		}
		else if( c < 0x800 ) {
			res[ j++ ] = c >> 6 | 0xc0;
			res[ j++ ] = c & 0x3f | 0x80;
		}
		else if( c < 0x10000 ) {
			res[ j++ ] = c >> 12 | 0xe0;
			res[ j++ ] = c >> 6 & 0x3f | 0x80;
			res[ j++ ] = c & 0x3f | 0x80;
		}
		else if( c < 0x200000 ) {
			res[ j++ ] = c >> 18 | 0xf0;
			res[ j++ ] = c >> 12 & 0x3f | 0x80;
			res[ j++ ] = c >> 6 & 0x3f | 0x80;
			res[ j++ ] = c & 0x3f | 0x80;
		}
	}
	res[j] = '\0';
}

//#define LOG
#define CPUTIME 
#ifndef LOG
#ifndef CPUTIME
static double U_CPU=0, S_CPU=0, ClockTime=0;
static long CLOCK;
#endif
void Log(std::string msg, bool error){
struct timeval tv;
gettimeofday(&tv,NULL);
const time_t* t=&tv.tv_sec;
struct tm *timefs=localtime(t);
struct tm gm;
gm=*timefs;
#ifndef CPUTIME
struct tms tsmm;
clock_t clockTime;
static long clockTicks=0;
if (clockTicks==0)
{
	clockTicks = sysconf(_SC_CLK_TCK);
}
clockTime=clock();
times(&tsmm);
double CT=(double)clockTime/CLOCKS_PER_SEC;
double CTR=CT-ClockTime;
ClockTime=CT;
double U_CP=(double)tsmm.tms_utime/clockTicks;
double U_CPUR=U_CP-U_CPU;
U_CPU=U_CP;
double S_CP=(double)tsmm.tms_stime/clockTicks;
double S_CPSR=S_CP-S_CPU;
S_CPU=S_CP;
long CTCLOK=(long)clockTime;
long CTCLOKR=CTCLOK-CLOCK;
CLOCK=CTCLOK;
#endif
std::stringstream ss;
ss << "\e[1;34m"<<msg<<"\e[0m\t\t\e[1;33m<"<<gm.tm_hour<<":"<<gm.tm_min<<":"<<gm.tm_sec<<">\e[0m";
std::string strResult = ss.str();
openlog("bnlinux",LOG_PERROR | LOG_PID,LOG_USER);
    #ifndef CPUTIME
    syslog(LOG_INFO,"\n%s\n\n\tclock=%ld\t(fun=%ld) per-sec(%.6f secs)\t(fun=%.6f secs)\n\tU_CPU: %.6f\t(fun=%.6f secs)\tsystem CPU: %.6f\t(fun=%.6f secs)", strResult.c_str(),
	CTCLOK,CTCLOKR,CT,CTR,U_CP,U_CPUR,S_CP,S_CPSR);
	#else
    syslog(LOG_INFO,"\n%s\n", strResult.c_str());
	#endif
    closelog();
}
#else
void Log(std::string msg, bool error){\

}
#endif

void Log1(std::string msg, bool error){\

openlog("bnlinux",LOG_PERROR | LOG_PID,LOG_USER);
    syslog(LOG_INFO,"\n%s", msg.c_str() );
    closelog();
}
void Log(std::string msg, int key, bool error){
   msg.append(" Key=<");
   msg.append(std::to_string(key));
   msg.append(">");
   Log(msg);
}
void Log1(std::string msg, int key, bool error){
   
   msg.append(std::to_string(key));
   
   Log1(msg);
}
