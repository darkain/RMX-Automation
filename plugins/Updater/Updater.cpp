/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


// todo: remove old files on startup, such as "RMX SA.old"


#error  OBSOLETE - RE-IMPLEMENT INTO RMX CORE DIRECTLY


#pragma comment(lib, "WS2_32.lib")


#include "Updater.h"
#include "md2.h"
#include "md4.h"
#include "huffman.h"


static Updater *UPDATER = new Updater();


Updater::Updater() : vPlug("Updates") {
  restart    = FALSE;
  lastcheck  = NULL;
  lastupdate = NULL;
  tree       = NULL;
}


void Updater::onInit() {
  lastcheck  = new vConfigInt("Last Check");
  lastupdate = new vConfigInt("Last Update");

  tree = new vUpdateTree();

  JNL::open_socketlib();
}


void Updater::onPostInit() {
  tree->update();
}


void Updater::onQuit() {
  JNL::close_socketlib();
}



/////////////////////////////////////////////////////////////////////////


vUpdateTree::vUpdateTree() : vConfigTree(UPDATER->getName()) {
  setSavable(FALSE);
  setTimeout(1);

  file   = NULL;

  text   = new vConfigNull("", this);
  text->setSavable(FALSE);
  updateText(0);

  button = new vUpdateButton(this);
  button->setEnabled(FALSE);
  button->setSavable(FALSE);

  vConfigLink *link = new vConfigLink("Click here to see the RM-X Change Log", this);
  link->setUrl("http://rmx.darkain.com/wiki/ChangeLog");
  link->setTooltip("http://rmx.darkain.com/wiki/ChangeLog");

  new vConfigLine(this);
}


vUpdateTree::~vUpdateTree() {
  if (file) fclose(file);
}


void vUpdateTree::update() {
  if (file) return;
  if (vUpdateItem::getNumItems() != 0) return;
  
  char path[MAX_PATH];
  char fullpath[MAX_PATH];

  if (GetTempPath(sizeof(path), path) != 0) {
    if (GetTempFileName(path, "RMX", 0, fullpath) != 0) {
      VFOPEN(&file, fullpath, "w+b");
      if (file) {
        filename.setName(fullpath);
        UserAgent(&http);
        http.connect(UPDATE_URL);
        startTimer();
      }
    }
  }
}


void vUpdateTree::onTimer(DWORD systemTime) {
  int ret = http.run();

  if ( (ret == 1)  |  (ret == 0) ) {
    int total = http.bytes_available();
    char *buffer = (char*)malloc(total);
    http.get_bytes(buffer, total);
    if (file) fwrite(buffer, 1, total, file);
    free(buffer);
  }

  if (ret == 1) { //we are done
    stopTimer();
    fclose(file);
    file = NULL;
    vUpdateReader reader(filename.getName(), this);
    remove(filename.getName());
    int num = vUpdateItem::getNumItems();
    if (num > 0) button->setEnabled(TRUE);
    updateText(num);
  }

  if (ret == -1) {  //ERROR!!!
    stopTimer();
    fclose(file);
    file = NULL;
    remove(filename.getName());

    char str[1024] = "";
    const char *err = http.geterrorstr();
    VPRINTF(str, sizeof(str), "ERROR: Download error! - %s", err);
    text->setName(str);
  }
}


void vUpdateTree::updateText(int num) {
  char str[64] = "There are currently no updates available for RM-X";
  if (UPDATER->needRestart()) {
    VSTRCPY(str, "RM-X needs to restart for changes to take effect", sizeof(str));

  }else if (num == 1) {
    VSTRCPY(str, "There is currently 1 update available for RM-X", sizeof(str));

  } else if (num > 1) {
    VPRINTF(str, sizeof(str), "There are currently %d updates available for RM-X", num);
  }

  text->setName(str);
}


void vUpdateTree::onClick(HPLUGIN source) {
  if (!source->isLocal()) return;

  vConfigList *child = getFirstChild();
  while (child) {
    child->setEnabled(FALSE);
    child = getNextChild(child);
  }

  vUpdateItem::startUpdate();
}


/////////////////////////////////////////////////////////////////////////


rmxList<vUpdateItem*> vUpdateItem::items;


vUpdateItem::vUpdateItem(const char *name, vConfigTree *parent) : vConfigBool(name, parent) {
  file    = NULL;
  filelen = 0;
  ZeroMemory(rmxhash, sizeof(rmxhash));

  setValue(TRUE);
  setExpanded(TRUE);
  setTimeout(1);

  text_cur = new vConfigText("Current version", this);
  text_cur->setEnabled(FALSE);

  text_new = new vConfigText("New version", this);
  text_new->setEnabled(FALSE);

  status = new vConfigNull("STATUS: Pending", this);

  insertParam("path");
  insertParam("file");
  insertParam("hash");
  insertParamInt("ver", 0);

  items.appendItem(this);
}


vUpdateItem::~vUpdateItem() {
  items.removeItem(this);
}


