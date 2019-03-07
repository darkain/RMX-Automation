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


#include "dxTexture.h"


dxTexture::dxTexture() {
  texture = NULL;
  ref     = 0;
  loadTexture(NULL);
}


dxTexture::dxTexture(const char *file) {
  texture = NULL;
  ref     = 0;
  loadTexture(file);
}


dxTexture::~dxTexture() {
  vrelease(texture);
}


void dxTexture::loadTexture(const char *file) {
  vrelease(texture);

  w = 0;
  h = 0;

  if (!file) return;

#ifdef _DEBUG
  CONSOLE_MAIN->printWarning("LOAD TEXTURE", file);
#endif

  D3DXIMAGE_INFO info;
  ZeroMemory(&info, sizeof(D3DXIMAGE_INFO));

  rmxUtf8ToWide str(file);
  if (D3DXCreateTextureFromFileEx(getDevice(), str.getBuffer(), 256, 256, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xffff00ff, &info, NULL, &texture) != D3D_OK) {
    CONSOLE_MAIN->print("dxTexture::dxTexture(file) > D3DXCreateTextureFromFile FAILED");
    return;
  }

  w = info.Width;
  h = info.Height;
}



#endif //_RMX_DIRECTX_