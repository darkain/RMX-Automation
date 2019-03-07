/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "WS2_32.lib")


#include "web_server.h"
#include "../../libraries/jnetlib/jnetlib.h"


static vWebServer *WEBSERVER = new vWebServer();


vWebServer::vWebServer() : plugBase("Web Server") {
  server = NULL;
}


void vWebServer::onInit() {
  JNL::open_socketlib();

  tree = new vWebTree(getName(), this);
  server = new vMyServer();
}


void vWebServer::onQuit() {
  vdelnull(server);
  JNL::close_socketlib();
}

//------------------------------------------------------------


vWebTree::vWebTree(const char *name, cfgBase *parent) : cfgTree(name, parent) {
  new vAddWebButton(this);
  new cfgLine(this);
}


vWebTree::~vWebTree() {
}


cfgBase *vWebTree::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  const char *unique = paramlist->getParam("unique");

  if (type  &&  VSTRCMP(type, "button")==0) {
    return addEvent(unique);
  }

  return cfgTree::onNewExternalChild(name, type, paramlist);
}


evntSingle *vWebTree::addEvent(const char *unique) {
  char str[32] = "";
  if (!unique) {
    VPRINTF(str, sizeof(str), "%d", rand());
    unique = str;
  }

  evntSingle *event = new evntSingle("Quick Link", this, NULL);
  event->insertParam("unique", unique);
  event->setParamReadOnly("unique", TRUE);

  new cfgText("Image File", event);
  new cfgMoveParentUp(event);
  new cfgMoveParentDown(event);
  new cfgRemoveParent("Remove Quick Link", event);
  event->initConfig(); //TODO: temporary fix
  return event;
}


//------------------------------------------------------------


IPageGenerator *vMyServer::onConnection(JNL_HTTPServ *serv, int port) {
  serv->set_reply_header("Server:jnetlib_test/0.0");
  const char *file = serv->get_request_file();
  const char *ext = "";

  const char *tmp = file;
  while (*tmp) {
    if (*tmp == '.') ext = tmp + 1;
    tmp++;
  }

  if (!strcmp(file, "/")) {
    serv->set_reply_string("HTTP/1.1 200 OK");
    serv->set_reply_header("Content-Type:text/html");
    serv->send_reply();

    return new vHome(serv);
  }

  if ( !VSTRCMPI(ext, "jpg")  ||  !VSTRCMPI(ext, "gif")  ||
       !VSTRCMPI(ext, "png")  ||  !VSTRCMPI(ext, "bmp")  ||
       !VSTRCMPI(ext, "txt")  ||  !VSTRCMPI(ext, "htm")  ||
       !VSTRCMPI(ext, "html") ||  !VSTRCMPI(ext, "jpeg") ||
       !VSTRCMPI(ext, "ico")  ||  !VSTRCMPI(ext, "css")  ||
       !VSTRCMPI(ext, "js")   ||  !VSTRCMPI(ext, "xml")  ) {

    char PATH[MAX_PATH] = "";
    VPRINTF(PATH, sizeof(PATH), "%s\\rmx_web%s", vHPLUGIN::getMaster()->getPath() , file);
    
    if (VFEXISTS(PATH)) {
      serv->set_reply_string("HTTP/1.1 200 OK");         //todo: send headers in vWebFile, so it can handle file IO errors
      serv->set_reply_header("Content-Type:text/html");  //todo: seperate mime-types for each extension
      serv->send_reply();
      return new vWebFile(serv, PATH);
    }
  }

  if (!VSTRCMPI(ext, "lph")) {
    char PATH[MAX_PATH] = "";
    VPRINTF(PATH, sizeof(PATH), "%s\\rmx_web%s", vHPLUGIN::getMaster()->getPath() , file);

    if (VFEXISTS(PATH)) {
      serv->set_reply_string("HTTP/1.1 200 OK");         //todo: send headers in vWebFile, so it can handle file IO errors
      serv->set_reply_header("Content-Type:text/html");  //todo: seperate mime-types for each extension
      serv->send_reply();

      FILE *tmp = tmpfile();                             //todo: this probably needs to be changed with tmpnam() or tempnam()

      return new vLuaFile(serv, PATH, tmp);
    }
  }

  serv->set_reply_string("HTTP/1.1 404 NOT FOUND");
  serv->send_reply();
  return NULL;
}


