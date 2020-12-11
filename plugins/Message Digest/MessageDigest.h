/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
//#include <vConfigList/vEncoder/Encoder.h>
#include "../../sdk/func/funcSingle.h"


class MD5Plugin : public plugBase {
  public:
    MD5Plugin();

  protected:
    virtual void onInit();
    virtual void onQuit();

  private:
//    vEncoder *md2_encoder;
//    vEncoder *md4_encoder;
//    vEncoder *md5_encoder;
    funcSingle *md2_script;
    funcSingle *md4_script;
    funcSingle *md5_script;
};


/*
class MessageDigestHandle : public vEncoderHandle {
  public:
    MessageDigestHandle(vEncoder *parent, int ver) : vEncoderHandle(parent), version(ver) {}
    virtual ~MessageDigestHandle() {}

  public:
    virtual void encodeData(const void *rawdata, UINT rawlen);

  private:
    int version;
};


class MessageDigestEncode : public vEncoder {
  public:
    MessageDigestEncode(const char *name, int ver) : vEncoder(name), version(ver) { setCanEncode(TRUE); }
    virtual ~MessageDigestEncode() {}

    virtual vEncoderHandle *getEncoderInterface() { return new MessageDigestHandle(this, version); }

  private:
    int version;
};
*/


class MessageDigestScript : public funcSingle {
  public:
    MessageDigestScript(const char *script_name, int ver);
    virtual ~MessageDigestScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
    int version;
};
