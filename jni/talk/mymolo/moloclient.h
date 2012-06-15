/*
 * moloclient.h
 *
 *  Created on: 2012-5-20
 *      Author: gongchen
 */

#ifndef MOLOCLIENT_H_
#define MOLOCLIENT_H_

#include <map>
#include <string>
#include <vector>
#include <jni.h> 

#include "talk/p2p/base/session.h"
#include "talk/p2p/base/sessionmanager.h"
#include "talk/p2p/base/candidate.h"
#include "talk/p2p/base/parsing.h"  
#include "talk/p2p/base/p2ptransportchannel.h"
#include "talk/p2p/client/sessionmanagertask.h"
#include "talk/xmpp/xmppclient.h"
#include "talk/examples/call/status.h"
#include "talk/mymolo/console.h"
#include "talk/molo/moloxmpptask.h"
//#include "talk/session/mymolo/mymolosessionclient.h"

class MoloClient: public talk_base::MessageHandler, public sigslot::has_slots<> {
public:
	MoloClient(buzz::XmppClient* xmpp_client);
	~MoloClient();

	void SetConsole(Console *console) {
		console_ = console;
	}
    
	void CreateP2P(const std::string &str);
	void SendData(const char* data);
	void ParseLine(const std::string &str);
	void Quit();
	void OnRequestSignaling();
	void OnSessionCreate(cricket::Session* session, bool initiate);
    void MakeCallTo(const std::string& name);
    void SendChat(const std::string& to, const std::string msg);
	//void OnCallCreate(cricket::P2Pcall* p2pcall);
	//void OnCallDestroy(cricket::P2Pcall* p2pcall);
	//void OnSessionState(cricket::P2Pcall* p2pcall, cricket::Session* session,
	//		cricket::Session::State state);
	void Input(const char * data, int len);

	void HandleCandidate(const buzz::XmlElement* elem);
	void HandleChat(const buzz::XmlElement* elem);
	virtual void OnMessage(talk_base::Message *msg);

	buzz::XmppClient* getXmppClient() const {
		return xmpp_client_;
	}

	void setXmppClient(buzz::XmppClient* xmppClient) {
		xmpp_client_ = xmppClient;
	}
	
		JNIEnv* getEnv() const {
			return this->env;
		}

		void setEnv(JNIEnv* env) {
			this->env = env;
		} 
		
		JavaVM* getJVM() const {
			return this->jvm;
		}

		void setJVM(JavaVM* jvm) {
			this->jvm = jvm;
		}
           
		jobject getCallback() const{
			return this->callback;
		}    
		
		void setCallback(jobject callback){
			this->callback = callback;
		}

private:
	//void Accept();
    void InitMedia();
	void OnStateChange(buzz::XmppEngine::State state);
	void SetAvailable(const buzz::Jid& jid, buzz::Status* status);
	void OnReadPacket(cricket::TransportChannel* channel, const char* data,
			size_t len);
	void OnCandidate(cricket::TransportChannelImpl* ch,
				const cricket::Candidate& c);
	bool WriteCandidate(const cricket::Candidate& candidate, buzz::XmlElement* elem,
			cricket::WriteError* error);
	bool ParseCandidate(const buzz::XmlElement* elem, cricket::Candidate* candidate,
			cricket::ParseError* error);
	bool ParseAddress(const buzz::XmlElement* elem,
			const buzz::QName& address_name, const buzz::QName& port_name,
			talk_base::SocketAddress* address, cricket::ParseError* error);
	static const std::string strerror(buzz::XmppEngine::Error err);
    
	JNIEnv* env;      
	JavaVM* jvm; 
	jobject callback;
	Console *console_;
	buzz::XmppClient* xmpp_client_;
	talk_base::Thread* worker_thread_;
	talk_base::NetworkManager* network_manager_;
	cricket::PortAllocator* port_allocator_;
	cricket::SessionManager* session_manager_;
	cricket::SessionManagerTask* session_manager_task_;
	cricket::MoloXmppTask* molo_xmpp_task_;
	bool allow_local_ips_;
	bool channel_is_not_work_;
	std::string to;

	//cricket::P2Pcall* call_;
	bool incoming_call_;
	//cricket::MyMoloSessionClient * molo_client_;
	cricket::Session *session_;
	buzz::Status my_status_;
	buzz::XmlElement* stanza;
	std::vector<cricket::Candidate> ready_candidates_;
	std::vector<cricket::Candidate> remote_candidates_;
	cricket::P2PTransportChannel* channel;
};


#endif /* MOLOCLIENT_H_ */
