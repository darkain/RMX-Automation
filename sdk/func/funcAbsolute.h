/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_ABSOLUTE_H_
#define _FUNC_ABSOLUTE_H_


#include "funcBase.h"


class funcAbsolute : public funcBase {
  protected:
    funcAbsolute(const char *function_name, const char *script_name, cfgRoot   *parent);
    funcAbsolute(const char *function_name, const char *script_name, funcGroup *parent=NULL);

  public:
    virtual ~funcAbsolute();


  public:
    inline double getMin()   const { return getParamNumber("min"  ); }
    inline double getMax()   const { return getParamNumber("max"  ); }
    inline double getDelta() const { return getParamNumber("delta"); }
    inline double getValue() const { return getParamNumber("value"); }

    inline void setMin(  double min)   { setParamNumber("min",   min  ); }
    inline void setMax(  double max)   { setParamNumber("max",   max  ); }
    inline void setDelta(double delta) { setParamNumber("delta", delta); }
    inline void setValue(double value) { setParamNumber("value", value); }

    void setMinMax(double min, double max);

  protected:
    //if using funcAbsolute, use "onActivated" version with "double value" at the end (see below)
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output);
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, double value)=0;
};


#endif //_FUNC_ABSOLUTE_H_
