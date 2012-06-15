#####################################################################
# the build script for p2p demo libjingle
#      
                           
LOCAL_PATH := $(call my-dir)    
                                  
LOCAL_PATH :=/Users/gongchen/Dev/Eclipse_workspace/P2Pdemo/jni
###########################################################
# the third prebuild library for linking
#

include $(CLEAR_VARS)   
LOCAL_CPP_EXTENSION := .cc 
LOCAL_MODULE := libexpat        

LOCAL_CPPFLAGS := -DXML_STATIC -DHAVE_EXPAT_CONFIG_H

LOCAL_SRC_FILES := \
	talk/third_party/expat-2.1.0/lib/xmlparse.c \
	talk/third_party/expat-2.1.0/lib/xmlrole.c \
	talk/third_party/expat-2.1.0/lib/xmltok.c \
		        
LOCAL_C_INCLUDES := talk/third_party/expat-2.1.0/lib  

include $(BUILD_STATIC_LIBRARY)
                  
###########################################################
# build libjingle itself
#    
include $(CLEAR_VARS)        
LOCAL_MODULE := libjingle      

LOCAL_CPPFLAGS := -DHAMMER_TIME=1 -DLOGGING=1 -DFEATURE_ENABLE_SSL -DFEATURE_ENABLE_VOICEMAIL -DFEATURE_ENABLE_PSTN -DHAVE_SRTP -DHASHNAMESPACE=__gnu_cxx -DHASH_NAMESPACE=__gnu_cxx -DPOSIX -DDISABLE_DYNAMIC_CAST -D_REENTRANT -DOS_LINUX=OS_LINUX -DLINUX -D_DEBUG -DFEATURE_ENABLE_VOICEMAIL -DEXPAT_RELATIVE_PATH -DSRTP_RELATIVE_PATH -DXML_STATIC -DANDROID -D_STLP_NO_ANACHRONISMS=1 -D_STLP_USE_DYNAMIC_LIB=1


LOCAL_C_INCLUDES := \
	/Users/gongchen/Dev/Eclipse_workspace/P2Pdemo/jni/talk/third_party/expat-2.1.0/lib \
	$(JNI_H_INCLUDE) \
	
LOCAL_CPP_EXTENSION := .cc             
      
LOCAL_STATIC_LIBRARIES := \
	cpufeatures \
	libexpat \
  	
#LOCAL_SHARED_LIBRARIES := libstlport  

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

#including source files    
LOCAL_SRC_FILES := \
	talk/base/asyncfile.cc \
	talk/base/asynchttprequest.cc \
	talk/base/asyncsocket.cc \
	talk/base/asynctcpsocket.cc \
	talk/base/asyncudpsocket.cc \
	talk/base/autodetectproxy.cc \
	talk/base/bandwidthsmoother.cc \
	talk/base/base64.cc \
	talk/base/basicpacketsocketfactory.cc \
	talk/base/bytebuffer.cc \
    talk/base/checks.cc \
    talk/base/common.cc \
    talk/base/cpuid.cc \
    talk/base/cpumonitor.cc \
    talk/base/diskcache.cc \
    talk/base/event.cc \
    talk/base/filelock.cc \
    talk/base/fileutils.cc \
    talk/base/firewallsocketserver.cc \
    talk/base/flags.cc \
    talk/base/helpers.cc \
    talk/base/host.cc \
    talk/base/httpbase.cc \
    talk/base/httpclient.cc \
    talk/base/httpcommon.cc \
    talk/base/httprequest.cc \
    talk/base/httpserver.cc \
    talk/base/ipaddress.cc \
	talk/base/linux.cc \
	talk/base/logging.cc \
    talk/base/md5c.c \
    talk/base/messagehandler.cc \
    talk/base/messagequeue.cc \
    talk/base/multipart.cc \
    talk/base/natserver.cc \
    talk/base/natsocketfactory.cc \
    talk/base/nattypes.cc \
    talk/base/nethelpers.cc \
    talk/base/network.cc \
    talk/base/openssladapter.cc \
    talk/base/optionsfile.cc \
    talk/base/pathutils.cc \
    talk/base/physicalsocketserver.cc \
    talk/base/proxydetect.cc \
    talk/base/proxyinfo.cc \
    talk/base/proxyserver.cc \
    talk/base/ratetracker.cc \
   	talk/base/sharedexclusivelock.cc \
   	talk/base/signalthread.cc \
	talk/base/socketadapters.cc \
   	talk/base/socketaddress.cc \
    talk/base/socketaddresspair.cc \
    talk/base/socketpool.cc \
    talk/base/socketstream.cc \
	talk/base/ssladapter.cc \
	talk/base/sslsocketfactory.cc \
	talk/base/stream.cc \
	talk/base/stringdigest.cc \
	talk/base/stringencode.cc \
   	talk/base/stringutils.cc \
	talk/base/systeminfo.cc \
   	talk/base/task.cc \
   	talk/base/taskparent.cc \
   	talk/base/taskrunner.cc \
   	talk/base/testclient.cc \
   	talk/base/thread.cc \
   	talk/base/timeutils.cc \
   	talk/base/timing.cc \
   	talk/base/transformadapter.cc \
	talk/base/unixfilesystem.cc \
   	talk/base/urlencode.cc \
   	talk/base/versionparsing.cc \
   	talk/base/virtualsocketserver.cc \
   	talk/base/worker.cc \
   	talk/p2p/base/constants.cc \
   	talk/p2p/base/p2ptransport.cc \
   	talk/p2p/base/p2ptransportchannel.cc \
   	talk/p2p/base/parsing.cc \
	talk/p2p/base/port.cc \
	talk/p2p/base/portallocator.cc \
	talk/p2p/base/portallocatorsessionproxy.cc \
	talk/p2p/base/portproxy.cc \
	talk/p2p/base/pseudotcp.cc \
	talk/p2p/base/relayport.cc \
	talk/p2p/base/relayserver.cc \
	talk/p2p/base/session.cc \
	talk/p2p/base/sessiondescription.cc \
	talk/p2p/base/sessionmanager.cc \
	talk/p2p/base/sessionmessages.cc \
	talk/p2p/base/stun.cc \
	talk/p2p/base/stunport.cc \
	talk/p2p/base/stunrequest.cc \
	talk/p2p/base/stunserver.cc \
	talk/p2p/base/tcpport.cc \
	talk/p2p/base/transport.cc \
	talk/p2p/base/transportchannel.cc \
	talk/p2p/base/transportchannelproxy.cc \
	talk/p2p/base/udpport.cc \
	talk/p2p/client/basicportallocator.cc \
	talk/p2p/client/connectivitychecker.cc \
	talk/p2p/client/httpportallocator.cc \
	talk/p2p/client/socketmonitor.cc \
	talk/xmllite/qname.cc \
	talk/xmllite/xmlbuilder.cc \
	talk/xmllite/xmlconstants.cc \
	talk/xmllite/xmlelement.cc \
	talk/xmllite/xmlnsstack.cc \
	talk/xmllite/xmlparser.cc \
	talk/xmllite/xmlprinter.cc \
	talk/xmpp/constants.cc \
	talk/xmpp/hangoutpubsubclient.cc \
	talk/xmpp/iqtask.cc \
	talk/xmpp/jid.cc \
	talk/xmpp/moduleimpl.cc \
	talk/xmpp/mucroomconfigtask.cc \
	talk/xmpp/mucroomhistorytask.cc \
	talk/xmpp/mucroomlookuptask.cc \
	talk/xmpp/pubsubclient.cc \
	talk/xmpp/pubsub_task.cc \
	talk/xmpp/pubsubtasks.cc \
	talk/xmpp/receivetask.cc \
	talk/xmpp/saslmechanism.cc \
	talk/xmpp/xmppclient.cc \
	talk/xmpp/xmppengineimpl.cc \
	talk/xmpp/xmppengineimpl_iq.cc \
	talk/xmpp/xmpplogintask.cc \
	talk/xmpp/xmppstanzaparser.cc \
	talk/xmpp/xmpptask.cc \

