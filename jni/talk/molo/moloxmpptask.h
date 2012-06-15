/*
 * MoloXmppTask.h
 *
 *  Created on: 2012-5-22
 *      Author: gongchen
 */

#ifndef MOLOXMPPTASK_H_
#define MOLOXMPPTASK_H_

#include "talk/base/sigslot.h"
#include "talk/xmpp/xmppengine.h"
#include "talk/xmpp/xmpptask.h"

namespace cricket {

class MoloXmppTask: public buzz::XmppTask
{
public:
	MoloXmppTask(buzz::XmppTaskParentInterface* parent);
	~MoloXmppTask();

	virtual bool HandleStanza(const buzz::XmlElement* stanza);
	virtual int ProcessStart();
	sigslot::signal1<const buzz::XmlElement*> SignalHandleCandidate;
	sigslot::signal1<const buzz::XmlElement*> SignalHandleChat;
};

}
#endif /* MOLOXMPPTASK_H_ */
