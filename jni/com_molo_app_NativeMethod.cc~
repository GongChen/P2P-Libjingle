#include "com_molo_app_NativeMethod.h"  
  
#include <jni.h>  
#include <cstdio>
#include <cstring>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <vector>       
#include <list>
#include <stdlib.h>    

#include <pthread.h> 

#include <android/log.h>    
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "P2P", __VA_ARGS__)
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
                                        
//JNI           
std::string jstringTostring(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("GB2312");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1); //new   char[alen+1];
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	std::string stemp(rtn);
	free(rtn);
	return stemp;
}

std::string jstring2str(JNIEnv* env, jstring jstr)
{   
	char*   rtn   =   NULL;   
	jclass   clsstring   =   env->FindClass("java/lang/String");   
	jstring   strencode   =   env->NewStringUTF("GB2312");   
	jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");   
	jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);   
	jsize   alen   =   env->GetArrayLength(barr);   
	jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);   
	if(alen   >   0)   
	{   
		rtn   =   (char*)malloc(alen+1);         
		memcpy(rtn,ba,alen);   
		rtn[alen]=0;   
	}   
	env->ReleaseByteArrayElements(barr,ba,0);   
	std::string stemp(rtn);
	free(rtn);
	return   stemp;   
}

MoloClient *client;  

JNIEXPORT jboolean JNICALL Java_com_molo_app_NativeMethod_doLogin
  (JNIEnv* env, jobject clazz, jstring name, jstring password, jobject callback){
	 
	LOGD("Java_com_molo_app_NativeMethod_doLogin");
	bool debug = true;//FLAG_d;

	// parse username and password, if present
	buzz::Jid jid;
	std::string c_username;        
	std::string c_password;
	talk_base::InsecureCryptStringImpl pass;          
	//get string from java         
	
	const char *str = "gc2";//env->GetStringUTFChars(name, NULL);
	      
	__android_log_print(ANDROID_LOG_WARN, "P2P", "c_username is  = %s", str);
	const char *pass_str = "123456";//env->GetStringUTFChars(password, NULL);
   	__android_log_print(ANDROID_LOG_WARN, "P2P", "c_password is  = %s", pass_str); 

	jid = buzz::Jid(str);            
	pass.password() = pass_str;     
        LOGD("username and password are ready"); 
	buzz::XmppClientSettings xcs;
	xcs.set_user(jid.domain());
	xcs.set_host("molohui.com");
	xcs.set_use_tls(buzz::TLS_DISABLED);
	xcs.set_pass(talk_base::CryptString(pass));
	xcs.set_server(talk_base::SocketAddress("www.molohui.com", 5222));
	xcs.set_allow_plain(false);     
    LOGD("main_thread init"); 
	talk_base::Thread* main_thread = talk_base::Thread::Current();    
	 
	LOGD("test done");  
	// pthread_t id = pthread_self();                             
	// 	__android_log_print(ANDROID_LOG_WARN, "P2P", "thread id is  = %lu", id);    
	XmppPump pump;    
	LOGD("MoloClient init");          	
	client = new MoloClient(pump.client()); 
	__android_log_print(ANDROID_LOG_WARN, "P2P", "env is  = %u", env); 	
	client->setEnv(env);  
	 
	JavaVM* gs_jvm=NULL;  		   	
	env->GetJavaVM(&gs_jvm);   	
	__android_log_print(ANDROID_LOG_WARN, "P2P", "JVM is  = %u", gs_jvm);   
	client->setJVM(gs_jvm);   
	
	jobject gs_object=env->NewGlobalRef(callback);  
	client->setCallback(gs_object);
	
	
    LOGD("Console init"); 
    Console *console = new Console(main_thread, client);
	client->SetConsole(console);
	console->Start();

	if (debug) {
		pump.client()->SignalLogInput.connect(&debug_log_, &DebugLog::Input);
	    pump.client()->SignalLogOutput.connect(&debug_log_, &DebugLog::Output);
    }
    LOGD("Java_com_molo_app_NativeMethod_doLogin::pump.DoLogin");

   	pump.DoLogin(xcs, new XmppSocket(buzz::TLS_DISABLED), new XmppAuth());
  	main_thread->Run();
   	pump.DoDisconnect();

   	console->Stop();    

	env->ReleaseStringUTFChars(name, str);   
	env->ReleaseStringUTFChars(password, pass_str); 
	
   	delete console;
   	delete client;    

	return true;
}         


JNIEXPORT jboolean JNICALL Java_com_molo_app_NativeMethod_createChannel
  (JNIEnv* env, jobject clazz, jstring name, jstring remoteName){     
	
	LOGD("createChannel!");
	// std::string c_name;        
	// std::string c_remote_name;
       
	//get string from java
	// c_name =  jstringTostring(env, name);     
	// c_remote_name =  jstringTostring(env, remoteName); 
	
	__android_log_print(ANDROID_LOG_WARN, "P2P", "createChannel::client is  = %u", client);
    if(client)
	{
	  	client->CreateP2P("");	 
	} 
	
	return false;
}

JNIEXPORT jboolean JNICALL Java_com_molo_app_NativeMethod_sendData
  (JNIEnv* env, jobject clazz, jstring remoteName, jstring data){  
	LOGD("sendData!");         
    // std::string c_remote_name;
    //     std::string c_data;      
    // 
    // 	//get string from java
    // 	c_data =  jstringTostring(env, data);     
    // 	c_remote_name =  jstringTostring(env, remoteName); 
                  
	const char *str = env->GetStringUTFChars(data, NULL);
	
	if(client)
	{
	  	client->SendData(str);	 
	}
	env->ReleaseStringUTFChars(data, str); 
	return true;
} 

JNIEXPORT jboolean JNICALL Java_com_molo_app_NativeMethod_quit
  (JNIEnv* env, jobject clazz){
	if(client)
	{           
		LOGD("client->Quit()");
		client->Quit();    
		talk_base::Thread::Current()->Quit();
	}
	return true;
}  
    
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *ajvm, void *reserved){     
    LOGD("JNI_OnLoad startup~~!");     
    return JNI_VERSION_1_4;     
}  

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved){     
    LOGD("call JNI_OnUnload ~~!!");    
} 
  
