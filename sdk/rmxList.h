/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2011 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_SDK_LIST_H_
#define _RMX_SDK_LIST_H_


#include <malloc.h>
#include "rmxTypes.h"


template <class T>
class rmxList {
  private:
    typedef struct vtagLIST {
      vtagLIST  *next;
      vtagLIST  *prev;
      T          data;
    } rmxListItem, *LPrmxListItem;


  public:
    rmxList() {
		zeroList();
    }


	rmxList(const rmxList<T> *source) {
		zeroList();
		clone(source);
	}


	rmxList(const rmxList<T> &source) {
		zeroList();
		clone(&source);
	}


    ~rmxList() {
      emptyItems();
    }


	inline void clone(const rmxList<T> *source) {
		emptyItems();
		T item = source->getFirstItem();
		while (item) {
			appendItem(item);
			item = source->getNextItem(item);
		}
	}



  protected:
    //called once an item is successfully added to the list
    virtual void onInsertItem(T data, T before) {}

    //called once an item is deleted from the list.
    virtual void onDelete(T data) {}

    //this is called when one of the move operations happens,
    //this way you can update anything that depends on list
    //positioning or anything like that
    virtual void onItemMoved(T data, T before) {}



  public:
    inline int  getItemCount()  const { return  count; }
    inline BOOL hasItems()      const { return (count>0); }
    inline BOOL hasItem(T data) const { return !!findItem(data); }

    
    int insertItem(T data, T insert) {
      if (!data) return -1;
      LPrmxListItem item = findItem(insert);

      LPrmxListItem newitem = createNew(data);
      if (item) {
        if (item->prev) {
          item->prev->next = newitem;
          newitem->prev = item->prev;
        }
        item->prev = newitem;
        newitem->next = item;
        if (item == first) first = newitem;
      } else if (last) {
        last->next = newitem;
        newitem->prev = last;
        last = newitem;
      } else {
        last = newitem;
        first = newitem;
      }
      cache = newitem;
      if (item)  onInsertItem(data, item->data);
      else onInsertItem(data, NULL);
      return count;
    }


    inline int prependItem(T data) {
      T fdata = NULL;
      if (first) fdata = first->data;
      return insertItem(data, fdata);
    }


    inline int appendItem(T data) { return insertItem(data, NULL); }
    inline int push(T data)       { return insertItem(data, NULL); }


    inline bool replaceItem(T olddata, T newdata) {
      LPrmxListItem item = findItem(olddata);
      if (!item) return false;
      item->data = newdata;
      return true;
    }


    inline int removeFirstItem() { if (first) removeOld(first); return count; }
    inline int removeLastItem()  { if (last)  removeOld(last);  return count; }
    inline int pop()             { if (last)  removeOld(last);  return count; }


    int removeItem(T data) {
      if (!first) return -1;
      LPrmxListItem item = findItem(data);
      if (item) removeOld(item);
      return count;
    }


    inline void emptyItems() {
      while (first) removeOld(first);
    }


	inline T getFirstItem()  const { cache = first; return (cache ? cache->data : NULL); }
    inline T getLastItem()   const { cache = last;  return (cache ? cache->data : NULL); }
    inline T getCachedItem() const {                return (cache ? cache->data : NULL); } 


    T getNextItem(T data) const {
      LPrmxListItem item = findItem(data);
      if (!item) return NULL;
      if (!item->next) return NULL;
      cache = item->next;
      return cache->data;
    }


    T getPrevItem(T data) const {
      LPrmxListItem item = findItem(data);
      if (!item)  return NULL;
      if (!item->prev) return NULL;
      cache = item->prev;
      return cache->data;
    }


    T getItem(int id) const {
	  if (id >= count) return NULL;
      T item = getFirstItem();
      for (int i=0; i<id; i++) item = getNextItem(item);
      return item;
    }


    int getItemIndex(T data) {
      LPrmxListItem item = first;
      int i=0;
      while (item) {
        if (item->data == data) return i;
        i++;
        item = item->next;
      }
      return -1;
    }


