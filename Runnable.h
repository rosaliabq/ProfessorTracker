/***************************************************************************
 *   Copyright (C) 2013 by  Elisardo González Agulla  <eli@gts.uvigo.es>   *
 ***************************************************************************/
#ifndef _RUNNABLE
#define _RUNNABLE
#include <sys/types.h>
//#define  HAVE_MODE_T
#include <semaphore.h>
#include <pthread.h>
#include <assert.h>   
//#include <time.h>
#include <stdexcept>
//#undef  HAVE_MODE_T



#include <iostream>
#include <string>

using namespace std;

class Runnable
{


public:
	Runnable(bool highPriority=true);
	virtual ~Runnable();
	void go();
	void go_detached();
	void stop();
//	void stop_detached();
	bool isRunning(); 
	bool isStarted();

	inline void setName(string value){
		name=value;
	}
	inline string getName() const {
		return name;
	}

//	protected:
pthread_mutex_t m_mutex;
pthread_attr_t attr;
struct sched_param param;
bool m_hightPriority;
//private:
	volatile bool m_running;
	volatile bool m_started;
	pthread_t m_thread;
	int rc;
	

	// This is the static class function that serves as a C style function  pointer
	// for the pthread_create call
   //	static void* start_thread(void *obj);

	virtual void do_work() = 0;

	void checkoutRC(string src, string function, int rc);
	string name;

};
#endif