include $(BUILD_STATIC_LIBRARY)  

###########################################################
# the xmpp help library
#     
include $(CLEAR_VARS)
LOCAL_MODULE := xmpphelp     

LOCAL_CPP_EXTENSION := .cc    
 
LOCAL_SRC_FILES := \
    talk/examples/login/xmppauth.cc \
    talk/examples/login/xmpppump.cc \
    talk/examples/login/xmppsocket.cc \

LOCAL_STATIC_LIBRARIES := libjingle  
    
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog    

include $(BUILD_STATIC_LIBRARY)  
###########################################################   
# test list
# include $(CLEAR_VARS)
# LOCAL_MODULE := list_test
# LOCAL_CPP_EXTENSION := .cc
# LOCAL_SRC_FILES := list2.cc
# include $(BUILD_EXECUTABLE) 

###########################################################
# the native jni interface library     
#
include $(CLEAR_VARS)
LOCAL_MODULE    := jingle_jni        

LOCAL_CPP_EXTENSION := .cc     

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
 
LOCAL_CPPFLAGS := -DHAMMER_TIME=1 -DLOGGING=1 -DFEATURE_ENABLE_SSL -DFEATURE_ENABLE_VOICEMAIL -DFEATURE_ENABLE_PSTN -DHAVE_SRTP -DHASHNAMESPACE=__gnu_cxx -DHASH_NAMESPACE=__gnu_cxx -DPOSIX -DDISABLE_DYNAMIC_CAST -D_REENTRANT -DOS_LINUX=OS_LINUX -DLINUX -D_DEBUG -DFEATURE_ENABLE_VOICEMAIL -DEXPAT_RELATIVE_PATH -DSRTP_RELATIVE_PATH -DXML_STATIC -DANDROID -DJNI_BASE -D_STLP_NO_ANACHRONISMS=1 -D_STLP_USE_DYNAMIC_LIB=1 

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog 

#LOCAL_SHARED_LIBRARIES := \
#	libstlport \
	 
LOCAL_STATIC_LIBRARIES := \
	libjingle \
	libexpat \
	xmpphelp \
	cpufeatures \
      
LOCAL_SRC_FILES := \
	com_molo_app_NativeMethod.cc \
    talk/mymolo/console.cc \
	talk/mymolo/moloclient.cc \
	talk/molo/moloxmpptask.cc \
    talk/examples/login/xmppthread.cc \

#LOCAL_STATIC_LIBRARIES := cpufeatures  

include $(BUILD_SHARED_LIBRARY)     
$(call import-module,android/cpufeatures)