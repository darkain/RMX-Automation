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

#include "dxTileset.h"
#include "dxTexture.h"


dxTileset::dxTileset(dxTexture *texture, int tilew, int tileh)  {
  tex   = texture;
  tex->appendRef();

  w     = tilew;
  h     = tileh;
  cols  = tex->getWidth()  / getTileWidth();
  rows  = tex->getHeight() / getTileHeight();
  tiles = cols * rows;
}


dxTileset::~dxTileset() {
  tex->removeRef();
}


RECT dxTileset::getTileRect(int tile) {
  RECT r;
  
  r.top    = (tile / getTileCols()) * getTileHeight();
  r.bottom = r.top  + getTileHeight();

  r.left   = (tile % getTileCols()) * getTileWidth();
  r.right  = r.left + getTileWidth();

  return r;
}


#endif //_RMX_DIRECTX_
