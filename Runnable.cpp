#include "Runnable.h"


extern "C" void *ThreadStartup(void *_tgtObject);
// This function is a helper function. It has normal C linkage, and is
// as the base for newly created ThreadClass objects. It runs the
// run method on the ThreadClass object passed to it (as a void *).
// After the ThreadClass method completes normally (i.e returns).
void *ThreadStartup(void *_tgtObject) {
	Runnable *tgtObject = (Runnable *)_tgtObject;
	try{
		tgtObject->do_work();
	}catch(exception & e){
		cout<<"RUNNABLE exception "<<tgtObject->getName()<<":"<<e.what()<<endl;
	}
	pthread_mutex_lock(&tgtObject->m_mutex);
	tgtObject->m_running = false;
	pthread_mutex_unlock(&tgtObject->m_mutex);
	return NULL;
}



Runnable::Runnable(bool hightPriority) : m_running(false),m_started(false)/*,m_mutex(PTHREAD_MUTEX_INITIALIZER)*/
{
	name="Runnable";
	rc=pthread_mutex_init(&m_mutex, NULL);
	checkoutRC("Runnable()","pthread_mutex_init()",rc);
	rc=pthread_attr_init(&attr);
	checkoutRC("Runnable()","pthread_attr_init()",rc);
	m_hightPriority=hightPriority;
	rc=pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	checkoutRC("Runnable()","pthread_attr_setdetachstate()",rc);
	rc=pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	checkoutRC("Runnable()","pthread_attr_setinheritsched()",rc); 

	if (m_hightPriority) {
		//rc=pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		rc=pthread_attr_setschedpolicy(&attr, SCHED_RR);
		//rc=pthread_attr_setschedpolicy(&attr, SCHED_NORMAL);
		//rc=pthread_attr_setschedpolicy(&attr,SCHED_OTHER);
		//checkoutRC("Runnable()","pthread_attr_setschedpolicy()",rc);


		memset(&param, 0, sizeof(param));
		param.sched_priority = 99;

		rc = pthread_attr_setschedparam(&attr, &param);
		//checkoutRC("Runnable()","pthread_attr_setschedparam()",rc);
	} else {
		rc=pthread_attr_setschedpolicy(&attr, SCHED_RR);
	}
	rc = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	checkoutRC("Runnable()","pthread_attr_setscope()",rc);

}



Runnable::~Runnable()
{

	rc=pthread_attr_destroy(&attr);
	checkoutRC("~Runnable()","pthread_attr_destroy()",rc);

	rc=pthread_mutex_destroy(&m_mutex);
	checkoutRC("~Runnable()","pthread_mutex_destroy()",rc);
	//m_mutex=0;
	m_running = false;
	rc=0;
}

// Create the thread and start work
// Note 1
void Runnable::go()
{
	assert(m_running == false);
	rc=pthread_mutex_lock(&m_mutex);
	checkoutRC("go()","pthread_mutex_lock()",rc);

	m_running = true;
	m_started = true;
	rc=pthread_mutex_unlock(&m_mutex);
	checkoutRC("go()","pthread_mutex_unlock()",rc);



	/*
	rc=pthread_attr_init(&attr);
	checkoutRC("go()","pthread_attr_init()",rc);
	rc=pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	checkoutRC("go()","pthread_attr_setdetachstate()",rc);

	rc=pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	checkoutRC("go()","pthread_attr_setinheritsched()",rc); 

	if (m_hightPriority) {
	//rc=pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	rc=pthread_attr_setschedpolicy(&attr, SCHED_RR);
	//rc=pthread_attr_setschedpolicy(&attr, SCHED_NORMAL);
	//rc=pthread_attr_setschedpolicy(&attr,SCHED_OTHER);
	//checkoutRC("go()","pthread_attr_setschedpolicy()",rc);


	memset(&param, 0, sizeof(param));
	param.sched_priority = 99;

	rc = pthread_attr_setschedparam(&attr, &param);
	//checkoutRC("go()","pthread_attr_setschedparam()",rc);
	} else {
	rc=pthread_attr_setschedpolicy(&attr, SCHED_RR);

	}



	rc = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	//checkoutRC("go()","pthread_attr_setscope()",rc);
	*/





	//	rc=pthread_create(&m_thread, NULL , &Runnable::start_thread, this);
	rc=pthread_create(&m_thread, NULL , ThreadStartup, this);
	//	rc=pthread_create(&m_thread,  &attr, ThreadStartup, this);
	checkoutRC("go()","pthread_create()",rc);
	//rc=pthread_setname_np(m_thread, name.substr(0,min(14,int(name.length()))).c_str());
	//checkoutRC("go()","pthread_setname_np()",rc);


}


