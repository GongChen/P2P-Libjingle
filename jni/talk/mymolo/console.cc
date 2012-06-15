/*
 * console.cc
 *
 *  Created on: 2012-5-20
 *      Author: gongchen
 */

//#ifdef 
//#if defined(LINUX) || defined(ANDROID) 
#include <signal.h>
#include <termios.h>
#include <unistd.h>
//#endif  // POSIX
#include <cassert>
#include "talk/base/logging.h"
#include "talk/base/messagequeue.h"
#include "talk/base/stringutils.h"
#include "talk/mymolo/console.h"
#include "talk/mymolo/moloclient.h"

//#ifdef POSIX
static void DoNothing(int unused) {}
//#endif

Console::Console(talk_base::Thread *thread, MoloClient *moloclient) :
		client_(moloclient), client_thread_(thread), console_thread_(
				new talk_base::Thread()) {
}

Console::~Console() {
	Stop();
}

void Console::Start() {
	if (!console_thread_.get()) {
		// stdin was closed in Stop(), so we can't restart.
		LOG(LS_ERROR) << "Cannot re-start";
		return;
	}
	if (console_thread_->started()) {
		LOG(LS_WARNING) << "Already started";
		return;
	}
	console_thread_->Start();
	console_thread_->Post(this, MSG_START);
}

void Console::Stop() {
	if (console_thread_.get() && console_thread_->started()) {
#ifdef WIN32
		CloseHandle(GetStdHandle(STD_INPUT_HANDLE));
#else
		close(fileno(stdin));
		// This forces the read() in fgets() to return with errno = EINTR. fgets()
		// will retry the read() and fail, thus returning.
		pthread_kill(console_thread_->GetPThread(), SIGUSR1);
#endif
		console_thread_->Stop();
		console_thread_.reset();
	}
}

void Console::SetEcho(bool on) {
#ifdef WIN32
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
  if ((hIn == INVALID_HANDLE_VALUE) || (hIn == NULL))
    return;

  DWORD mode;
  if (!GetConsoleMode(hIn, &mode))
    return;

  if (on) {
    mode = mode | ENABLE_ECHO_INPUT;
  } else {
    mode = mode & ~ENABLE_ECHO_INPUT;
  }

  SetConsoleMode(hIn, mode);
#else
  const int fd = fileno(stdin);
  if (fd == -1)
   return;

  struct termios tcflags;
  if (tcgetattr(fd, &tcflags) == -1)
    return;

  if (on) {
    tcflags.c_lflag |= ECHO;
  } else {
    tcflags.c_lflag &= ~ECHO;
  }

  tcsetattr(fd, TCSANOW, &tcflags);
#endif
}

void Console::PrintLine(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  char buf[4096];
  int size = vsnprintf(buf, sizeof(buf), format, ap);
  assert(size >= 0);
  assert(size < static_cast<int>(sizeof(buf)));
  buf[size] = '\0';
  printf("%s\n", buf);
  fflush(stdout);

  va_end(ap);
}

void Console::RunConsole() {
  char input_buffer[128];
  while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
    client_thread_->Post(this, MSG_INPUT,
        new talk_base::TypedMessageData<std::string>(input_buffer));
  }
}

void Console::OnMessage(talk_base::Message *msg) {
  switch (msg->message_id) {
    case MSG_START:
//#ifdef POSIX
      // Install a no-op signal so that we can abort RunConsole() by raising
      // SIGUSR1.
      struct sigaction act;
      act.sa_handler = &DoNothing;
      sigemptyset(&act.sa_mask);
      act.sa_flags = 0;
      if (sigaction(SIGUSR1, &act, NULL) < 0) {
        LOG(LS_WARNING) << "Can't install signal";
      }
//#endif
      RunConsole();
      break;
    case MSG_INPUT:
      talk_base::TypedMessageData<std::string> *data =
          static_cast<talk_base::TypedMessageData<std::string>*>(msg->pdata);
      client_->ParseLine(data->data());
      break;
  }
}




