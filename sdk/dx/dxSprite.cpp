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


#include "dxSprite.h"
#include "dxWnd.h"


dxSprite::dxSprite(const char *name, dxTexture *texture, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : dxRender(name, "sprite", parent, insert, paramlist) {

  insertParam("x");
  insertParam("y");

  tex = texture;
  tex->appendRef();

  src.left   = 0;
  src.top    = 0;
  src.right  = texture->getWidth();
  src.bottom = texture->getHeight();
  
  dest       = D3DXVECTOR3(0.0f, 0.0f, 0.5f);

  visible    = TRUE;
}


dxSprite::~dxSprite() {
  tex->removeRef();
}


int dxSprite::onRender() {
  if (!isVisible()) return 0;

  getSprite()->Draw(tex->getTexture(), &src, getVector(), &dest, 0x8fffffff);

  return 1;  //TODO:  2 for 2 polys?
}


void dxSprite::onSetParam(const prmBase *param) {
  if (param->isNamed("x")) {
    dest.x = (float)param->getValueNumber();
    onMove();
  }

  else if (param->isNamed("y")) {
    dest.y = (float)param->getValueNumber();
    onMove();
  }

  cfgBase::onSetParam(param);
}


/*
int dxSprite::getWidth() const {
  return dest.right - dest.left;
}


int dxSprite::getHeight() const {
  return dest.bottom - dest.top;
}


void dxSprite::setWidth(int w) {
  w = validateW(w);
  dest.right = dest.left + w;
  onResize();
}


void dxSprite::setHeight(int h) {
  h = validateH(h);
  dest.bottom = dest.top + h;
  onResize();
}


void dxSprite::resize(int w, int h) {
  w = validateW(w);
  h = validateH(h);
  dest.right = dest.left + w;
  dest.bottom = dest.top + h;
  onResize();
}
*/


#endif //_RMX_DIRECTX_
