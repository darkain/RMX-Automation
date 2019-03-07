/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DX_TEXTURE_H_
#define _DX_TEXTURE_H_


#include "vDirectX.h"


class dxTexture : public vDirectX {
  public:
    dxTexture();
    dxTexture(const char *file);

    virtual ~dxTexture();

  public:
    inline LPDIRECT3DTEXTURE9 getTexture() const { return texture; }

    inline dxTexture *getTex() { return this; }

    inline int getWidth()  const { return w; }
    inline int getHeight() const { return h; }

    
    void loadTexture(const char *file);

    inline void appendRef() { ref++; }
    inline void removeRef() { ref--; }
    inline int  getRefCount() const { return ref; }

  private:
    LPDIRECT3DTEXTURE9 texture;
    int w;
    int h;
    int ref;
};


#endif //_DX_TEXTURE_H_