//------------------------------------------------------------


vWebFile::vWebFile(JNL_HTTPServ *server, const char *filename, FILE *handle) : name(filename) {
  serv = server;

  if (handle) {
    file = handle;
  } else {
    file = fopen(name.getName(), "r+b");
  }
}


vWebFile::~vWebFile() {
  if (file) fclose(file);
}


int vWebFile::GetData(char *buf, int size) {
  if (!file) return 0;
  return fread(buf, 1, size, file);
}


//------------------------------------------------------------


vLuaFile::vLuaFile(JNL_HTTPServ *server, const char *luafile, FILE *tempfile)
 : vWebFile(server, NULL, tempfile), consBase("Web Script") {

  FILE *flua = fopen(luafile ,"r+b");
  int len = VFSIZE(luafile);

  char *buffer = (char*) malloc(len);
  fread(buffer, 1, len, flua);

  fclose(flua);
  flua = NULL;

  grabActiveHandle();
  parse(buffer, len);
  releaseActiveHandle();

  free(buffer);
  rewind(tempfile);
}


vLuaFile::~vLuaFile() {
}


void vLuaFile::print(const char *s) {
  fwrite(s, 1, VSTRLEN(s), getFile());
}


void vLuaFile::parse(char *buffer, int len) {
  bool inCode = false;
  int start = 0;

  for (int i=0; i<len; i++) {
    if (inCode == false) {
      if (buffer[i] == '<'  &&  i<len-5  && buffer[i+1] == '?') {
        fwrite(buffer + start, 1, i-start, getFile());
        inCode = true;
        start = i+2;
      }
    } else {
      if (buffer[i] == '?'  &&  i<len-1  && buffer[i+1] == '>') {
        inCode = false;
        buffer[i] = 0;
        runScript(buffer + start);
        start = i+2;
      }
    }
  }

  if (start < len) {
    fwrite(buffer + start, 1, len-start, getFile());
  }
}


void vLuaFile::runScript(const char *s) {
  WEBSERVER->consoleCommand(s);
}


//------------------------------------------------------------


vHome::vHome(JNL_HTTPServ *serv) {
  //whats our active pane?
  const char *str_plugin = serv->get_request_parm("vplugin");
  const char *str_list   = serv->get_request_parm("vlist");
  vplugin = (HPLUGIN) ((str_plugin) ? atoi(str_plugin) : NULL);
  vlist   = (HLIST)   ((str_list)   ? atoi(str_list)   : NULL);
  if (!vplugin->root_hasList(vlist)) vlist = NULL;

  //is this a valid item?
  if (vlist) {
    char str[64] = "";
    VPRINTF(str, sizeof(str), "&vplugin=%d&vlist=%d", vplugin, vlist);
    params.setName(str);
  } else {
    params.setName("");
  }

  if (serv->postdatasize() > 0) {
    processSubmit(serv);
  }

  processCommand(serv);

  name.setName(tmpnam(NULL));
  file = fopen(name.getName(), "w+b");
  if (!file) return;

  //generate file
  fprintf(file, "<html>\n<head>\n<title>RM-X Web Server</title>\n");
  fprintf(file, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\" />\n");
  fprintf(file, "<script type=\"text/javascript\">prm = '%s';</script>\n", vlist?prm():"");
  fprintf(file, "<script type=\"text/javascript\" src=\"script.js\"></script>\n");
  fprintf(file, "</head>\n<body>\n");

  generateLinks();
  
  fprintf(file, "<form action=\"/?x=1%s\" method=\"post\">\n", prm());
  fprintf(file, "<table align=\"center\" class=\"maintable\" cellspacing=\"0\"><tr><td class=\"left\" nowrap=\"nowrap\" valign=\"top\">");

  //build left menu
  HPLUGIN plugin = vHPLUGIN::getFirst();
  while (plugin) {
    HLIST list = plugin->root_getFirstChild();
    while (list) {
      enumList(plugin, list);
      list = plugin->root_getNextChild(list);
    }
    plugin = vHPLUGIN::getNext(plugin);
  }

  fprintf(file, "\n\n</td><td class=\"right\" valign=\"top\">\n\n");

  //right pane title
  const char *title = vplugin->list_getParam(vlist, "title");
  if (!title) title = vplugin->list_getName(vlist);
  if (!title) title = "";
  fprintf(file, "<div class=\"title\">%s</div>\n", title);

  //right pane items
  generatePane(vplugin, vlist);

  //end of page
  fprintf(file, "\n</td></tr><tr><td colspan=\"2\" class=\"apply\">\n");
  fprintf(file, "<input type=\"submit\" name=\"submit\" value=\"Apply Changes\" />");
  fprintf(file, "\n</td></tr></table>\n");
  fprintf(file, "\n</form>\n\n</body>\n</html>");

  rewind(file);
}


