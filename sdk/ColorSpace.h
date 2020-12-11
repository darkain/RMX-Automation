/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __COLORSPACE_H_
#define __COLORSPACE_H_


// 555 Color Space... 16-bit
#define RGB555(r,g,b)  (r + (g<<5) + (b<<10))
#define BGR555(b,g,r)  (r + (g<<5) + (b<<10))
#define RGBRED555(c)   ((c >>  0) & 0x001F)
#define RGBGREEN555(c) ((c >>  5) & 0x001F)
#define RGBBLUE555(c)  ((c >> 10) & 0x001F)

// 888 Color Space.. 24/32-Bit
#define RGB888(r,g,b)  (r + (g<<5) + (b<<10))
#define BGR888(b,g,r)  (r + (g<<5) + (b<<10))


#ifdef _GBA_
#define RGB       RGB555
#define BRG       BRG555
#define RGBRED    RGBRED555
#define RGBGREEN  RGBGREEN555
#define RGBBLUE   RGBBLUE555
#else
#define RGB       RGB888
#define BRG       BRG888
#define RGBRED    RGBRED888
#define RGBGREEN  RGBGREEN888
#define RGBBLUE   RGBBLUE888
#endif


#endif //__COLORSPACE_H_
