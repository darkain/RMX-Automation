/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __STRINGLIST_H_
#define __STRINGLIST_H_


#include "rmxList.h"
#include "rmxPair.h"
#include "rmxData.h"


#define vNullStringList vNullAnsiStringList


class rmxStringList : public rmxList<const char*> {
public:
	rmxStringList(char *buffer, const char split, bool copy=false) {
		copied = copy;
		if (copied) {
			int len = strlen(buffer);
			list_buffer = (char*) malloc(len+1);
			memcpy(list_buffer, buffer, len+1);
		} else {
			list_buffer = buffer;
		}

		char *start = list_buffer;
		char *pos   = list_buffer;

		while (*pos) {
			if (*pos == split) {
				*pos = NULL;
				appendItem(start);
				start = pos + 1;
			}
			pos++;
		}

		appendItem(start);
	}


	virtual ~rmxStringList() {
		if (copied) free(list_buffer);
	}

private:
	char *list_buffer;
	bool copied;
};



class rmxDataList : public rmxList<rmxData*> {
public:
	rmxDataList(char *buffer, const char split, bool copy=false) {
		copied = copy;
		if (copied) {
			int len = strlen(buffer);
			list_buffer = (char*) malloc(len+1);
			memcpy(list_buffer, buffer, len+1);
		} else {
			list_buffer = buffer;
		}

		char *start = list_buffer;
		char *pos   = list_buffer;

		while (*pos) {
			if (*pos == split) {
				*pos = NULL;
				appendItem( new rmxData(start) );
				start = pos + 1;
			}
			pos++;
		}

		appendItem( new rmxData(start) );
	}


	virtual ~rmxDataList() {
		emptyItems();
		if (copied) free(list_buffer);
	}


	virtual void onDelete(rmxData *data) {
		delete data;
	}

private:
	char *list_buffer;
	bool copied;
};



class vNullAnsiStringList : public rmxList<const char*> {
  public:
    vNullAnsiStringList(const char *buffer) {
      if (!buffer) return;
      appendItem(buffer);
      while (*buffer) {
        buffer++;
        if (*buffer == 0) {
          buffer++;
          if (*buffer) appendItem(buffer);
        }
      }
    }
};


class vNullUnicodeStringList : public rmxList<const short*> {
  public:
    vNullUnicodeStringList(const short *buffer) {
      if (!buffer) return;
      appendItem(buffer);
      while (*buffer) {
        buffer++;
        if (*buffer == 0) {
          buffer++;
          if (*buffer) appendItem(buffer);
        }
      }
    }
};



template <class T>
class vNullStringPairList : public rmxDeleteList<rmxPair<T>*> {
  public:
    vNullStringPairList(T *buffer) {
      for (int i=0; buffer[i]; i+=2) {
        appendItem(new rmxPair<T>(buffer[i], buffer[i+1]));
      }
    }

    ~vNullStringPairList() {
      emptyItems();
    }


  public:
    inline T getItemValue(T item) {
      rmxPair<T> *pair = getPair(item);
      return ( (pair) ? (pair->two) : (NULL) );
    }

    inline T getFirstName() {
      rmxPair<T> *pair = getFirstItem();
      return ( (pair) ? (pair->one) : (NULL) );
    }

    inline T getLastName()  {
      rmxPair<T> *pair = getLastItem();
      return ( (pair) ? (pair->one) : (NULL) );
    }

    inline T getPrevName(T item) {
      rmxPair<T> *pair = getPrevItem(getPair(item));
      return ( (pair) ? (pair->one) : (NULL) );
    }

    inline T getNextName(T item) {
      rmxPair<T> *pair = getNextItem(getPair(item));
      return ( (pair) ? (pair->one) : (NULL) );
    }


  private:
    rmxPair<T> *getPair(T n) {
      rmxPair<T> *p = getCachedItem();
      if (p!=NULL  &&  p->one == n) return p;  //'n' might indeed be the pointer, so this works

      p = getFirstItem();
      while (p) {
        if (VSTRCMP(n, p->one) == 0) return p;
        p = getNextItem(p);
      }
      return NULL;
    }
};


#endif //__STRINGLIST_H_
