/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2011 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_SDK_ANA_AVERAGE_H_
#define _RMX_SDK_ANA_AVERAGE_H_



template <class T>
class rmxListAverage : public rmxList<T> {
	public:
		rmxListAverage() : rmxList<T>() {
			sum = 0;
			avr = 0;
			max = -1;
		}


	public:
		virtual void onInsertItem(T data, T before) {
			sum += data;

			if (hasItems()) {
				avr = sum / getItemCount();
			} else {
				avr = sum;
			}

			if (max >= 0  &&  getItemCount() > max) {
				removeFirstItem();
			}
		}

		virtual void onDelete(T data) {
			sum -= data;

			if (hasItems()) {
				avr = sum / getItemCount();
			} else {
				avr = 0;
			}
		}


	public:
		inline T getSum()        const { return sum; }
		inline T getAverage()    const { return avr; }
		inline int getMaxCount() const { return max; }


		void setMaxCount(int new_max) {
			max = new_max;

			if (max >= 0) {
				while (getItemCount() > max) {
					removeFirstItem();
				}
			}
		}


	private:
		T sum;
		T avr;
		int max;
};


#endif //_RMX_SDK_ANA_AVERAGE_H_
