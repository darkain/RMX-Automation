/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __HCONSOLE_H_
#define __HCONSOLE_H_


#include "../rmxTypes.h"


/*
Don't directly create instances of this class.  Use the instances
provided to you from the functions listed in "RMX DLL.h"
*/

class vHCONSOLE {
  private:
    vHCONSOLE() {}
    ~vHCONSOLE() {}

  public:
    void print(       const char *text);
    void print(       const char *name, const char *text);
    void printWarning(const char *name, const char *text);
    void printError(  const char *name, const char *text);
    void printDebug(  const char *name, const char *text);
    
    inline void newLine()  { print("\r\n"); }
    inline void sameLine() { print("\r"  ); }

    void clear();

    HPLUGIN getPlugin();
    HLIST   getList();


  public:
    static HCONSOLE getFirst();
    static HCONSOLE getLast();
    static HCONSOLE getPrev(HCONSOLE console);
    static HCONSOLE getNext(HCONSOLE console);
    static inline HCONSOLE getMain()      { return CONSOLE_MAIN;      }
    static inline HCONSOLE getActive()    { return CONSOLE_ACTIVE;    }
    static inline HCONSOLE getBroadcast() { return CONSOLE_BROADCAST; }
};



/*
[1;30m COMBINED!

[0m    reset; clears all colors and styles (to white on black)
[1m    bold on (see below)
[3m    italics on
[4m    underline on
[7m    inverse on; reverses foreground & background colors
[9m    strikethrough on
[22m   bold off (see below)
[23m   italics off
[24m   underline off
[27m   inverse off
[29m   strikethrough off
[30m   set foreground color to black
[31m   set foreground color to red
[32m   set foreground color to green
[33m   set foreground color to yellow
[34m   set foreground color to blue
[35m   set foreground color to magenta (purple)
[36m   set foreground color to cyan
[37m   set foreground color to white
[39m   set foreground color to default (white)
[40m   set background color to black
[41m   set background color to red
[42m   set background color to green
[43m   set background color to yellow
[44m   set background color to blue
[45m   set background color to magenta (purple)
[46m   set background color to cyan
[47m   set background color to white
[49m   set background color to default (black)

[r;cH  Position cursor at row r and column c 
[nA    Move cursor n rows up 
[nB    Move cursor n rows down 
[nC    Move cursor n columns forward (right) 
[nD    Move cursor n columns back (left) 
[6n    Show current cursor position 
[s     Save current cursor position 
[u     Restore previously stored cursor position 

[2J    Clear screen 
[K     Clear to end of line

[=7l   disable wrap 
[=7h   enable wrap 
*/

#endif //__HCONSOLE_H_