void vUpdateItem::onTimer(DWORD systemTime) {
  int ret = http.run();

  if ( (ret == 1)  |  (ret == 0) ) {
    int total = http.bytes_available();
    char *buffer = (char*)malloc(total);
    http.get_bytes(buffer, total);
    if (file) fwrite(buffer, 1, total, file);
    filelen += total;
    free(buffer);
  }

  if (ret == 1) { //we are done
    stopTimer();
    fflush(file);
    fseek(file, 0, 0);
    extract();
  }

  if (ret == -1) {  //ERROR!!!
    stopTimer();
    fclose(file);
    file = NULL;
    remove(filename.getName());

    char str[1024] = "";
    const char *err = http.geterrorstr();
    VPRINTF(str, sizeof(str), "STATUS: Download error! - %s", err);
    setStatus(str);
    onUpdateComplete();
  }
}


void vUpdateItem::onUpdateComplete() {
  vUpdateItem *item = items.getNextItem(this);
  if (item) {
    item->update();

  } else {  //all updates are complete
    item = items.getFirstItem();
    while (item) {
      vUpdateItem *next = items.getNextItem(item);
      if (!item->isVisible()) delete item;
      item = next;
    }

    int num = items.getItemCount();
/*    if (num > 0) {
      UPDATER->getTree()->getButton()->setEnabled(TRUE);
      item = items.getFirstItem();
      while (item) {
        item->setEnabled(TRUE);
        item = items.getNextItem(item);
      }
    }*/
    UPDATER->getTree()->updateText(num);   
  }
}


void vUpdateItem::onSetParam(const vConfigParam *param) {
  if (param->isNamed("hash")) {
    const char *hash = param->getValue();
    if (hash  &&  VSTRLEN(hash)==32) {
      for (int i=0; i<16; i++) {
        unsigned int val = 0;
        VSCANF(hash, "%02X", &val);
        rmxhash[i] = val & 0xFF;
        hash += 2;
      }
    }
  }
  vConfigBool::onSetParam(param);
}


void vUpdateItem::update() {
  if (!getValue()) {
    onUpdateComplete();
    return;
  }

  setStatus("STATUS: Starting");

  char url[MAX_PATH] = "";
  char path[MAX_PATH] = "";
  char fullpath[MAX_PATH] = "";
  char *name = UrlEncode( getName() );

  VPRINTF( url, sizeof(url), "%s%s.%d.rmx", UPDATE_URL, name, getParamInt("ver") );

  if (GetTempPath(sizeof(path), path) != 0) {
    if (GetTempFileName(path, "RMX", 0, fullpath) != 0) {
      VFOPEN(&file, fullpath, "w+b");
      if (file) {
        filename.setName(fullpath);
        UserAgent(&http);
        http.connect(url);
        startTimer();
      }
    }
  }

  free(name);
}


void vUpdateItem::extract() {
  int i=0;  //stupid MSVC

  setStatus("STATUS: Extracting");
  Sleep(1);

  char dest[MAX_PATH] = "";
  char destold[MAX_PATH] = "";
  VPRINTF(dest, sizeof(dest), "%s\\%s", getParamSafe("path"), getParamSafe("file"));
  VPRINTF(destold, sizeof(destold), "%s.old", dest);

  //TODO:  only remove old file if de-encryption is successful
  if (FileExists(destold)) remove(destold);
  rename(dest, destold);

  FILE *destfile = NULL;
  VFOPEN(&destfile, dest, "w+b");
  if (!destfile) {
    fclose(file);
    file = NULL;
    rename(destold, dest);
    setStatus("STATUS: Error creating file");
    return;
  }

  int len = 16;
  unsigned char *buffer = (unsigned char*) malloc(len);

  //XOR with RMX HASH, and create MD2 HASH
  MD2_CTX md2;
  MD2Init(&md2);
  int pos = 0;
  while (pos < filelen) {
    fseek(file, pos, SEEK_SET);
    int count = fread(buffer, 1, len, file);

    for (i=0; i<count; i++) {
      buffer[i] ^= rmxhash[i];
    }

    MD2Update(&md2, buffer, count);
    fseek(file, pos, SEEK_SET);
    fwrite(buffer, 1, count, file);
    pos += count;
  }
  unsigned char md2hash[16];
  MD2Final(md2hash, &md2);

  //create TEST HASH
  unsigned char tsthash[16];
  for (i=0; i<16; i++) {
    tsthash[i] = md2hash[i] ^ rmxhash[i];
  }

  //XOR with TEST HASH, and create MD4 HASH
  MD4_CTX md4;
  MD4Init(&md4);
  pos = 0;
  while (pos < filelen) {
    fseek(file, pos, SEEK_SET);
    int count = fread(buffer, 1, len, file);

    for (i=0; i<count; i++) {
      buffer[i] ^= tsthash[i];
    }

    MD4Update(&md4, buffer, count);
    fseek(file, pos, SEEK_SET);
    fwrite(buffer, 1, count, file);
    pos += count;
  }
  unsigned char md4hash[16];
  MD4Final(md4hash, &md4);

  //compare TEST to MD4
  BOOL ok = TRUE;
  for (i=0; i<16; i++) {
    if (tsthash[i] != md4hash[i]) ok = FALSE;
  }

  if (ok) {
    fseek(file, 0, SEEK_SET);
    huffman_decode_file(file, destfile);
  }

  //close files
  free(buffer);
  fflush(destfile);
  fclose(destfile);
  destfile = NULL;
  fflush(file);
  fclose(file);
  file = NULL;


  if (!ok) {
    remove(dest);
    rename(destold, dest);
    setStatus("STATUS: Error extracting file");
    return;
  }


  HPLUGIN plugin = vHPLUGIN::getByName(getName());
  if (plugin) {
    if (plugin == UPDATER->getThisPluginHandle()) {
      UPDATER->setNeedRestart(TRUE);
      setStatus("RM-X needs to restart for changes to take effect");
      onUpdateComplete();
    } else {
      setStatus("STATUS: Restarting plugin");
      plugin->unload();
      remove(destold);
      new vLoadPluginTimer(this);
    }
  }

  if (hasParam("core")) {  //is RM-X Core?
    UPDATER->setNeedRestart(TRUE);
    setStatus("RM-X needs to restart for changes to take effect");
    onUpdateComplete();
  }
}


