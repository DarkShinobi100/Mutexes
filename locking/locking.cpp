// Race conditions example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <iostream>
#include <string>
#include <thread>
#include "account.h"

// Import things we need from the standard library
using std::cout;
using std::endl;
using std::thread;

Account bill;
void add()
{
	for (int i = 0; i < 1000000; i++)
	{
		bill.add(17, 29);
	}
}


int main(int argc, char *argv[])
{
	thread myThread[5];

	cout << "Initial: " << bill.total() << "\n";
	
	for (int i = 0; i < 5; i++)
	{
		myThread[i] = thread(add);
	}
	//bill.add(4, 17);
	//bill.add(3, 0);
	//bill.add(10, 99);
	// Wait for myThread to finish.

	for (int i = 0; i < 5; i++)
	{
		myThread[i].join();
	}

	cout << "Total: " << bill.total() << "\n";

	return 0;
}