/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/handles/hfunction.h"
#include "../../sdk//func/funcGroup.h"
#include "rmx_msg.h"


HFUNCTION vHFUNCTION::getFirst() {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 90) );
}

HFUNCTION vHFUNCTION::getLast() {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 91) );
}

HFUNCTION vHFUNCTION::getPrev(HFUNCTION function) {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 92, function) );
}

HFUNCTION vHFUNCTION::getNext(HFUNCTION function) {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 93, function) );
}


const char *vHFUNCTION::getScriptName() const {
  RmxStr( (VMSG::FUNCTION_BASE, 3, const_cast<vHFUNCTION*>(this)) );
}

const char *vHFUNCTION::getFunctionName() const {
  RmxStr( (VMSG::FUNCTION_BASE, 20, const_cast<vHFUNCTION*>(this)) );
}

int vHFUNCTION::getParamCount() const {
  RmxInt( (VMSG::FUNCTION_BASE, 4, const_cast<vHFUNCTION*>(this)) );
}

const char *vHFUNCTION::getToolTip() const {
  RmxStr( (VMSG::FUNCTION_BASE, 5, const_cast<vHFUNCTION*>(this)) );
}


int vHFUNCTION::getType() const {
  RmxInt( (VMSG::FUNCTION_BASE, 8, const_cast<vHFUNCTION*>(this)) );
}

BOOL vHFUNCTION::isEnabled() const {
  RmxInt( (VMSG::FUNCTION_BASE, 6, const_cast<vHFUNCTION*>(this)) );
}

BOOL vHFUNCTION::isVisible() const {
  RmxInt( (VMSG::FUNCTION_BASE, 7, const_cast<vHFUNCTION*>(this)) );
}

HFUNCTION vHFUNCTION::getParent() const {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 9, const_cast<vHFUNCTION*>(this)) );
}

HFUNCTION vHFUNCTION::child_getFirst() const {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 10, const_cast<vHFUNCTION*>(this)) );
}

HFUNCTION vHFUNCTION::child_getLast() const {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 11, const_cast<vHFUNCTION*>(this)) );
}

HFUNCTION vHFUNCTION::child_getPrev(HFUNCTION script) const {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 12, const_cast<vHFUNCTION*>(this), script) );
}

HFUNCTION vHFUNCTION::child_getNext(HFUNCTION script) const {
  RmxPtr( HFUNCTION, (VMSG::FUNCTION_BASE, 13, const_cast<vHFUNCTION*>(this), script) );
}

HPLUGIN vHFUNCTION::getPlugin() const {
  RmxPtr( HPLUGIN, (VMSG::FUNCTION_BASE, 21, const_cast<vHFUNCTION*>(this)) );
}

HLIST vHFUNCTION::getList() const {
  RmxPtr( HLIST, (VMSG::FUNCTION_BASE, 22, const_cast<vHFUNCTION*>(this)) );
}


//-----------------------------------------------------------------------



BOOL funcGroup::script_isValueNull(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 100, getListHandle(), name) );
}

BOOL funcGroup::script_isValueBoolean(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 101, getListHandle(), name) );
}

BOOL funcGroup::script_isValueInteger(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 102, getListHandle(), name) );
}

BOOL funcGroup::script_isValueNumber(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 103, getListHandle(), name) );
}

BOOL funcGroup::script_isValueString(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 104, getListHandle(), name) );
}


void funcGroup::script_setValueBoolean(const char *name, BOOL value) {
  RmxNul( (VMSG::FUNCTION_BASE, 111, getListHandle(), name, value) );
}

void funcGroup::script_setValueInteger(const char *name, int value) {
  RmxNul( (VMSG::FUNCTION_BASE, 112, getListHandle(), name, value) );
}

void funcGroup::script_setValueNumber(const char *name, double value) {
  RmxNul( (VMSG::FUNCTION_BASE, 113, getListHandle(), name, value) );
}

void funcGroup::script_setValueString(const char *name, const char *value) {
  RmxNul( (VMSG::FUNCTION_BASE, 114, getListHandle(), name, value) );
}


BOOL funcGroup::script_getValueBoolean(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 121, getListHandle(), name) );
}

int funcGroup::script_getValueInteger(const char *name) const {
  RmxInt( (VMSG::FUNCTION_BASE, 121, getListHandle(), name) );
}

double funcGroup::script_getValueNumber(const char *name) const {
  RmxNum( (VMSG::FUNCTION_BASE, 121, getListHandle(), name) );
}

const char *funcGroup::script_getValueString(const char *name) const {
  RmxStr( (VMSG::FUNCTION_BASE, 121, getListHandle(), name) );
}



//-----------------------------------------------------------------------



BOOL vHFUNCTION::global_isValueNull(const char *name) {
  RmxInt( (VMSG::SYSTEM, 600, name) );
}

BOOL vHFUNCTION::global_isValueBool(const char *name) {
  RmxInt( (VMSG::SYSTEM, 601, name) );
}

BOOL vHFUNCTION::global_isValueInt(const char *name) {
  RmxInt( (VMSG::SYSTEM, 602, name) );
}

BOOL vHFUNCTION::global_isValueNumber(const char *name) {
  RmxInt( (VMSG::SYSTEM, 603, name) );
}

BOOL vHFUNCTION::global_isValueString(const char *name) {
  RmxInt( (VMSG::SYSTEM, 604, name) );
}



void vHFUNCTION::global_setValueBoolean(const char *name, BOOL value) {
  RmxNul( (VMSG::SYSTEM, 611, name, (int)value) );
}

void vHFUNCTION::global_setValueInteger(const char *name, int value) {
  RmxNul( (VMSG::SYSTEM, 612, name, value) );
}

void vHFUNCTION::global_setValueNumber(const char *name, double value) {
  RmxNul( (VMSG::SYSTEM, 613, name, value) );
}

void vHFUNCTION::global_setValueString(const char *name, const char *value) {
  RmxNul( (VMSG::SYSTEM, 614, name, value) );
}



BOOL vHFUNCTION::global_getValueBoolean(const char *name) {
  RmxInt( (VMSG::SYSTEM, 621, name) );
}

int vHFUNCTION::global_getValueInteger(const char *name) {
  RmxInt( (VMSG::SYSTEM, 622, name) );
}

double vHFUNCTION::global_getValueNumber(const char *name) {
  RmxNum( (VMSG::SYSTEM, 623, name) );
}

const char *vHFUNCTION::global_getValueString(const char *name) {
  RmxStr( (VMSG::SYSTEM, 624, name) );
}


const char *vHFUNCTION::global_getValueSafe(const char *name, const char *safe) {
  const char *ret = global_getValueString(name);
  return (ret) ? (ret) : (safe);
}

