/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _PLAYLISTFUNCTION_H_
#define _PLAYLISTFUNCTION_H_


#include "../../sdk/func/funcSingle.h"


class vPlaylistFunction : public funcSingle {
  public:
    vPlaylistFunction(const char *function_name, const char *script_name, int id, funcGroup *parent, const char *hint=NULL);
    virtual ~vPlaylistFunction();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    int cmd;
    funcParam *param;
};


class vPlaylistFunctionHint : public vPlaylistFunction {
  public:
    vPlaylistFunctionHint(const char *name, int id, funcGroup *parent, const char *hint)
      : vPlaylistFunction(name, NULL, id, parent, hint) {

      setVisible(FALSE);
      setSavable(FALSE);
    }
    
    ~vPlaylistFunctionHint() {}
};

#endif //_PLAYLISTFUNCTION_H_
