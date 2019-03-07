/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_BASE_H_
#define _FUNC_BASE_H_


#include "../cfg/cfgBase.h"
#include "funcParam.h"
#include "funcCallback.h"


/*
UserData is not used here
*/


class funcGroup;
class cfgRoot;


class funcBase : public cfgBase {
  friend class funcCallback;

  protected:
    funcBase(const char *function_name, const char *script_name, cfgRoot   *parent,      HPARAMLIST paramlist=NULL);
    funcBase(const char *function_name, const char *script_name, funcGroup *parent=NULL, HPARAMLIST paramlist=NULL);

  public:
    virtual ~funcBase();


  public:
    inline const char *getFunctionName() const { return getName();          }
    inline const char *getScriptName()   const { return getParam("script"); }

    inline void setHint(const char *text) { setParam("hint", text); }
    inline const char *getHint() const { return getParam("hint"); }

    inline const char *getStyle() const { return getParam("style"); }

    inline funcCallback *getFunctionCallback() const { return callback; }

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output);

    virtual const char *getMessageName(HPLUGIN source, HLIST input) { return getName(); }

    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);

    virtual void validateParam(prmBase *param);
    //virtual void onSetParam(const prmBase *param);
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);

    void removeFunctionCallback();

  private:
    funcCallback *callback;
};


#endif //_FUNC_BASE_H_
