/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_INT_H_
#define _CFG_INT_H_


#include "cfgBase.h"


/*
this class uses UserData to store its displayed integer
*/

class cfgInt : public cfgBase {
  public:
    cfgInt(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgInt();

  public:
    void setValue(int value, bool defaultval=false);
    inline int getValue() const { return getValueInt(); }
    inline UINT getValueUnsigned() const { return (UINT) getValueInt(); }

    inline void increment(int amount=1) { setValue(getValue() + amount); }
    inline void decrement(int amount=1) { setValue(getValue() - amount); }

    void setMin(int min);
    void setMax(int max);
    void setMinMax(int min, int max);
    int getMin() const;
    int getMax() const;

    inline int getDefault() const { return getParamInt("default"); }
    void setDefault(int val);

    inline const char *getStyle() const { return getParam("style"); }
    inline void setStyle(const char *style) { setParam("style", style); }

    void validateUserData(prmBase *userdata);

  protected:
    virtual void onFirstLoad();
    virtual void onSetMinMax() {}
    virtual void onGainFocus(HPLUGIN source) {}
    virtual void onLostFocus(HPLUGIN source) {}

    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);
//    virtual void onSetParam(prmBase *param);
};


class cfgIntSlider : public cfgInt {
  public:
    cfgIntSlider(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : cfgInt(initname, parent, insert, paramlist) { setStyle("slider"); }

    virtual ~cfgIntSlider() {}
};


#endif //_CFG_INT_H_
