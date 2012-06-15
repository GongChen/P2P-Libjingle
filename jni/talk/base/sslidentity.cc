/*
 * libjingle
 * Copyright 2004--2008, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Handling of certificates and keypairs for SSLStreamAdapter's peer mode.

#include <string>

#if HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

// Decide which (if any) implementation of SSL we will use.
#if !defined(SSL_USE_SCHANNEL) && !defined(SSL_USE_OPENSSL)
#ifdef WIN32
#define SSL_USE_SCHANNEL 1
#else  // !WIN32
#define SSL_USE_OPENSSL HAVE_OPENSSL_SSL_H
#endif  // !WIN32
#endif

#include "talk/base/sslidentity.h"

#if SSL_USE_SCHANNEL

#error "Not implemented yet"

#elif SSL_USE_OPENSSL  // && !SSL_USE_SCHANNEL

#include "talk/base/opensslidentity.h"

namespace talk_base {

// From RFC 4572
const char DIGEST_SHA_1[] = "sha-1";
const char DIGEST_SHA_224[] = "sha-224";
const char DIGEST_SHA_256[] = "sha-256";
const char DIGEST_SHA_384[] = "sha-384";
const char DIGEST_SHA_512[] = "sha-512";

SSLCertificate* SSLCertificate::FromPEMString(const std::string& pem_string,
                                              int* pem_length) {
  return OpenSSLCertificate::FromPEMString(pem_string, pem_length);
}

SSLIdentity* SSLIdentity::Generate(const std::string& common_name) {
  return OpenSSLIdentity::Generate(common_name);
}

}  // namespace talk_base

#else  // !SSL_USE_OPENSSL && !SSL_USE_SCHANNEL

#error "No SSL implementation"

#endif  // SSL_USE_OPENSSL/!SSL_USE_SCHANNEL
