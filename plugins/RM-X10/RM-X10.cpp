/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

/*
ftp://ftp.x10.com/pub/manuals/cm11a_protocol.txt
*/

#include <time.h>
#include "RM-X10.h"


static vRM_X10 *RM_X10 = new vRM_X10();


unsigned char vRM_X10::house_code[16] = {
  HOUSE_A, HOUSE_B, HOUSE_C, HOUSE_D,
  HOUSE_E, HOUSE_F, HOUSE_G, HOUSE_H,
  HOUSE_I, HOUSE_J, HOUSE_K, HOUSE_L,
  HOUSE_M, HOUSE_N, HOUSE_O, HOUSE_P,
};

unsigned char vRM_X10::unit_code[16] = {
  UNIT_1,  UNIT_2,  UNIT_3,  UNIT_4, 
  UNIT_5,  UNIT_6,  UNIT_7,  UNIT_8, 
  UNIT_9,  UNIT_10, UNIT_11, UNIT_12, 
  UNIT_13, UNIT_14, UNIT_15, UNIT_16, 
};


vRM_X10::vRM_X10() : plugBase("RM-X10") {
  x10 = NULL;
  function = NULL;
  root = NULL;
}


void vRM_X10::onInit() {
  x10 = new x10comm();
  x10->openPort(COM1);

  root = new cfgTree("RM-X10");

  function = new funcGroup("X10", "X10");
  //todo:  write a script Power On and Power Off function

  for (int house=0; house<16; house++) {
    char strhouse[16];
    VPRINTF(strhouse, sizeof(strhouse), "House Code: %C", 'A'+house);
    funcGroup *funchouse = new funcGroup(strhouse, NULL, function);
    cfgTree    *treehouse = new cfgTree(strhouse, root);

    treehouse->insertParamIntNoSave("code", house_code[house]);

    for (int unit=0; unit<16; unit++) {
      char strunit[16];
      VPRINTF(strunit, sizeof(strunit), "Unit Code: %d", unit+1);
      funcGroup *funcunit = new vX10FunctionGroup(strunit, funchouse, house, unit);
      cfgBase    *treeunit = new cfgNull(strunit, treehouse);

      treeunit->insertParamIntNoSave("code", unit_code[unit]);

      new vX10Function("Power On",  1, getDeviceCode(house, unit), funcunit);
      new vX10Function("Power Off", 2, getDeviceCode(house, unit), funcunit);

      new vX10Event("Powered On",   1, getDeviceCode(house, unit), treeunit);
      new vX10Event("Powered Off",  1, getDeviceCode(house, unit), treeunit);

//      VPRINTF(strunit, sizeof(strunit), "%C%d", 'a'+house, unit+1);
//      function->script_setValueInteger(strunit, getDeviceCode(house, unit));
    }
  }
}


void vRM_X10::onQuit() {
  vdelnull(function);
  vdelnull(x10);
  vdelnull(root);
}


unsigned char vRM_X10::getUnitCode(unsigned char unitcode) {
  for (int i=0; i<16; i++) {
    if ( (unitcode & 0x0F) == unit_code[i] ) return i;
  }
  return 0;
}

unsigned char vRM_X10::getHouseCode(unsigned char unitcode) {
  for (int i=0; i<16; i++) {
    if ( (unitcode & 0xF0) == house_code[i] ) return i;
  }
  return 0;
}


evntSingle *vRM_X10::getEvent(unsigned char unitcode, int mode) {
  cfgBase *house = root->getFirstChild();
  while (house) {
    if (house->getParamInt("code") == (unitcode & 0xF0)) {
      cfgBase *unit = house->getFirstChild();
      while (unit) {
        if (unit->getParamInt("code") == (unitcode & 0x0F)) {
          if (mode == 1) {
            return static_cast<evntSingle*>(unit->getChild("Powered On"));
          } else if (mode == 2) {
            return static_cast<evntSingle*>(unit->getChild("Powered Off"));
          } else {
            return NULL;
          }
        }
        unit = house->getNextChild(unit);
      }
    }
    house = root->getNextChild(house);
  }
  return NULL;
}


