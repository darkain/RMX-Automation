/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Test Plugin.h"


//static test t;
static test *t = new test();


test::test() : plugBase("Test Plugin") {
  root = NULL;
  froot = NULL;
//  script_table = NULL;
//  script_function = NULL;
//  script_vargs = NULL;
}


void test::onInit() {
  root = new cfgTree(getName());

//  makeevents();
//  dynamictest();
//  childrentest();
  datatypetest();
//  movetest();
//  testmenu();
//  functiontest();
//  scripttest();
}


void test::onQuit() {
  vdelnull(root);
  vdelnull(froot);
//  vdelnull(script_table);
//  vdelnull(script_function);
//  vdelnull(script_vargs);
}


void test::dynamictest() {
  cfgTree *dynTitle = new cfgTreeTitleChange("Dynamic Title", root);
  cfgTree *dynName = new cfgTreeNameChange("0", root);
  dynName->setTitle("Dynamic Name");

  cfgTree *addremove = new cfgTree("Add/Remove", root);
  new cfgAddItem("Add Item", addremove);
  new cfgInsertItem("Insert Item", addremove);
  new cfgAddTree("Add Tree", addremove);
  new cfgAddChild("Add Child", addremove);
  new cfgHideTree("Show/Hide Tree Item", addremove);
  new cfgLine(addremove);

  cfgTree *dynchildren = new cfgTree("Dynamic Params", root);
  cfgBase *showhide = new cfgShowHideChild("Show/Hide Child", dynchildren);
//  showhide->setTooltip("Dynamicly show/hide child list objects");

  new cfgNull("some text 1", showhide);
  new cfgNull("some text 2", showhide);
  new cfgNull("some text 3", showhide);
  new cfgLine(dynchildren);

  (new cfgButtonNameChange("name", dynchildren))->setButtonDescription("Dynamic Button Name");
  new cfgButtonDescriptionChange("Dynamic Button Description", dynchildren);
  new cfgButtonTextChange("Dynamic Button Text", dynchildren);
  (new cfgButtonEventChange("Dynamic Button onClick() Event", dynchildren))->setButtonDescription("0");
  new cfgLine(dynchildren);

  (new cfgTextNameChange("name", dynchildren))->setText("Dynamic EditText Name");
  new cfgTextTextChange("Dyanmic EditText text", dynchildren);
  (new cfgTextFocusChange("Focus Change", dynchildren))->setParamSavable("Text", TRUE);

}


void test::childrentest() {
  cfgTree *childc = new cfgTree("children", root);

  cfgTree *childc1 = new cfgTree("child 1", childc);
  cfgTree *childc2 = new cfgTree("child 2", childc);
  cfgTree *childc3 = new cfgTree("child 3", childc);
  cfgTree *childc4 = new cfgTree("child 4", childc);
  cfgTree *childc5 = new cfgTree("child 5", childc);
  cfgTree *childc6 = new cfgTree("child 6", childc);

  cfgTree *childc1a = new cfgTree("child 1a", childc1);
  cfgTree *childc1b = new cfgTree("child 1b", childc1);
  cfgTree *childc1c = new cfgTree("child 1c", childc1);
  cfgTree *childc1d = new cfgTree("child 1d", childc1);
  cfgTree *childc1e = new cfgTree("child 1e", childc1);
  cfgTree *childc1f = new cfgTree("child 1f", childc1);

  cfgTree *childc3a = new cfgTree("child 3a", childc3);
  cfgTree *childc3b = new cfgTree("child 3b", childc3);
  cfgTree *childc3c = new cfgTree("child 3c", childc3);
  cfgTree *childc3d = new cfgTree("child 3d", childc3);
  cfgTree *childc3e = new cfgTree("child 3e", childc3);
  cfgTree *childc3f = new cfgTree("child 3f", childc3);
}


void test::datatypetest() {
  cfgTree *types = new cfgTree("Data Types", root);

  evntSingle *e = new evntSingle("vSetting", types);
  new vSettingTest(e, types);
  new cfgLine(types);

  new cfgText("cfgText", types);
  new cfgInt("cfgInt", types);
  new cfgBool("cfgBool", types);
  new cfgLine(types);
  new cfgNull("cfgNull", types);
  (new cfgLabel("cfgLabel", types))->setLabel("cfgLabel");
  
  cfgIntSlider *slider = new cfgIntSlider("cfgInt - Slider Style", types);
  slider->setMin(-100);
  slider->setMax(100);

  new cfgPath("cfgPath", types);
}


