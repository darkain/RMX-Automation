/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "engine.h"
#include "engWnd.h"
#include "resource.h"


engWnd::engWnd(const char *name, cfgBase *parent) : dxWnd(name, parent) {
  sprite = NULL;

  tex = NULL;
  set = NULL;
  map = NULL;

  xd = 1.5f;
  yd = 2.0f;

  mousedown = FALSE;

  insertHookWnd(this);
}


engWnd::~engWnd() {
  stopTimer();

  vdelnull(sprite);
  vdelnull(map);
  vdelnull(set);
  vdelnull(tex);
}


void engWnd::onTimer(DWORD systemTime) {
  render();
}


BOOL engWnd::onClose() {
  if (ENGINE) ENGINE->destroyPassive();
  return FALSE;
}


void engWnd::onPostInit() {
  dxWnd::onPostInit();

  setIcon(RMX_ICON_LARGE_XP);

  resizeClient(800, 600, FALSE);


  LuaObject globals = getLuaHandle()->GetGlobals();
  globals.Register("drawText", _drawText);

  if (!globals.GetByName("onRender").IsFunction()) {
    lua_script("function onRender() end");
  }



  if (FAILED(getDevice()->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vertex, NULL))) {
    CONSOLE_MAIN->print("vdxWnd::onPostInit() > CreateVertexBuffer FAILED");
    return;
  }


  CUSTOMVERTEX vertices[] = {
    { 150.0f, 480.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
    { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
    { 480.0f, 150.0f, 0.5f, 1.0f, 0x00ffff00, },
  };

  void *pVertices;
  if (FAILED(vertex->Lock(0, sizeof(vertices), (void**)&pVertices, 0))) {
    CONSOLE_MAIN->print("vdxWnd::onPostInit() > vertex->Lock FAILED");
    return;
  }

  memcpy( pVertices, vertices, sizeof(vertices) );

  vertex->Unlock();


  tex = new dxTexture("tileset.bmp");
  set = new dxTileset(tex);
  map = new dxTilemap("map", set, 32, 32, this);

  sprite = new dxSpriteSingle("image", "img.bmp", this);

  setTimeout(1);
  startTimer();
}


void engWnd::onPostRender() {
  getDevice()->SetStreamSource(0, vertex, 0, sizeof(CUSTOMVERTEX));
  getDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);
  getDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

  sprite->moveRight(xd * getTime());
  if (sprite->getX() < 0  &&  xd < 0) xd = -xd;
  if (sprite->getX() + sprite->getTexture()->getWidth()  > 800  &&  xd > 0) xd = -xd;

  sprite->moveDown(yd * getTime());
  if (sprite->getY() < 0  &&  yd < 0) yd = -yd;
  if (sprite->getY() + sprite->getTexture()->getHeight() > 600  &&  yd > 0) yd = -yd;

  

  char str[64];
  sprintf(str, "%d : %d", map->getOffsetX(), map->getOffsetY());
  drawText(str, 0, 32);


  lua_script("onRender()");
}



void engWnd::onLeftDown(int x, int y) {
  mapx = map->getOffsetX();
  mapy = map->getOffsetY();

  mousex = x;
  mousey = y;

  mousedown = TRUE;

  SetCapture(getWnd());
  SetCursor(NULL);

  whkMouse::onLeftDown(x, y);
}


void engWnd::onLeftUp(int x, int y) {
  mousedown = FALSE;

  SetCursor(LoadCursor(NULL, IDC_ARROW));
  ReleaseCapture();

  whkMouse::onLeftUp(x, y);
}


void engWnd::onMouseMove(int x, int y, HWND hwnd) {
  if (mousedown) {
    int newx = mousex - x;
    int newy = mousey - y;

    map->setOffsetX(mapx + newx);
    map->setOffsetY(mapy + newy);
  }

  whkMouse::onMouseMove(x, y, hwnd);
}



int engWnd::_drawText(LuaState *lua) {
  if (lua->GetTop() > 0  &&  lua->Stack(1).IsString()) {
    int   x     = (lua->GetTop() > 1  &&  lua->Stack(2).IsInteger()) ? (lua->Stack(2).GetInteger()) : 0;
    int   y     = (lua->GetTop() > 2  &&  lua->Stack(3).IsInteger()) ? (lua->Stack(3).GetInteger()) : 0;
    DWORD color = (lua->GetTop() > 3  &&  lua->Stack(4).IsInteger()) ? (lua->Stack(4).GetInteger()) : 0xffffffff;
    drawText(lua->Stack(1).GetString(), x, y, color);
  }
  return 0;
}