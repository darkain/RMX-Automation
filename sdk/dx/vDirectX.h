/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DIRECTX_H_
#define _DIRECTX_H_


#include "../rmxBase.h"
#include "../Win32/Win32.h"


#ifdef _RMX_DIRECTX_


#ifdef _DEBUG
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#endif


#define DIRECT3D_VERSION 0x0900


#include <d3d9.h>
#include <D3dx9core.h>


class dxWnd;


class vDirectX {
  friend class dxWnd;

  public:
    vDirectX()  {}
    ~vDirectX() {}

  public:
    static IDirect3D9       *getDirectX()  { return directx;  }
    static IDirect3DDevice9 *getDevice()   { return device;   }
    static ID3DXSprite      *getSprite()   { return sprite;   }
    static ID3DXFont        *getFont()     { return font;     }

    static __int64 getTick()     { return tick;     }
    static __int64 getLastTick() { return lasttick; }
    static float   getTime()     { return time;     }

    static D3DXVECTOR3 *getVector() { return &vector; }


    static void drawText(const char *text, int x, int y, DWORD color=0xffffffff) {
      rmxUtf8ToWide str(text);
      RECT r = {x, y, 799, 599};
      font->DrawText(NULL, str.getBuffer(), -1, &r, 0, color);
    }


  private:
    static IDirect3D9       *directx;
    static IDirect3DDevice9 *device;
    static ID3DXSprite      *sprite;
    static ID3DXFont        *font;

    static __int64 counter;
    static __int64 tick;
    static __int64 lasttick;
    static float   time;

    static D3DXVECTOR3 vector;
};



#endif //_RMX_DIRECTX_


#endif //_DIRECTX_H_