    BOOL moveItem(T data, T insert) {
      if (count < 2) return FALSE;
      LPrmxListItem itm = findItem(data);
      if (!itm) return FALSE;
      LPrmxListItem itm2 = findItem(insert);
      if (itm == itm2) return FALSE;
      if (itm->prev) itm->prev->next = itm->next;
      if (itm->next) itm->next->prev = itm->prev;
      if (itm == first) first = itm->next;
      if (itm == last) last = itm->prev;

      itm->next = itm2;
      if (itm2) {      
        itm->prev = itm2->prev;
        itm2->prev = itm;
        if (itm->prev) {
          itm->prev->next = itm;
        } else {
          first = itm;
        }
      } else {
        itm->prev = last;
        last->next = itm;
        last = itm;
      }

      onItemMoved(data, insert);
      return TRUE;
    }


    inline BOOL moveToBegining(T data) {
      T fdata = NULL;
      if (first) fdata = first->data;
      return moveItem(data, fdata);
    }


    inline BOOL moveToEnd(T data) {
      return moveItem(data, NULL);
    }


    inline BOOL chainLinkForward() {
      if (!first) return FALSE;
	  cache = first;
      return moveToEnd(first->data);
    }

   
    inline BOOL chainLinkBackward() {
      if (!last) return FALSE;
	  cache = last;
      return moveToBegining(last->data);
    }


    void reverseList() {
      LPrmxListItem *item = first;
      while (item) {
        LPrmxListItem *tmp = item->prev;
        item->prev = item->next;
        item->next = tmp;
        item = item->prev;
      }
    }


// NOTE: IWRAM_CODE is for use with ARM7TDMI when running code
//       from system RAM instead of from ROM (speed improvements)
#ifdef IWRAM_CODE
  public:
    inline T IWRAM_CODE L_getFirstItem()  const { cache = first; return (cache ? cache->data : NULL); }
    inline T IWRAM_CODE L_getLastItem()   const { cache = last;  return (cache ? cache->data : NULL); }
    inline T IWRAM_CODE L_getCachedItem() const {                return (cache ? cache->data : NULL); } 

    T IWRAM_CODE L_getNextItem(T data) const {
      LPrmxListItem item = L_findItem(data);
      if (!item) return NULL;
      if (!item->next) return NULL;
      cache = item->next;
      return cache->data;
    }

  protected:
    inline LPrmxListItem LONG_CALL L_findItem(T data) const {
      if (!data) return NULL;
      if (cache) if (cache->data == data) return cache;
      if (last) if (last->data == data) return last;
      LPrmxListItem item = first;
      while (item) {
        if (item->data == data) {
          cache = item;
          return item;
        }
        item = item->next;
      }
      return NULL;
    }
#endif //IWRAM_CODE


  private:
	  inline void zeroList() {
		  first = NULL;
		  last  = NULL;
		  cache = NULL;
		  count = 0;
	  }

    inline LPrmxListItem createNew(T data) {
      LPrmxListItem item = (LPrmxListItem) malloc(sizeof(vtagLIST));
      item->next = NULL;
      item->prev = NULL;
      item->data = data;
      count++;
      return item;
    }


    inline void removeOld(LPrmxListItem item) {
      count--;
      if (count == 0) {
        first = NULL;
        last  = NULL;
        cache = NULL;
      } else {
        if (item == cache) cache = item->next;
        if (item->next)    item->next->prev = item->prev;
        if (item->prev)    item->prev->next = item->next;
        if (item == first) first = item->next;
        if (item == last)  last  = item->prev;
      }
      onDelete(item->data);
      free(item);
    }


    inline LPrmxListItem findItem(T data) const {
      if (!data) return NULL;
      if (cache) if (cache->data == data) return cache;
      if (last)  if (last->data  == data) return last;
      LPrmxListItem item = first;
      while (item) {
        if (item->data == data) {
          cache = item;
          return item;
        }
        item = item->next;
      }
      return NULL;
    }


  private:
    LPrmxListItem first;
    LPrmxListItem last;
    mutable LPrmxListItem cache;
    int count;
};


template <class T>
class rmxFreeList : public rmxList<T> {
  public:
    virtual ~rmxFreeList() { rmxList<T>::emptyItems(); }
    virtual void onDelete(T data) { free((void*)data); }
};

template <class T>
class rmxDeleteList : public rmxList<T> {
  public:
    virtual ~rmxDeleteList() { rmxList<T>::emptyItems(); }
    virtual void onDelete(T data) { delete data; }
};


#endif //_RMX_SDK_LIST_H_
