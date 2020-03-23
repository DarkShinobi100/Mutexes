// Race conditions example
// Adam Sampson <a.sampson@abertay.ac.uk>

//Timings (insecure)
//20ms
//21ms
//20ms
//18ms
//43ms

//Timings (secure)
//137ms
//142ms
//136ms
//148ms
//141ms

#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include "account.h"
#include <mutex>

// Import things we need from the standard library
using std::cout;
using std::endl;
using std::thread;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::this_thread::sleep_for;

//define the alias for the clock type we're going to use
typedef std::chrono::steady_clock the_clock;

 Account bill;
 mutex bill_mutex;
 //Variables
 bool result_ready = false;
 condition_variable result_cv;
void add()
{
	for (int i = 0; i < 1000000; i++)
	{
		//bill_mutex.lock();
		unique_lock <mutex> lock(bill_mutex);
		bill.add(17, 29);
		//bill_mutex.unlock();

		result_ready = true;
		result_cv.notify_one();
	}
}

void print_total()
{
	unique_lock <mutex> lock(bill_mutex);

	while(!result_ready)
	{
		result_cv.wait(lock);
	}
	cout << "account total is: " << bill.total() << endl;
}


int main(int argc, char *argv[])
{
	float time_taken;
	thread myThread[5];
	thread printThread;

	cout << "Initial: " << bill.total() << "\n";
	
	//time how long it takes
	//the_clock::time_point start = the_clock::now();
	
	for (int i = 0; i < 5; i++)
	{
		myThread[i] = thread(add);
	}
	printThread = thread(print_total);
	//bill.add(4, 17);
	//bill.add(3, 0);
	//bill.add(10, 99);
	// Wait for myThread to finish.

	for (int i = 0; i < 5; i++)
	{
		myThread[i].join();
	}
	printThread.join();
	//the_clock::time_point end = the_clock::now();
	//time_taken = duration_cast<milliseconds>(end - start).count();

	//print the time taken
	//cout << "time taken to Search " << time_taken << "ms" << endl;

	cout << "Total: " << bill.total() << "\n";

	return 0;
}