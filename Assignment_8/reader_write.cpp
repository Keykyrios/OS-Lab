#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

mutex resource_mutex;
mutex read_count_mutex;
int read_count = 0;
int shared_data = 0;

void reader(int id) {
    for(int i = 0; i < 3; i++) {
        read_count_mutex.lock();
        read_count++;
        if (read_count == 1) {
            resource_mutex.lock();
        }
        read_count_mutex.unlock();

        cout << "Reader " << id << " read data: " << shared_data << "\n";
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));

        read_count_mutex.lock();
        read_count--;
        if (read_count == 0) {
            resource_mutex.unlock();
        }
        read_count_mutex.unlock();
        
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));
    }
}

void writer(int id) {
    for(int i = 0; i < 2; i++) {
        resource_mutex.lock();
        
        shared_data++;
        cout << "Writer " << id << " modified data to: " << shared_data << "\n";
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));
        
        resource_mutex.unlock();
        
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));
    }
}

int main() {
    vector<thread> r_threads, w_threads;

    for(int i = 1; i <= 2; i++) {
        w_threads.emplace_back(writer, i);
    }
    for(int i = 1; i <= 3; i++) {
        r_threads.emplace_back(reader, i);
    }

    for(auto& t : w_threads) t.join();
    for(auto& t : r_threads) t.join();

    return 0;
}
