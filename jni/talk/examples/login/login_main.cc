/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
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

#include <cstdio>
#include <iostream>

#include "talk/base/thread.h"
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/examples/login/xmppthread.h"
#include "talk/xmpp/xmppengine.h"

int main(int argc, char **argv) {
//  std::cout << "Password Cookie: ";
  std::cout<< "Password :";
  std::string auth_cookie;
  std::getline(std::cin, auth_cookie);

  std::cout << "User Name: ";
  std::string username;
  std::getline(std::cin, username);

  // Start xmpp on a different thread
  XmppThread thread;
  thread.Start();

  buzz::XmppClientSettings xcs;
  xcs.set_user(username.c_str());
  xcs.set_host("molohui.com");
  xcs.set_use_tls(buzz::TLS_DISABLED);
//  xcs.set_auth_cookie(auth_cookie.c_str());
  talk_base::InsecureCryptStringImpl pass;
  pass.password() = auth_cookie;
  xcs.set_pass(talk_base::CryptString(pass));
  xcs.set_server(talk_base::SocketAddress("www.molohui.com", 5222));
  thread.Login(xcs);

  // Use main thread for console input
  std::string line;
  while (std::getline(std::cin, line)) {
		if (line == "quit") {
			thread.Disconnect();
			break;
		}
  }
  thread.Stop();
  return 0;
}

