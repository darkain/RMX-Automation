/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#ifndef _RMX_NAME_H_
#define _RMX_NAME_H_


#include "rmxBase.h"
#include <malloc.h>


//TODO: replace this with a more powerful string class
class rmxName {
  public:
    rmxName(const char *initialName=NULL) {
      name = NULL;
      setName(initialName, FALSE);
    }

    rmxName(const wchar_t *initialName) {
      name = NULL;
      setName(initialName, FALSE);
    }

    virtual ~rmxName() {
      free((void*)name);
      name = NULL;
    }

  protected:
    virtual void onNameChange(const char *newname) {}

  public:
    inline const char *getName(const char *defVal=NULL) const {
      return (name ? name : defVal);
    }

	inline BOOL isNamed(const char *test) const {
	  if (!name  ||  !*name  ||  !test  ||  !*test) return FALSE;
	  return (VSTRCMP(name, test) == 0);
	}


    void setName(const char *newName, BOOL cb=TRUE) {
      if (newName == name) return;
      if (!newName) {
        free((void*)name);
        name = NULL;
        if (cb) onNameChange(NULL);
        return;
      }
      VINT len = VSTRLEN(newName)+1;
      name = (char*)realloc((void*)name, len);
      memcpy((void*)name, newName, len);
      if (cb) onNameChange(newName);
    }


    inline void setName(const wchar_t *newName, BOOL cb=TRUE) {
      rmxToUtf8 str(newName);
      setName(str.getBuffer(), cb);
    }


    //dont use this one, use setName instead
    inline void _setNameBuffer(const char *newName, BOOL cs=TRUE) {
      if (name == newName) return;
      free((void*)name);
      name = newName;
    }

  private:
    const char *name;
};


#endif _RMX_NAME_H_

#endif 
