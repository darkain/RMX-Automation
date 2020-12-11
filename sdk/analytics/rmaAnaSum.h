/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2011 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_SDK_ANA_SUM_H_
#define _RMX_SDK_ANA_SUM_H_



template <class T>
class rmxListSum : public rmxList<T> {
	public:
		rmxListSum() : rmxList<T>() {
			sum = 0;
		}


	public:
		virtual void onInsertItem(T data, T before) {
			sum += data;
		}

		virtual void onDelete(T data) {
			sum -= data;
		}


	public:
		inline T getSum() const { return sum; }


	private:
		T sum;
};


#endif //_RMX_SDK_ANA_SUM_H_