vHome::~vHome() {
  fclose(file);
  remove(name.getName());
}


int vHome::GetData(char *buf, int size) {
  return fread(buf, 1, size, file);
}


void vHome::enumList(HPLUGIN plugin, HLIST list, int depth) {
  if (!plugin->list_isVisible(list)) return;
  const char *type = plugin->list_getType(list);
  if (!type) return;
  if (VSTRCMP(type, "tree") != 0) return;

  if (hasVisibleTree(plugin, list)) {
    if (plugin->list_isExpanded(list)) {
      fprintf(file, "<div><a href=\"/?action=unexpand&plugin=%d&list=%d%s\" class=\"plusminus\">-</a>", plugin, list, prm());
    } else {
      fprintf(file, "<div><a href=\"/?action=expand&plugin=%d&list=%d%s\" class=\"plusminus\">+</a>", plugin, list, prm());
    }
  } else {
    fprintf(file, "<div><span class=\"plusminus\">&nbsp;</span>");
  }

  fprintf(file, "<a class=\"menu\" href=\"/?vplugin=%d&vlist=%d\">%s</a>", plugin, list, plugin->list_getName(list));

  if (plugin->list_isExpanded(list)) {
    HLIST child = plugin->list_getFirstChild(list);
    while (child) {
      enumList(plugin, child, depth + 1);
      child = plugin->list_getNextChild(list, child);
    }
  }

  fprintf(file, "</div>\n");
}


BOOL vHome::hasVisibleTree(HPLUGIN plugin, HLIST list) {
  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    if (plugin->list_isVisible(child)) {
      const char *type = plugin->list_getType(child);
      if (type  &&  VSTRCMP(type, "tree")==0) {
        return TRUE;
      }
    }
    child = plugin->list_getNextChild(list, child);
  }
  return FALSE;
}


void vHome::generateLinks() {
  fprintf(file, "<div class=\"quicklink\">");

  cfgBase *child = WEBSERVER->getTree()->getFirstChild();
  while (child) {
    if (child->hasChildren()) {
      cfgBase *ptrurl = child->getChild("Image File");
      const char *url = (ptrurl) ? (ptrurl->getUserData()) : (NULL);
      if (url) {
        fprintf(file, "<a href=\"/?action=quicklink&plugin=%d&list=%d%s\"><img src=\"%s\" /></a>\n", WEBSERVER->getThisPluginHandle(), child, prm(), url);
      }
    }
    child = WEBSERVER->getTree()->getNextChild(child);
  }

  fprintf(file, "</div>\n\n");
}


