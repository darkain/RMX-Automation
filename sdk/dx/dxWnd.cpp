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


#include "dxWnd.h"
#include "dxSprite.h"


IDirect3D9       *vDirectX::directx = NULL;
IDirect3DDevice9 *vDirectX::device  = NULL;
ID3DXSprite      *vDirectX::sprite  = NULL;
ID3DXFont        *vDirectX::font    = NULL;

__int64 vDirectX::counter  = 0;
__int64 vDirectX::tick     = 0;
__int64 vDirectX::lasttick = 0;
float   vDirectX::time     = 0;

D3DXVECTOR3 vDirectX::vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


rmxList<dxWnd*>    dxWnd::dxwnds;


dxWnd::dxWnd(const char *name, cfgBase *parent)
 : wndBase("vDrawWnd"), cfgBase(name, "vDrawWnd", parent) {

  QueryPerformanceFrequency((PLARGE_INTEGER)&counter);
  QueryPerformanceCounter(  (PLARGE_INTEGER)&tick   );
  lasttick  = tick;
  time      = 0.0f;

  setSavable(FALSE);
  setName(name);

  if (!directx) {
    if(NULL == (directx = Direct3DCreate9(D3D_SDK_VERSION))) {
	  CONSOLE_MAIN->print("Failed to create Direct3D object");
    }
  }

  dxwnds.appendItem(this);
}


dxWnd::~dxWnd() {
  dxwnds.removeItem(this);

  if (dxwnds.getItemCount() == 0) {
    vrelease(font);
    vrelease(sprite);
    vrelease(device);
    vrelease(directx);
  }
}


void dxWnd::onPostInit() {
  wndBase::onPostInit();

  if (!device) {
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
    d3dpp.BackBufferWidth        = 800;
    d3dpp.BackBufferHeight       = 600;
    d3dpp.BackBufferCount        = 1;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


    if (FAILED(directx->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, getWnd(), D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &device))) {
      CONSOLE_MAIN->print("dxWnd::onPostInit() > CreateDevice FAILED");
      return;
    }

    if (D3DXCreateSprite(device, &sprite) != S_OK) {
      CONSOLE_MAIN->print("dxWnd::onPostInit() > D3DXCreateSprite FAILED");
      return;
    }

    if (D3DXCreateFont(device, 30, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &font) != S_OK) {
      CONSOLE_MAIN->print("dxWnd::onPostInit() > D3DXCreateFont FAILED");
      return;
    }

  }
}



void dxWnd::render() {
  if (isMinimized()) return;

  device->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0,0,255), 1.0f, 0);
  device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
  device->BeginScene();

  sprite->Begin(D3DXSPRITE_ALPHABLEND);
  onPreRender();


  HPLUGIN hmaster = vHPLUGIN::getMaster();
  HLIST   hlist   = getListHandle();
  int     count   = 0;

  cfgBase *child = cfgBase::getFirstChild();
  while (child) {
    count += (int) child->onMessage(hmaster, GENERIC_PAINT, (RMXLONG)hlist, 0);
    child = cfgBase::getNextChild(child);
  }


  sprite->Flush();
  sprite->End();

  device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  onPostRender();


  lasttick = tick;
  QueryPerformanceCounter((PLARGE_INTEGER)&tick);
  time = ((float)(tick - lasttick)) / (counter / 60.0f);


  char text[64];
  VPRINTF(text, sizeof(text), "%.1f FPS : %d Objects", 60.0f/time, count);
  drawText(text, 10, 0);


  device->EndScene();
  device->Present(NULL, NULL, NULL, NULL);
}


#endif //_RMX_DIRECTX_
