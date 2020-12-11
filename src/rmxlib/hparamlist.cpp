/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/handles/hparamlist.h"
#include "rmx_msg.h"


const char *vHPARAMLIST::getParam(const char *name) const { RmxStr( (VMSG::XML_PARAM_LIST, XMLPRMGETITEM, const_cast<vHPARAMLIST*>(this), name) ); }


const char *vHPARAMLIST::getParamSafe(const char *name, const char *safe) const {
  const char *ret = getParam(name);
  return ( (ret) ? (ret) : (safe) );
}


const char *vHPARAMLIST::getFirstParam()                 const { RmxStr( (VMSG::XML_PARAM_LIST, XMLPRMGETFIRST, const_cast<vHPARAMLIST*>(this)       ) ); }
const char *vHPARAMLIST::getLastParam()                  const { RmxStr( (VMSG::XML_PARAM_LIST, XMLPRMGETLAST,  const_cast<vHPARAMLIST*>(this)       ) ); }
const char *vHPARAMLIST::getPrevParam(const char *param) const { RmxStr( (VMSG::XML_PARAM_LIST, XMLPRMGETPREV,  const_cast<vHPARAMLIST*>(this), param) ); }
const char *vHPARAMLIST::getNextParam(const char *param) const { RmxStr( (VMSG::XML_PARAM_LIST, XMLPRMGETNEXT,  const_cast<vHPARAMLIST*>(this), param) ); }
