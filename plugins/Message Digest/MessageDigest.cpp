/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "MessageDigest.h"
#include "../../libraries/encoders/md5.h"
#include "../../libraries/encoders/md2.h"
#include "../../libraries/encoders/md4.h"


static MD5Plugin *md5 = new MD5Plugin();


MD5Plugin::MD5Plugin() : plugBase("Message Digest") {
//  md2_encoder = NULL;
//  md4_encoder = NULL;
//  md5_encoder = NULL;

  md2_script = NULL;
  md4_script = NULL;
  md5_script = NULL;
}


void MD5Plugin::onInit() {
//  md2_encoder = new MessageDigestEncode("MD2", 2);
//  md4_encoder = new MessageDigestEncode("MD4", 4);
//  md5_encoder = new MessageDigestEncode("MD5", 5);

  md2_script = new MessageDigestScript("md2", 2);
  md4_script = new MessageDigestScript("md4", 4);
  md5_script = new MessageDigestScript("md5", 5);
}


void MD5Plugin::onQuit() {
//  vdelnull(md2_encoder);
//  vdelnull(md4_encoder);
//  vdelnull(md5_encoder);

  vdelnull(md2_script);
  vdelnull(md4_script);
  vdelnull(md5_script);
}


//-------------------------------------------------------------------------------------------------------

/*
void MessageDigestHandle::encodeData(const void *rawdata, UINT rawlen) {
  BYTE *digest = (BYTE*)malloc(16);
  ZeroMemory(digest, 16);

  if (version == 2) {
    MD2_CTX state;
    MD2Init(&state);
    MD2Update(&state, (BYTE*)rawdata, rawlen);
    MD2Final(digest, &state);
  } else if (version == 4) {
    MD4_CTX state;
    MD4Init(&state);
    MD4Update(&state, (BYTE*)rawdata, rawlen);
    MD4Final(digest, &state);
  } else if (version == 5) {
    md5_state_t state;
    md5_init(&state);
    md5_append(&state, (const md5_byte_t*)rawdata, rawlen);
    md5_finish(&state, digest);
  }

  setData(digest, 16);
}
*/

//-------------------------------------------------------------------------------------------------------


MessageDigestScript::MessageDigestScript(const char *script_name, int ver) : funcSingle(NULL, script_name), version(ver) {
  insertParamNoSave("data", "");
//  setTooltip("Returns hash code of specified string");
}


void MessageDigestScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  const char *rawdata = getParamSafe("data");
  UINT rawlen = VSTRLEN(rawdata);

  BYTE *digest = (BYTE*)malloc(16);
  ZeroMemory(digest, 16);

  if (version == 2) {
    MD2_CTX state;
    MD2Init(&state);
    MD2Update(&state, (BYTE*)rawdata, rawlen);
    MD2Final(digest, &state);
  } else if (version == 4) {
    MD4_CTX state;
    MD4Init(&state);
    MD4Update(&state, (BYTE*)rawdata, rawlen);
    MD4Final(digest, &state);
  } else if (version == 5) {
    md5_state_t state;
    md5_init(&state);
    md5_append(&state, (const md5_byte_t*)rawdata, rawlen);
    md5_finish(&state, digest);
  }

  char outstr[33];
  char *out = outstr;
  BYTE *digout = digest;
  for (int i=0; i<16; i++) {
    VPRINTF(out, 3, "%02X", *digout);
    out+=2;
    digout++;
  }

  source->list_insertParam(output, "prm1");
  source->list_setParam(output, "prm1", outstr);

  free(digest);
}
