/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/handles/hconsole.h"
#include "rmx_msg.h"


void vHCONSOLE::print(const char *text) {
  RmxNul( (VMSG::SYSTEM, 304, text, this) );
}


void vHCONSOLE::print(const char *name, const char *text) {
  char str[1024] = "";
  VPRINTF(str, sizeof(str), "\033[1;36m[%s]\033[37m %s", name, text);
  print(str);
}


void vHCONSOLE::printWarning(const char *name, const char *text) {
  char str[1024] = "";
  VPRINTF(str, sizeof(str), "\033[1;36m[%s]\033[1;33m %s", name, text);
  print(str);
}


void vHCONSOLE::printError(const char *name, const char *text) {
  char str[1024] = "";
  VPRINTF(str, sizeof(str), "\033[1;36m[%s]\033[1;31m %s", name, text);
  print(str);
}


void vHCONSOLE::printDebug(const char *name, const char *text) {
  char str[1024] = "";
  VPRINTF(str, sizeof(str), "\033[1;36m[%s]\033[1;32m %s", name, text);
  print(str);
}


void vHCONSOLE::clear() {
  RmxNul( (VMSG::SYSTEM, 305, this) );
}


HPLUGIN vHCONSOLE::getPlugin() {
  RmxPtr( HPLUGIN, (VMSG::SYSTEM, 320, this) );
}


HLIST vHCONSOLE::getList() {
  RmxPtr( HLIST, (VMSG::SYSTEM, 321, this) );
}


HCONSOLE vHCONSOLE::getFirst() {
  RmxPtr( HCONSOLE, (VMSG::SYSTEM, 310) );
}


HCONSOLE vHCONSOLE::getLast() {
  RmxPtr( HCONSOLE, (VMSG::SYSTEM, 311) );
}


HCONSOLE vHCONSOLE::getPrev(HCONSOLE console) {
  RmxPtr( HCONSOLE, (VMSG::SYSTEM, 312, console) );
}


HCONSOLE vHCONSOLE::getNext(HCONSOLE console) {
  RmxPtr( HCONSOLE, (VMSG::SYSTEM, 313, console) );
}