/////////////////////////////////////////////////////////////////


x10comm::x10comm() {
}


x10comm::~x10comm() {
  closePort();
}


void x10comm::onRing() {
  unsigned char command = 0;
  readBytes(&command, 1);

  switch(command) {
    case INTERFACE_CQ:
      onInterface();
      break;

    case CM11_CLOCK_REQ:
    case CP10_CLOCK_REQ:
      setClock();
      break;
  }
}


void x10comm::onInterface() {
  unsigned char length = 0;
  int count = 0;

  unsigned char command = COMPUTER_READY;
  writeBytes(&command, 1);

  do {
    readBytes(&length, 1);
    count++;
    if (count == 10) length = 0;
  } while(length == INTERFACE_CQ);

  if (length > 0) {
    unsigned char data[256];
    ZeroMemory(data, sizeof(data));
    readBytes(data, length);
    onCommand(data, length);
  }
}


void x10comm::onCommand(const unsigned char *data, int len) {
  int i;  //stupid MSVC

#ifdef _DEBUG
  char str[128];
  char *tmp = str;
  for (i=0; i<len; i++) {
    VPRINTF(tmp, 4, "x%02X", data[i]);
    tmp += 3;
  }
  CONSOLE_MAIN->printWarning(RM_X10->getName(), str);
#endif

  unsigned char mask = data[0];
  for (i=1; i<len; i++) {
    unsigned char cmd = data[i];
    if (mask & (1<<(i-1))) {

      unsigned char house = cmd & 0xF0;
      int mode = 0;
      if ( (cmd & 0x0F) == 0x02) mode = 1;
      if ( (cmd & 0x0F) == 0x03) mode = 2;

      for (int j=i-1; j>0; j++) {
        if (mask & (1<<(j-1))) break;

        if ( (data[j] & 0xF0) == house ) {
          evntSingle *event = RM_X10->getEvent(data[j], mode);
          if (event) event->activate();
        }
      }
    }
  }
}


void x10comm::setClock() {
  unsigned char data[9];
  unsigned char buffer[3];
  time_t mytime;
  tm *thetime;

  mytime = time(NULL);
  thetime = localtime(&mytime);

  data[0] = SET_TIME;             /* CM11A timer download code */
  data[1] = thetime->tm_sec ;
  data[2] = thetime->tm_min + (((thetime->tm_hour) %2) * 60 ) ;  /* minutes 0 - 119 */
  data[3] = thetime->tm_hour / 2 ;                 /* hour / 2         0 - 11 */
  data[4] = thetime->tm_yday % 256 ;               /* mantisa of julian date */
  data[5] = ((thetime->tm_yday / 256 ) << 7);      /* radix of julian date */
  data[5] |= (0x01 << (thetime->tm_wday)); /* bits 0-6 =  single bit mask day */
                                       /* of week ( smtwtfs ) */
  data[6] = HOUSE_A; /* hard coded house code A for monitoring, probably shouldn't be hard coded */
  data[6] |= 0x01;

  writeBytes(data, 7);
  Sleep(1000);
  readBytes(buffer, 1);
}


BOOL x10comm::x10WriteBytes(const unsigned char *data, int len) {
  BOOL dostuff = TRUE;
  unsigned char checksum = 0;
  int count = 0;

  for (int i=0; i<len; i++) {
    checksum += data[i];
  }

  while (dostuff) {
    if (writeBytes(data, len) == FALSE) return FALSE;

    unsigned char reply = 0;
    readBytes(&reply, 1);

    if (reply == CM11_CLOCK_REQ  ||  reply == CP10_CLOCK_REQ) {
      setClock();
    } else if (reply == INTERFACE_CQ) {
      onInterface();
    } else if (reply == checksum) {
      unsigned char ready = 0x00;
      writeBytes(&ready, 1);
      readBytes(&ready, 1);
      if (ready == INFERFACE_READY) return TRUE;
      if (ready == 0x00) return TRUE;
      return FALSE;
    }

    if (++count == 10) break;
  }
  return FALSE;
}


