// @author : ian cain
// @date   : 2018
// @b      : Coding challenge for interview candidates to show ability to use and synchronize threads
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

bool done = false;
condition_variable cv;// condition variable to sync the threds together.
mutex mu;             // mutex to lock common resorses

void worker(string id)
{
	while (!done) {													//The loop that has no exit to allow the work to go on forever
		unique_lock<mutex> locker(mu);								//A mutex that lockes the worker function
		
		cv.wait(locker);											// A condition variable that pusts the string calling worker on pause
		cout << "starting work - id: " << id << endl; 

		auto start = high_resolution_clock::now();//starting time for worker
		
		this_thread::sleep_for(chrono::milliseconds(5));			//makes the fuction sleep for 5 ms to simulate work being done.
		
		auto end = high_resolution_clock::now();// ending the time for the worker
		chrono::duration<double, milli> elapsed = end - start; // gets the total time that work was done
		cout << "done - id: " << id << ", worked: " << elapsed.count() << " ms\n" << endl;
		locker.unlock();										//unlock the mutex that was locking the work function
		
		cv.notify_one();										//Cv notifies the next waiting thread
	}
}

int main()
{
	string worker1 = "1234";
	string worker2 = "1345";
	string worker3 = "1456";
	
	cout << "Starting all threads" << endl;
	thread t1(worker, move(worker1));
	thread t2(worker, move(worker2));
	thread t3(worker, move(worker3));
	
	
	cv.notify_one(); // control variable to notify next to start working
	
	if(t1.joinable()){               // test to see if therad is joinable then joins the thread
		t1.join();          // joins the threads
	}
	
	if (t2.joinable()) {
		t2.join();
	}
	if (t3.joinable()) {
		t3.join();
	}
	
	return 0;
}