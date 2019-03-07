/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _EVNT_ABSOLURE_H_
#define _EVNT_ABSOLURE_H_


#include "evntBase.h"


class evntAbsolute : public evntBase {
  public:
    evntAbsolute(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~evntAbsolute();

  public:
    void setMin(double min);
    void setMax(double max);
    void setMinMax(double min, double max);

    inline double getMin() const { return getParamNumber("min"); }
    inline double getMax() const { return getParamNumber("max"); }


    //this will automatically convert to proper internal measurements
    void setAbsoluteValue(double value);

  protected:
//    virtual void onSetParam(const prmBase *param);
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);
};


#endif //_EVNT_ABSOLURE_H_
