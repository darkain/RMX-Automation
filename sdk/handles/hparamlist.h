/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __HPARAM_LIST_H_
#define __HPARAM_LIST_H_


class vHPARAMLIST {
  private:
    vHPARAMLIST() {}
    ~vHPARAMLIST() {}

  public:
    const char *getParam(const char *name) const;
    const char *getParamSafe(const char *name, const char *safe="") const;

    const char *getFirstParam() const;
    const char *getLastParam()  const;
    const char *getPrevParam(const char *param) const;
    const char *getNextParam(const char *param) const;
};


#endif //__HPARAM_LIST_H_
