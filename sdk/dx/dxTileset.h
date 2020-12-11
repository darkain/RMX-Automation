/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DX_TILESET_H_
#define _DX_TILESET_H_


#include "vDirectX.h"


class dxTexture;


class dxTileset {
  public:
    dxTileset(dxTexture *texture, int tilew=32, int tileh=32);
    virtual ~dxTileset();


  public:
    inline dxTexture *getTexture()  { return tex;   }

    inline int getTileWidth()  const { return w;     }
    inline int getTileHeight() const { return h;     }
    inline int getTileCols()   const { return cols;  }
    inline int getTileRows()   const { return rows;  }
    inline int getTileCount()  const { return tiles; }

    RECT getTileRect(int tile);

  private:
    int w, h;
    int cols;
    int rows;
    int tiles;

    dxTexture *tex;
};


#endif //_DX_TILESET_H_
