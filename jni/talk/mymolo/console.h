/*
 * console.h
 *
 *  Created on: 2012-5-19
 *      Author: gongchen
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <cstdio>

#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/scoped_ptr.h"

class MoloClient;

class Console: public talk_base::MessageHandler {
public:
	Console(talk_base::Thread *thread, MoloClient *moloclient);
	~Console();

	// Starts reading lines from the console and giving them to the CallClient.
	void Start();
	// Stops reading lines. Cannot be restarted.
	void Stop();

	virtual void OnMessage(talk_base::Message *msg);

	void PrintLine(const char* format, ...);

	static void SetEcho(bool on);

private:
	enum {
		MSG_START, MSG_INPUT,
	};

	void RunConsole();
	void ParseLine(std::string &str);

	MoloClient *client_;
	talk_base::Thread *client_thread_;
	talk_base::scoped_ptr<talk_base::Thread> console_thread_;
};

#endif /* CONSOLE_H_ */