BOOL x10comm::x10HouseCommand(unsigned char function, unsigned char data, unsigned char house) {
  unsigned char senddata[2];
  senddata[0] = (data << 3) | STANDARD_FUNCTION;
  senddata[1] = house | function;
  return x10WriteBytes(senddata, 2);
}


BOOL x10comm::x10UnitCommand(unsigned char function, unsigned char data, unsigned char house, unsigned char unit) {
  unsigned char senddata[2];
  senddata[0] = STANDARD_ADDRESS;
  senddata[1] = house | unit;
  if (x10WriteBytes(senddata, 2) == FALSE) return FALSE;
  return x10HouseCommand(function, data, house);
}


BOOL x10comm::x10UnitCommand(unsigned char function, unsigned char data, unsigned char device) {
  unsigned char senddata[2];
  senddata[0] = STANDARD_ADDRESS;
  senddata[1] = device;
  if (x10WriteBytes(senddata, 2) == FALSE) return FALSE;
  return x10HouseCommand(function, data, (device & 0xF0));
}


/////////////////////////////////////////////////////////////////


vX10FunctionGroup::vX10FunctionGroup(const char *name, funcGroup *parent, int house, int unit) 
: funcGroup(name, NULL, parent) {
  insertParamInt("house", house);
  insertParamInt("unit" , unit);
}

vX10FunctionGroup::~vX10FunctionGroup() {
}

RMXLONG vX10FunctionGroup::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
  if (message == FUNCTION_START) {
    char str[8] = "";
    int house = getParamInt("house");
    int unit  = getParamInt("unit");
    VPRINTF(str, sizeof(str), "%C%d", house+'a', unit+1);
    RM_X10->getScript()->script_setValueInteger(str, RM_X10->getDeviceCode(house, unit));
  }
  return funcGroup::onMessage(source, message, param1, param2);
}

/////////////////////////////////////////////////////////////////


void vX10Function::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (mode == 1) {  //TODO: switch this over to evntPress
    RM_X10->getX10()->x10UnitCommand(ON,  0, code);
  } else if (mode == 2) {
    RM_X10->getX10()->x10UnitCommand(OFF, 0, code);
  }
}


/////////////////////////////////////////////////////////////////

/*
vX10Script::vX10Script(const char *name, vScriptTable *parent, int x10mode) : vScriptFunction(name, parent) {
  mode = x10mode;
  insertParamNoCfg("code",  NULL);
  
  if (mode == 1) {
    setTooltip("Sends a X10 Unit Power ON signal");
  } else if (mode == 2) {
    setTooltip("Sends a X10 Unit Power OFF signal");
  }
}


void vX10Script::onActivated(vScriptStack *stack) {
  vConfigParam *param = getParamData("code");
  if (!param  ||  param->isNULL())  {
    stack->addValueNull();
    return;
  }

  int code = -1;
  if (param->isInteger()) {
    code = param->getValueInt();
  } else {
    char letter = -1;
    int number = -1;
    VSCANF(param->getValue(), "%c%d", &letter, &number);
    if (letter >= 'a'  &&  letter <= 'p') {
      if (number >= 1  &&  number <= 16) {
        code = RM_X10.getDeviceCode(letter-'a', number-1);
      }
    }
  }

  if (code < 0  ||  code > 255) {
    stack->addValueNull();
    return;
  }

  if (mode == 1) {
    stack->addValueInt(RM_X10.getX10()->x10UnitCommand(ON , 0, code));
  } else if (mode == 2) {
    stack->addValueInt(RM_X10.getX10()->x10UnitCommand(OFF, 0, code));
  }
}
*/
