/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_LIST_AUTO_H_
#define _CB_LIST_AUTO_H_


template <class T>
class cbListAuto : public cbList {
  public:
    cbListString(const char *type) : string(type) {}
    virtual ~cbListAuto() {}

  protected:
    cbCore *validateConfig(HLIST config) {
      if (getPlugin()->list_getType() == string) return new T(getPlugin(), config);
      return NULL;
    }

  private:
   rmxString string;
};


#endif //_CB_LIST_AUTO_H_
