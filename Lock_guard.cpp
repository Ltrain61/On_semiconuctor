#include <iostream>
#include <mutex>
#include <random>
#include <chrono>
#include <thread>
#include <string>
using namespace std;

bool done = false;
mutex m;

namespace LockGuard {
	class lock_guard {
	public:
		lock_guard(mutex *pm):mptr(pm) {     // constructor That attomatically locks the function
			mptr->lock();
		}
		
		
		~lock_guard()   // when the object goes out of scope the lock gaurd atomatically unlocks.
		{
			mptr->unlock();
		}
	private:
		mutex *mptr;
		
	};
};

void worker(string id)
{
	while (!done) {
		{
			LockGuard::lock_guard l(&m); // creates the lock gaurd and passes the mutex ptr to it
			cout << "starting work - id: " << id << endl;
			auto start = chrono::high_resolution_clock::now();

			std::random_device r;
			mt19937 mt(r());     //selects a random number and seeds with random number using mt19937
			
			std::default_random_engine e1(r());
			std::uniform_int_distribution<int> uniform_dist(1, 6); //makes the random number for 1-6
			int mean = uniform_dist(e1);                           //assignes the int mean a random nuber from 1-6
			this_thread::sleep_for(chrono::milliseconds(mean));    // makes the 
			auto end = chrono::high_resolution_clock::now();
			chrono::duration<double, milli> elapsed = end - start;
			cout << "done - id: " << id << ", worked: " << elapsed.count() << " ms\n";;
		}
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
	

	



	if (t1.joinable()) {
		t1.join();
	}

	if (t2.joinable()) {
		t2.join();
	}
	if (t3.joinable()) {
		t3.join();
	}


	return 0;
}