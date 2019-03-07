/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_MESSAGE_H_
#define _CFG_MESSAGE_H_


typedef enum {
  GENERIC_NONE         = 0x0000,
  GENERIC_CLICK,
  GENERIC_GAIN_FOCUS,
  GENERIC_LOST_FOCUS,
  GENERIC_OPEN,
  GENERIC_CLOSE,
  GENERIC_PAINT,

  MENU_START           = 0x0080,
  MENU_MASTER,
  MENU_POP,

  CONSOLE_START        = 0x0100,
  CONSOLE_CLEAR,
  CONSOLE_PRINT,
  CONSOLE_GRAB,
  CONSOLE_RELEASE,

  FUNCTION_START       = 0x0120,
  FUNCTION_ACTIVATE,
  FUNCTION_GET_NAME,
  FUNCTION_STOP,
  FUNCTION_DEPTH,

  EVENT_START          = 0x0140,
  EVENT_ACTIVATE,
  EVENT_GET_NAME,
  EVENT_OPEN_GUI,
  EVENT_CLOSE_GUI,

  GENERIC_USER         = 0x1000,
} cfgMessage;


#endif //_CFG_MESSAGE_H_
