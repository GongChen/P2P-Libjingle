/*
 * moloxmpptask.cc
 *
 *  Created on: 2012-5-22
 *      Author: gongchen
 */
#include <string>


#include "talk/molo/moloxmpptask.h"


#include "talk/p2p/base/constants.h"
#include "talk/xmpp/constants.h"
#include "talk/base/helpers.h"
#include "talk/base/logging.h"

using namespace cricket;

namespace cricket {
MoloXmppTask::MoloXmppTask(buzz::XmppTaskParentInterface* parent) :
		XmppTask(parent, buzz::XmppEngine::HL_ALL) {
}

MoloXmppTask::~MoloXmppTask() {

}

bool MoloXmppTask::HandleStanza(const buzz::XmlElement* stanza) {
	LOG(LS_INFO)
			<< "MoloXmppTask::HandleStanza(const buzz::XmlElement* stanza)";
		const buzz::XmlElement* cand_elem = stanza->FirstNamed(
				QN_GINGLE_P2P_CANDIDATE);

		if (stanza->Name() == buzz::QN_MESSAGE && !cand_elem) {
			LOG(LS_INFO) << "got a message. \n";
			SignalHandleChat(stanza);
			return true;
		}

		if (!cand_elem || !cand_elem->HasAttr(QN_NAME)
				|| !cand_elem->HasAttr(QN_ADDRESS) || !cand_elem->HasAttr(QN_PORT)
				|| !cand_elem->HasAttr(QN_USERNAME)
				|| !cand_elem->HasAttr(QN_PREFERENCE)
				|| !cand_elem->HasAttr(QN_PROTOCOL)
				|| !cand_elem->HasAttr(QN_GENERATION)) {
			return false;
		}
		LOG(LS_INFO) << "got a Candidate request. \n";
		SignalHandleCandidate(cand_elem);
		return true;
}

int MoloXmppTask::ProcessStart() {
	return 0;
}
}


