/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _ENGWND_H_
#define _ENGWND_H_


#include "../../sdk/Win32/Timer.h"
#include "../../sdk/dx/dxWnd.h"
#include "../../sdk/dx/dxSprite.h"
#include "../../sdk/dx/dxTileset.h"
#include "../../sdk/dx/dxTilemap.h"
#include "../../sdk/whk/whkMouse.h"
#include "../core/Lua/LuaHelper.h"


class engWnd : public dxWnd, public vTimer, public whkMouse {
  public:
    engWnd(const char *name, cfgBase *parent);
    virtual ~engWnd();

  protected:
    virtual void onTimer(DWORD systemTime);

    virtual BOOL onClose();

    virtual void onPostInit();

    virtual void onPostRender();

    virtual void onLeftDown( int x, int y);
    virtual void onLeftUp(   int x, int y);
    virtual void onMouseMove(int x, int y, HWND hwnd);


  private:
    static int _drawText(LuaState *lua);


  private:
    dxSprite  *sprite;

    dxTexture *tex;
    dxTileset *set;
    dxTilemap *map;

    IDirect3DVertexBuffer9 *vertex;
    float xd, yd;

    BOOL mousedown;
    int mousex, mousey;
    int mapx, mapy;
};


struct CUSTOMVERTEX {
  FLOAT x, y, z, rhw; // The transformed position for the vertex.
  DWORD color;        // The vertex color.
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


#endif //_ENGWND_H_
