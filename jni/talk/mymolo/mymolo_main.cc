/*
 * mymolo_main.cc
 *
 *  Created on: 2012-5-19
 *      Author: gongchen
 */
#include <jni.h>

#include <cstdio>
#include <cstring>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <vector>     

#include "talk/base/flags.h"
#include "talk/base/logging.h"
#include "talk/base/sigslot.h"
#include "talk/examples/login/xmppthread.h"
#include "talk/examples/login/xmppauth.h"
#include "talk/examples/login/xmpppump.h"
#include "talk/mymolo/console.h"
#include "talk/mymolo/moloclient.h"


class DebugLog : public sigslot::has_slots<> {
 public:
  DebugLog() :
    debug_input_buf_(NULL), debug_input_len_(0), debug_input_alloc_(0),
    debug_output_buf_(NULL), debug_output_len_(0), debug_output_alloc_(0),
    censor_password_(false)
      {}
  char * debug_input_buf_;
  int debug_input_len_;
  int debug_input_alloc_;
  char * debug_output_buf_;
  int debug_output_len_;
  int debug_output_alloc_;
  bool censor_password_;

  void Input(const char * data, int len) {
    if (debug_input_len_ + len > debug_input_alloc_) {
      char * old_buf = debug_input_buf_;
      debug_input_alloc_ = 4096;
      while (debug_input_alloc_ < debug_input_len_ + len) {
        debug_input_alloc_ *= 2;
      }
      debug_input_buf_ = new char[debug_input_alloc_];
      memcpy(debug_input_buf_, old_buf, debug_input_len_);
      delete[] old_buf;
    }
    memcpy(debug_input_buf_ + debug_input_len_, data, len);
    debug_input_len_ += len;
    DebugPrint(debug_input_buf_, &debug_input_len_, false);
  }

  void Output(const char * data, int len) {
    if (debug_output_len_ + len > debug_output_alloc_) {
      char * old_buf = debug_output_buf_;
      debug_output_alloc_ = 4096;
      while (debug_output_alloc_ < debug_output_len_ + len) {
        debug_output_alloc_ *= 2;
      }
      debug_output_buf_ = new char[debug_output_alloc_];
      memcpy(debug_output_buf_, old_buf, debug_output_len_);
      delete[] old_buf;
    }
    memcpy(debug_output_buf_ + debug_output_len_, data, len);
    debug_output_len_ += len;
    DebugPrint(debug_output_buf_, &debug_output_len_, true);
  }

  static bool IsAuthTag(const char * str, size_t len) {
    if (str[0] == '<' && str[1] == 'a' &&
                         str[2] == 'u' &&
                         str[3] == 't' &&
                         str[4] == 'h' &&
                         str[5] <= ' ') {
      std::string tag(str, len);

      if (tag.find("mechanism") != std::string::npos)
        return true;
    }
    return false;
  }

  void DebugPrint(char * buf, int * plen, bool output) {
    int len = *plen;
    if (len > 0) {
      time_t tim = time(NULL);
      struct tm * now = localtime(&tim);
      char *time_string = asctime(now);
      if (time_string) {
        size_t time_len = strlen(time_string);
        if (time_len > 0) {
          time_string[time_len-1] = 0;    // trim off terminating \n
        }
      }
			LOG(INFO)
					<< (output ?
							"SEND >>>>>>>>>>>>>>>>" : "RECV <<<<<<<<<<<<<<<<")
					<< " : " << time_string;

      bool indent;
      int start = 0, nest = 3;
      for (int i = 0; i < len; i += 1) {
        if (buf[i] == '>') {
          if ((i > 0) && (buf[i-1] == '/')) {
            indent = false;
          } else if ((start + 1 < len) && (buf[start + 1] == '/')) {
            indent = false;
            nest -= 2;
          } else {
            indent = true;
          }

          // Output a tag
          LOG(INFO) << std::setw(nest) << " "
                    << std::string(buf + start, i + 1 - start);

          if (indent)
            nest += 2;

          // Note if it's a PLAIN auth tag
          if (IsAuthTag(buf + start, i + 1 - start)) {
            censor_password_ = true;
          }

          // incr
          start = i + 1;
        }

        if (buf[i] == '<' && start < i) {
          if (censor_password_) {
            LOG(INFO) << std::setw(nest) << " " << "## TEXT REMOVED ##";
            censor_password_ = false;
          } else {
            LOG(INFO) << std::setw(nest) << " "
                      << std::string(buf + start, i - start);
          }
          start = i;
        }
      }
      len = len - start;
      memcpy(buf, buf + start, len);
      *plen = len;
    }
  }
};

static DebugLog debug_log_;
static const int DEFAULT_PORT = 5222;

void Print(const char* chars) {
  printf("%s", chars);
  fflush(stdout);
}

int main(int argc, char **argv) {
	DEFINE_bool(d, false, "Turn on debugging.");

	// parse options
	FlagList::SetFlagsFromCommandLine(&argc, argv, true);

	bool debug = true;//FLAG_d;

	// parse username and password, if present
	buzz::Jid jid;
	std::string username;
	talk_base::InsecureCryptStringImpl pass;
//	if (argc > 1) {
//		username = argv[1];
//		if (argc > 2) {
//			pass.password() = argv[2];
//		}
//	}

	if (debug)
		talk_base::LogMessage::LogToDebug(talk_base::LS_VERBOSE);

	if (username.empty()) {
		Print("JID: ");
		std::cin >> username;
	}

	jid = buzz::Jid(username);
//	if (!jid.IsValid() || jid.node() == "") {
//		Print("Invalid JID. JIDs should be in the form user@domain\n");
//		return 1;
//	}

	if (pass.password().empty()) {
//		Console::SetEcho(false);
		Print("Password: ");
		std::cin >> pass.password();
//		Console::SetEcho(true);
		Print("\n");
	}

	buzz::XmppClientSettings xcs;
	xcs.set_user(jid.domain());
	xcs.set_host("molohui.com");
	xcs.set_use_tls(buzz::TLS_DISABLED);
	xcs.set_pass(talk_base::CryptString(pass));
	xcs.set_server(talk_base::SocketAddress("www.molohui.com", 5222));
	xcs.set_allow_plain(false);
	Print(("Logging in to " + xcs.server().ToString() + " as " + jid.Str() + "\n").c_str());

#if WIN32
	// Need to pump messages on our main thread on Windows.
	talk_base::Win32Thread w32_thread;
	talk_base::ThreadManager::Instance()->SetCurrentThread(&w32_thread);
#endif
	talk_base::Thread* main_thread = talk_base::Thread::Current();
	// #ifdef OSX
	//talk_base::MacCarbonAppSocketServer ss;
	//talk_base::SocketServerScope ss_scope(&ss);
	// #endif
//	talk_base::Thread* worker_thread_ = new talk_base::Thread();
//	worker_thread_->Start();
	XmppPump pump;
	MoloClient *client = new MoloClient(pump.client());

	Console *console = new Console(main_thread, client);
	client->SetConsole(console);
	console->Start();

	if (debug) {
		pump.client()->SignalLogInput.connect(&debug_log_, &DebugLog::Input);
		pump.client()->SignalLogOutput.connect(&debug_log_, &DebugLog::Output);
	}

//	thread.Login(xcs);
	pump.DoLogin(xcs, new XmppSocket(buzz::TLS_DISABLED), new XmppAuth());
	main_thread->Run();
	pump.DoDisconnect();

	console->Stop();
	delete console;
	delete client;

	return 0;
}