void Runnable::go_detached()
{
	assert(m_running == false);

	rc=pthread_mutex_lock(&m_mutex);
	checkoutRC("go_detached()","pthread_mutex_lock()",rc);
	m_running = true;
	rc=pthread_mutex_unlock(&m_mutex);
	checkoutRC("go_detached()","pthread_mutex_unlock()",rc);


	pthread_attr_t attr;  //*********************
	rc=pthread_attr_init(&attr);
	checkoutRC("go_detached()","pthread_attr_init()",rc);
	rc=pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//checkoutRC("go_detached()","pthread_attr_setdetachstate()",rc);

	/* BOUND behavior */
	/*
	rc = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	checkoutRC("go_detached()","pthread_attr_setscope()",rc);
	*/
	//rc=pthread_create(&m_thread, &attr,  ThreadStartup, this);
	rc=pthread_create(&m_thread, &attr, ThreadStartup, this);
	checkoutRC("go_detached()","pthread_create()",rc);


	// prevent a memory leak
	rc=pthread_detach(m_thread);
	//checkoutRC("go_detached()","pthread_detach()",rc);



}
/*
void Runnable::stop_detached(){
//cout<<"***************************"<<name<<"  stop_detached()" <<endl;

rc=pthread_detach(m_thread);
checkoutRC("stop_detached()","pthread_detach()",rc);

rc=pthread_mutex_lock(&m_mutex);
checkoutRC("stop_detached()","pthread_mutex_lock()",rc);
m_running = false;
rc=pthread_mutex_unlock(&m_mutex);
checkoutRC("stop_detached()","pthread_mutex_unlock()",rc);

} 
*/
void Runnable::stop() // Note 2
{

	//	void *status;
	//	int rc=pthread_join(m_thread, &status);
	//cout<<"Completed join with thread, status="<<(long)status<<endl;

	int rc=pthread_join(m_thread, NULL);
	checkoutRC("stop()","pthread_join()",rc);

	//  prevent a memory leak
	//	rc=pthread_detach(m_thread);
	//	checkoutRC("stop()","pthread_detach()",rc);
}

bool Runnable::isRunning(){
	bool value;
	pthread_mutex_lock(&m_mutex);
	value=m_running;
	pthread_mutex_unlock(&m_mutex);
	return value;
}

bool Runnable::isStarted(){
	bool value;
	pthread_mutex_lock(&m_mutex);
	value=m_started;
	pthread_mutex_unlock(&m_mutex);
	return value;
}
/*
void* Runnable::start_thread(void *obj)
{
//All we do here is call the do_work() function
reinterpret_cast<Runnable *>(obj)->do_work();

assert(reinterpret_cast<Runnable *>(obj)->m_running == true);
pthread_mutex_lock(& reinterpret_cast<Runnable *>(obj)->m_mutex);
reinterpret_cast<Runnable *>(obj)->m_running = false;
pthread_mutex_unlock(&reinterpret_cast<Runnable *>(obj)->m_mutex);

return NULL;
}

*/


void Runnable::checkoutRC(string src, string function, int rc){
	/*
	if (rc==ESRCH) {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is ESRCH "<< rc<<endl;
	} else if (rc==EINVAL) {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is EINVAL "<< rc<<endl;
	} else if (rc==ESRCH) {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is ESRCH "<< rc<<endl;
	} else if (rc==EAGAIN) {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is EAGAIN "<< rc<<endl;
	} else if (rc==ENOMEM) {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is ENOMEM "<< rc<<endl;
	} else if (rc==EBUSY) {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is EBUSY "<< rc<<endl;
	} else if (rc==0) {
	} else {
	cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is "<< rc<<endl;
	}*/
	if (rc==0) {
	} else {
		cout<<"ERROR ("<<name<<" -> "<<src<<"); return code from "<<function<<" is "<< rc<<endl;
	}


}
