/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/Comm.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/func/FuncSingle.h"
#include "../../sdk/func/FuncGroup.h"
#include "../../sdk/evnt/evntSingle.h"
#include "x10.h"


class x10comm;


class vRM_X10 : public plugBase {
  public:
    vRM_X10();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    inline x10comm *getX10() { return x10; }
    inline unsigned char getDeviceCode(unsigned char house, unsigned char unit) const { return house_code[house] | unit_code[unit]; }
    unsigned char getUnitCode(unsigned char unitcode);
    unsigned char getHouseCode(unsigned char unitcode);

    evntSingle *getEvent(unsigned char unitcode, int mode);
    inline funcGroup *getScript() { return function; }

  private:
    x10comm *x10;
    funcGroup *function;

    cfgTree *root;

    static unsigned char house_code[16];
    static unsigned char unit_code[16];
};


class x10comm : public vComm {
  public:
    x10comm();
    virtual ~x10comm();

  protected:
    virtual void onRing();
    virtual void onInterface();
    virtual void onCommand(const unsigned char *data, int len);

    void setClock();

  public:
    BOOL x10WriteBytes(const unsigned char *data, int len);
    BOOL x10HouseCommand(unsigned char function, unsigned char data, unsigned char house);
    BOOL x10UnitCommand(unsigned char function, unsigned char data, unsigned char house, unsigned char unit);
    BOOL x10UnitCommand(unsigned char function, unsigned char data, unsigned char device);
};


class vX10FunctionGroup : public funcGroup {
  public:
    vX10FunctionGroup(const char *name, funcGroup *parent, int house, int unit);
    virtual ~vX10FunctionGroup();

  protected:
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
};


class vX10Function : public funcSingle {
  public:
    vX10Function(const char *name, int setmode, unsigned char devcode, funcGroup *parent) : funcSingle(name, NULL, parent) {
      mode = setmode;
      code = devcode;
    }

    virtual ~vX10Function() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    int mode;
    unsigned char code;
};



class vX10Event : public evntSingle {
  public:
    vX10Event(const char *name, int setmode, unsigned char devcode, cfgBase *parent) : evntSingle(name, parent) {
      mode = setmode;
      code = devcode;
    }

    virtual ~vX10Event() {}

  public:
    inline unsigned char getDeviceCode() const { return code; }
    inline int getMode() const { return mode; }

  private:
    int mode;
    unsigned char code;
};
