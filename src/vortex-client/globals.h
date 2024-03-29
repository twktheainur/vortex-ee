/*
 * File:   globals.h
 * Author: twk
 *
 * Created on January 10, 2009, 1:01 PM
 */
#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include "../common/Event.h"
#include <fstream>
#include <unistd.h>
#define XSTART -680
#define YSTART  160
#define ZSTART  127
#define XDIRSTART 1
#define YDIRSTART 0
extern Event connectionManagerInEvent;
extern Event connectionManagerOutEvent;
extern Event worldManagerEvent;
extern Event ogreManagerEvent;
extern std::ofstream flog;

extern string idClientGlobal;

extern bool continuer;
#endif


