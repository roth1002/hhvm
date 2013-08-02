/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   | Copyright (c) 1998-2010 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/curl_tls_workarounds.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "openssl/ssl.h"
#include "hphp/runtime/base/runtime_error.h"
#include "hphp/runtime/base/runtime_option.h"

namespace HPHP {

CURLcode curl_tls_workarounds_cb(CURL *curl, void *sslctx, void *parm) {
  // Check to see if workarounds are enabled.
  SSL_CTX* ctx = (SSL_CTX*)sslctx;
  if (RuntimeOption::TLSDisableTLS1_2) {
#ifdef SSL_OP_NO_TLSv1_2
    SSL_CTX_set_options(ctx, SSL_CTX_get_options (ctx) | SSL_OP_NO_TLSv1_2);
#else
    raise_notice("TLSDisableTLS1_2 enabled, but this version of "
                 "SSL does not support that option");
#endif
  }
  if (!RuntimeOption::TLSClientCipherSpec.empty()) {
    SSL_CTX_set_cipher_list(ctx, RuntimeOption::TLSClientCipherSpec.c_str());
  }
  return CURLE_OK;
}

}
