/******************************  RMX SDK  ******************************\
*  Copyright (c) 2012 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_LABEL_H_
#define _CFG_LABEL_H_


#include "cfgBase.h"



class cfgLabel : public cfgBase {
public:
	cfgLabel(const char *name, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
		: cfgBase(name, "label", parent, insert, paramlist) {
		setParam("align", "left")->setSavable(false);
		setParam("label", ""    )->setSavable(false);
	}

	virtual ~cfgLabel() {}


protected:
	virtual void validateParam(prmBase *param) {
		if (param->isNamed("align")) {
			if (param->isValued("left")) {}
			else if (param->isValued("center")) {}
			else if (param->isValued("right")) {}
			else {
				param->setValue("left");
			}
		}
		cfgBase::validateParam(param);
	}


public:
	inline const char *getLabel() { return getParam("label"); }
	inline cfgLabel* setLabel(const char *label)    { setParam("label", label); return this; }
	inline cfgLabel* setLabel(const rmxData *label) { setParam("label", label); return this; }

	inline cfgLabel* alignLeft()   { setParam("align", "left");   return this; }
	inline cfgLabel* alignCenter() { setParam("align", "center"); return this; }
	inline cfgLabel* alignRight()  { setParam("align", "right");  return this; }
};


#endif //_CFG_LABEL_H_
