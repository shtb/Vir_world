#include"console.h"
#include<iostream>
#include<sstream>
#include<string>
#include<iomanip>

using namespace std;

console::console()
{
}

void console::warning(string sentence)
{
	lock.slock();
	cerr<< "\033[35m >WARNING: " + sentence+"\033[0m"<<endl;
	lock.unlock();
}

void console::error(string sentence)
{
	lock.slock();
	cerr<<"\033[31m >ERROR: "+sentence+"\033[0m"<<endl;
	lock.unlock();
}

void console::message(string sentence)
{
	lock.slock();
	cout<<"\033[34m >"+sentence+"\033[0m"<<endl;
	lock.unlock();
}

stringstream&console::con_input()
{
	ss_input.clear();
	ss_input.str("");
	string s;
	cin>>s;
	ss_input<<s;
	return ss_input;
}

void thread_progress_bar(string note,long*did,long sum,spinlock* lock)
{
	while(*did<sum)
	{
		float dper=(*did)/(double)sum*100.0;
		lock->slock();
		cout<<"\033[34m      >--"+note<<setprecision(3)<<dper<<"% "<<"("<<*did<<"/"<<sum<<")"<<"--<\033[0m"<<endl;
		lock->unlock();
		usleep(1000000);
	}
	delete did;
}

void console::progress_bar(string note,long*did,long sum)
{
	thread tem_thread(thread_progress_bar,note,did,sum,&lock);
	tem_thread.detach();
}

console::~console()
{
}