void vHome::generatePane(HPLUGIN plugin, HLIST list) {
  if (!plugin  ||  !list) return;

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    const char *type = plugin->list_getType(child);
    if (type  &&  plugin->list_isVisible(child)) {
      if (VSTRCMP(type, "button") == 0) {
        printButton(vplugin, child);
      } else if (VSTRCMP(type, "string") == 0) {
        printString(vplugin, child);
      } else if (VSTRCMP(type, "bool") == 0) {
        printBool(vplugin, child);
      } else if (VSTRCMP(type, "path") == 0) {
        printText(vplugin, child);
      } else if (VSTRCMP(type, "link") == 0) {
        printLink(vplugin, child);
      } else if (VSTRCMP(type, "null") == 0) {
        printText(vplugin, child);
      } else if (VSTRCMP(type, "int") == 0) {
        printInt(vplugin, child);
      } else if (VSTRCMP(type, "hr") == 0) {
        fprintf(file, "<div class=\"line\">&nbsp;</div>\n");
      }
      fprintf(file, "\n\n");
    }
    child = plugin->list_getNextChild(list, child);
  }
}


void vHome::printExpand(HPLUGIN plugin, HLIST list) {
  BOOL hasChildren = FALSE;

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    const char *type = vplugin->list_getType(child);
    if (type  &&  vplugin->list_isVisible(child)) {
      if (VSTRCMP(type, "button") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "string") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "bool") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "path") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "link") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "null") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "int") == 0) {
        hasChildren = TRUE; break;
      } else if (VSTRCMP(type, "hr") == 0) {
        hasChildren = TRUE; break;
      }
    }
    child = plugin->list_getNextChild(list, child);
  }

  if (hasChildren) {
    if (plugin->list_isExpanded(list)) {
      fprintf(file, "<a href=\"/?action=unexpand&plugin=%d&list=%d%s\" class=\"plusminus\">-</a>", plugin, list, prm());
    } else {
      fprintf(file, "<a href=\"/?action=expand&plugin=%d&list=%d%s\" class=\"plusminus\">+</a>", plugin, list, prm());
    }
  } else {
    fprintf(file, "<span class=\"plusminus\">&nbsp;</span>");
  }
}


void vHome::printButton(HPLUGIN plugin, HLIST list) {
  fprintf(file, "<div><table cellspacing=\"0\" cellpadding=\"0\"><tr><td class=\"label\">");
  printExpand(plugin, list);

  fprintf(file, "%s</td><td class=\"desc\">%s</td><td class=\"text\"><button onclick=\"btn(%d,%d)\">%s</button></td></tr></table>",
    vplugin->list_getName(list), vplugin->list_getParam(list, "desc"), plugin, list, vplugin->list_getParam(list, "text"));

  if (plugin->list_isExpanded(list)) generatePane(plugin, list);
  fprintf(file, "</div>");
}


void vHome::printLink(HPLUGIN plugin, HLIST list) {
  fprintf(file, "<div>");
  printExpand(plugin, list);

  fprintf(file, "<a href=\"%s\">%s</a>", vplugin->list_getUserData(list), vplugin->list_getName(list));
 
  if (plugin->list_isExpanded(list)) generatePane(plugin, list);
  fprintf(file, "</div>");
}


void vHome::printString(HPLUGIN plugin, HLIST list) {
  fprintf(file, "<div><table cellspacing=\"0\" cellpadding=\"0\"><tr><td class=\"label\">");
  printExpand(plugin, list);

  const char *data = vplugin->list_getUserData(list);
  if (!data) data = "";

  fprintf(file, "%s</td><td class=\"text\"><input name=\"%d\" type=\"text\" value=\"%s\" /></td></tr></table>",
    vplugin->list_getName(list), list, data);
  
  if (plugin->list_isExpanded(list)) generatePane(plugin, list);
  fprintf(file, "</div>");
}


void vHome::printInt(HPLUGIN plugin, HLIST list) {
  fprintf(file, "<div><table cellspacing=\"0\" cellpadding=\"0\"><tr><td class=\"label\">");
  printExpand(plugin, list);
  
  const char *data = vplugin->list_getUserData(list);
  if (!data) data = "";

  fprintf(file, "%s</td><td class=\"text\"><input name=\"%d\" type=\"text\" value=\"%s\" /></td></tr></table>",
    vplugin->list_getName(list), list, data);
  
  if (plugin->list_isExpanded(list)) generatePane(plugin, list);
  fprintf(file, "</div>");
}


