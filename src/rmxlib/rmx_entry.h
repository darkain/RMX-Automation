/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_ENTRY_H_
#define _RMX_ENTRY_H_


#include "../../sdk/rmxMsg.h"


#define RMXCALL __cdecl

extern "C" {
  typedef void __declspec(dllexport) (RMXCALL* RMX_ENTRY)(vMSG*);
  extern  void __declspec(dllexport)  RMXCALL _RMX_CORE(  vMSG *msg);
  extern  void __declspec(dllexport)  RMXCALL _RMX_ENTRY( vMSG *msg);
}


#endif //_RMX_ENTRY_H_
