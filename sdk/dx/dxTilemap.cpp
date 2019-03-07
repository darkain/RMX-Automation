/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//primary include and definitions
#include "../rmxBase.h"
#ifdef _RMX_DIRECTX_

#include "dxTilemap.h"
#include "dxTileset.h"
#include "dxTexture.h"


dxTilemap::dxTilemap(const char *name, dxTileset *tiles, int width, int height, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : dxRender(name, "tilemap", parent, insert, paramlist) {

  tileset = tiles;
  w       = width;
  h       = height;
  offx    = 0;
  offy    = 0;

  map = (short*) malloc(2 * w * h);

  int cnt = 0;
  for (int i=0; i<w*h; i++) {
    map[i] = cnt++;
    if (cnt > 63) cnt = 0;
  }
}


dxTilemap::~dxTilemap() {
  free(map);
  map = NULL;
}


int dxTilemap::onRender() {
  if (!map) return 0;

  int count = 0;
//  short *tile = map;
  int tw      = tileset->getTileWidth();
  int th      = tileset->getTileHeight();

//  int offx = 10;  //pixel offsets
//  int offy = 0;

  int minx = offx / tw;
  int miny = offy / th;

  int maxx = minx + (800 / tw) + 2;
  int maxy = miny + (600 / th) + 2;

  minx = max(minx, 0);
  miny = max(miny, 0);

  maxx = min(maxx, getWidth());
  maxy = min(maxy, getHeight());

  for (int y=miny; y<maxy; y++) {
    for (int x=minx; x<maxx; x++) {

      short tile = *(map + (y * getWidth()) + x);

      if (tile) {

        D3DXVECTOR3 vector = D3DXVECTOR3((float)((x*tw)-offx), (float)((y*th)-offy), 0.6f);

        RECT r = tileset->getTileRect(tile);
        getSprite()->Draw(tileset->getTexture()->getTexture(), &r, getVector(), &vector, 0xffffffff);
  //      tile++;
        count++;

      }

    }
  }

  return count;
}


#endif //_RMX_DIRECTX_
