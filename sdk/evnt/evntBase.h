/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _EVNT_BASE_H_
#define _EVNT_BASE_H_


#include "../cfg/cfgBase.h"


/*
this class uses UserData to store its user-assigned function information
*/


class evntBase : public cfgBase {
  protected:
    evntBase(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);

  public:
    virtual ~evntBase() {}


  public:
    void updateDescription();

	inline const char *getFunction() const { return cfgBase::getValue(); }

    inline void setDisplayText(const char *text) { setParam("display", text); }
    inline const char *getDisplayText() const { return getParam("display"); }

    //hints are used to automatically match events to functions
    inline void setHint(const char *text) { setParam("hint", text); }
    inline const char *getHint() const { return getParam("hint"); }

    //value: 0.0 to 100.0 for absolute, -100.0 to 100.0 for relative
    inline void setValue(double value) { setParamNumber("value", value); }
    inline double getValue() const { return getParamNumber("value"); }

    inline const char *getStyle() const { return getParam("style"); }

  protected:
    virtual void onActivated() {}

    virtual void onClick(HPLUGIN source);
//    virtual void onSetParam(const prmBase *param);
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);
    virtual void validateParam(prmBase *param);

//    virtual void onPreLoadSettings();
//    virtual void onLoadSettings();
    virtual prmBase *onNewExternalParam(const char *name, const char *value);
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
};


#endif //_EVNT_BASE_H_
