/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_PAIR_H_
#define _RMX_PAIR_H_


template<class T>
struct rmxPair {
	rmxPair(T a, T b) : one(a), two(b) {}
	T one;
	T two;
};


#endif //_RMX_PAIR_H_