void vUpdateItem::startUpdate() {
  vUpdateItem *item = items.getFirstItem();
  if (item) item->update();
}


/////////////////////////////////////////////////////////////////////////


vUpdateReader::vUpdateReader(const char *filename, vUpdateTree *parent) {
  tree     = parent;
  loadXmlFile(filename);
}


vUpdateReader::~vUpdateReader() {
}


void vUpdateReader::onElementStart(const char *xmlpath, const char *xmltag, vXmlParams *params) {
  HPLUGIN plugin = NULL;
  HLIST   ident  = NULL;
  const char *name  = params->getValue("name");
  const char *build = params->getValue("build");
  BOOL core = FALSE;

  if (!name || !build) return;

  if (VSTRCMP(xmltag, "core") == 0) {    //RM-X Core (Stand-Alone or Media Edition)
    core   = TRUE;
    plugin = vHPLUGIN::getMaster();
    ident  = plugin->root_getIdent();
    const char *appname = plugin->list_getParam(ident, "appname");
    if ( (!appname)  ||  (VSTRCMP(appname, name)!=0) ) ident = NULL;
  }

  if (VSTRCMP(xmltag, "plugin") == 0) {  //RM-X Plugin
    plugin = vHPLUGIN::getByName(name);
    ident  = plugin->root_getIdent();
  }


  if (ident  &&  plugin->isLocal()) {
    int curver = plugin->list_getParamInt(ident, "sdk");
    int xmlver = params->getValueInt("build");
    if (xmlver > curver) {
      vUpdateItem *item = new vUpdateItem(name, tree);
      item->setParam("path", plugin->getPath());
      item->setParam("file", plugin->getFilename());
      item->setParam("hash", params->getValue("hash"));
      item->setParamInt("ver", xmlver);
      item->setCurrentVersion(curver);
      item->setUpdateVersion( xmlver);
      if (core) item->insertParamInt("core", 1);
    }
  }
}


/////////////////////////////////////////////////////////////////////////


char *UrlEncode(const char *url) {  //todo: move this to vBase
  const char *tmp = url;
  int len = 0;
  while (*tmp) {
    tmp++;
    if (*tmp < 0x26  ||  *tmp > 0x7E) {
      len += 3;
    } else {
      len += 1;
    }
  }

  char *mem = (char*)malloc(len);
  char *ret = mem;  

  while (*url) {
    if (*url < 0x26  ||  *url > 0x7E) {
      VPRINTF(ret, 4, "%s%02X", "%", *url);
      ret += 3;
    } else {
      *ret = *url;
      ret++;
    }

    url++;
  }

  *ret = NULL;
  return mem;
}


void UserAgent(JNL_HTTPGet *http) {
  char str[128] = "";
  const char *platform = (vWin32::getOsVersionPlatform()==2) ? " NT" : "";

  HPLUGIN master = vHPLUGIN::getMaster();
  HLIST   ident  = master->root_getIdent();
  const char *ver  = master->list_getParam(ident, "title");
  const char *host = master->list_getParam(ident, "host" );

  VPRINTF(str, sizeof(str), "User-Agent:Mozilla/4.0 (compatible; %s; Windows%s %d.%d; %s) vBase/%d",
    ver, platform, vWin32::getOsVersionMajor(), vWin32::getOsVersionMinor(), host,
    UPDATER->getIdentification()->getParamInt("sdk"));

  http->addheader(str);
}


/////////////////////////////////////////////////////////////////////////


void vLoadPluginTimer::onTimer(DWORD systemTime) {
  stopTimer();

  HPLUGIN plugin = vHPLUGIN::loadPlugin(item->getParam("file"));
  if (plugin) {
    item->setStatus("STATUS: Complete");
    remove(item->getOldName());
    item->setVisible(FALSE);
  } else {
    item->setStatus("STATUS: Error restarting plugin");
  }

  item->onUpdateComplete();
  delete this;
}