void test::movetest() {
  cfgTree *moves = new cfgTree("Movable Items", root);

  cfgNull *up = new cfgNull("Up", moves);
  cfgNull *down = new cfgNull("Down", moves);
  cfgNull *begin = new cfgNull("Begining", moves);
  cfgNull *end = new cfgNull("Ending", moves);

  new cfgMoveUp("1", up);
  new cfgMoveUp("2", up);
  new cfgMoveUp("3", up);
  new cfgMoveUp("4", up);
  new cfgMoveUp("5", up);

  new cfgMoveDown("1", down);
  new cfgMoveDown("2", down);
  new cfgMoveDown("3", down);
  new cfgMoveDown("4", down);
  new cfgMoveDown("5", down);

  new cfgMoveBegining("1", begin);
  new cfgMoveBegining("2", begin);
  new cfgMoveBegining("3", begin);
  new cfgMoveBegining("4", begin);
  new cfgMoveBegining("5", begin);

  new cfgMoveEnding("1", end);
  new cfgMoveEnding("2", end);
  new cfgMoveEnding("3", end);
  new cfgMoveEnding("4", end);
  new cfgMoveEnding("5", end);
}


void test::makeevents() {
  cfgTree *events = new cfgTree("Events", root);

  char str[32];
  for (int i=0; i<16; i++) {
    VPRINTF(str, sizeof(str), "Event %1X", i);
    (new vSettingTest(new evntSingle(str, events), events))->setSavable(FALSE);;
    new cfgLine(events);
  }
}


void test::testmenu() {
  cfgMenu *menu = new cfgTestMenu("cfgMenu");
  cfgMenu *sub1 = new cfgMenu("Sub1", menu);
  cfgMenu *sub2 = new cfgMenu("Sub2", menu);
  cfgMenu *sub3 = new cfgMenu("Sub3", menu);
  cfgMenu *sub1a = new cfgMenu("Sub1a", sub1);
  cfgMenu *sub1b = new cfgMenu("Sub1b", sub1);
  cfgMenu *sub1c = new cfgMenu("Sub1c", sub1);
  cfgMenu *sub2a = new cfgMenu("Sub2a", sub2);
  cfgMenu *sub2b = new cfgMenu("Sub2b", sub2);
  cfgMenu *sub2c = new cfgMenu("Sub2c", sub2);
  cfgMenu *sub3a = new cfgMenu("Sub3a", sub3);
  cfgMenu *sub3b = new cfgMenu("Sub3b", sub3);
  cfgMenu *sub3c = new cfgMenu("Sub3c", sub3);
}


void test::functiontest() {
  froot = new funcGroup("funcSingle Group", "group");
  funcGroup *item1  = new funcGroup("Item 1",  "item1",  froot);
  funcGroup *item2  = new funcGroup("Item 2",  "item1",  froot);
  funcGroup *item3  = new funcGroup("Item 3",  "item1",  froot);
  funcGroup *item1a = new funcGroup("Item 1a", "item1a", item1);
  funcSingle *item1b = new funcTest("Item 1b", "item1b", item1);
  funcSingle *item1c = new funcTest("Item 1c", "item1c", item1);
  funcSingle *item2a = new funcTest("Item 2a", "item2a", item2);
  funcSingle *item2b = new funcTest("Item 2b", "item2b", item2);
  funcSingle *item2c = new funcTest("Item 2c", "item2c", item2);
  funcSingle *item3a = new funcTest("Item 3a", "item3a", item3);
  funcSingle *item3b = new funcTest("Item 3b", "item3b", item3);
  funcSingle *item3c = new funcTest("Item 3c", "item3c", item3);
  new funcTest("Item X", "itemx", item1a);
}

/*
void test::scripttest() {
  script_table = new vTestScriptTable();
  script_function = new vTestScriptFunction();
  script_vargs = new vTestScriptFunctionVargs();
}
*/