void vHome::printBool(HPLUGIN plugin, HLIST list) {
  fprintf(file, "<div>");
  printExpand(plugin, list);

  fprintf(file, "%s", vplugin->list_getName(list));

  if (plugin->list_isExpanded(list)) generatePane(plugin, list);
  fprintf(file, "</div>");
}


void vHome::printText(HPLUGIN plugin, HLIST list) {
  fprintf(file, "<div>");
  printExpand(plugin, list);

  fprintf(file, "%s", vplugin->list_getName(list));

  if (plugin->list_isExpanded(list)) generatePane(plugin, list);
  fprintf(file, "</div>");
}


void vHome::processCommand(JNL_HTTPServ *serv) {
  const char *action = serv->get_request_parm("action");
  if (!action) return;

  const char *str_plugin = serv->get_request_parm("plugin");
  const char *str_list   = serv->get_request_parm("list");

  HPLUGIN plugin = (HPLUGIN) ((str_plugin) ? atoi(str_plugin) : NULL);
  HLIST   list   = (HLIST)   ((str_list)   ? atoi(str_list)   : NULL);
  if (!plugin->root_hasList(list)) list = NULL;

  if (VSTRCMP(action, "expand") == 0) {
    plugin->list_setExpanded(list, TRUE);
  }

  else if (VSTRCMP(action, "unexpand") == 0) {
    plugin->list_setExpanded(list, FALSE);
  }

  else if (VSTRCMP(action, "button") == 0) {
    plugin->list_message(list, GENERIC_CLICK);
  }

  else if (VSTRCMP(action, "quicklink") == 0) {
    cfgBase *mylist = (cfgBase*)list;
    cfgBase *child  = mylist->getChild("Image File");
    if (!child) return;
    evntSingle *event = static_cast<evntSingle*>(mylist);
    event->activate();
  }
}


void vHome::processSubmit(JNL_HTTPServ *serv) {
  vPostParam params;
  char *input = serv->getpostdata();
  if (!input) return;


  {
    postparam *post = (postparam*) malloc(sizeof(postparam));
    post->param = input;
    post->value = NULL;
    params.appendItem(post);

    while (*input!=NULL  &&  *input!='&'  &&  *input!='=') input++;
    if (*input == '=') {
      *input = NULL;
      input++;
      post->value = input;
    }
  }

  while (*input) {
    if (*input == '&') {
      *input = NULL;
      input++;

      postparam *post = (postparam*) malloc(sizeof(postparam));
      post->param = input;
      post->value = NULL;
      params.appendItem(post);

      while (*input!=NULL  &&  *input!='=') input++;
      if (*input == '=') {
        *input = NULL;
        input++;
        post->value = input;
      }
    }
    input++;
  }

  
  //url decode params
  char *urldecode = (char*) malloc(4096);
  postparam *item = params.getFirstItem();
  while (item) {
    if (item->value) {
      *urldecode = NULL;
      WebServerBaseClass::url_decode(item->value, urldecode, 4096);
      int len = strlen(urldecode) + 1;
      item->value = (char*)malloc(len);
      memcpy(item->value, urldecode, len);
    }
    item = params.getNextItem(item);
  }
  free(urldecode);

  char *submit = params.getParam("submit");
  if (!submit) return;
  if (VSTRCMP(submit, "Apply Changes") != 0) return;


  //parse params, and apply userdata
  item = params.getFirstItem();
  while (item) {
    if (item->param) {
      HLIST itemlist = (HLIST) atoi(item->param);
      if (vplugin->list_hasChild(vlist, itemlist, TRUE)) {
        if (vplugin->list_isEnabled(itemlist)) {
          vplugin->list_setUserData(itemlist, item->value);
        }
      }
    }
    item = params.getNextItem(item);
  }
}
