/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DX_TILEMAP_H_
#define _DX_TILEMAP_H_


#include "dxRender.h"


class dxTileset;


class dxTilemap : public dxRender {
  public:
    dxTilemap(const char *name, dxTileset *tiles, int width, int height, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~dxTilemap();

  protected:
    virtual int onRender();

  public:
    inline dxTileset *getTileset() { return tileset; }

    inline int getWidth()  const { return w; }
    inline int getHeight() const { return h; }

    inline int getOffsetX() const { return offx; }
    inline int getOffsetY() const { return offy; }

    inline void setOffsetX(int x) { offx = x; }
    inline void setOffsetY(int y) { offy = y; }

  private:
    dxTileset *tileset;
    int w, h;
    int offx, offy;
    short *map;
};


#endif //_DX_TILEMAP_H_
