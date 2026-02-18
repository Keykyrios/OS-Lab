#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <string>

using namespace std;

mutex print_mtx;

class Philosopher {
    int id;
    mutex& firstFork;
    mutex& secondFork;
    string colorCode;
    string resetCode = "\033[0m";

    void performAction(const string& action) {
        {
            lock_guard<mutex> lock(print_mtx);
            cout << colorCode << "Philosopher " << id << " " << action << resetCode << "\n";
        }
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
    }

public:
    Philosopher(int id, mutex& firstFork, mutex& secondFork, const string& colorCode)
        : id(id), firstFork(firstFork), secondFork(secondFork), colorCode(colorCode) {}

    void operator()() {
        for (int i = 0; i < 3; i++) {
            performAction("is Thinking... 🧠");
            
            firstFork.lock();
            performAction("picked up their first fork. 🍴");
            
            secondFork.lock();
            performAction("picked up their second fork and is EATING! 🍝");
            
            secondFork.unlock();
            performAction("put down their second fork.");
            
            firstFork.unlock();
            performAction("put down their first fork and is full (for now).");
        }
        performAction("has finished their meal and left the table. 👋");
    }
};

int main() {
    const int NUM_PHILOSOPHERS = 5;
    mutex forks[NUM_PHILOSOPHERS];
    
    string colors[] = {
        "\033[31m",
        "\033[32m",
        "\033[33m",
        "\033[34m",
        "\033[35m"
    };

    cout << "🍽️  The dinner is starting...\n\n";

    vector<thread> threads;
    srand(time(nullptr));

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        mutex& leftFork = forks[i];
        mutex& rightFork = forks[(i + 1) % NUM_PHILOSOPHERS];

        if (i % 2 == 0) {
            threads.emplace_back(Philosopher(i, rightFork, leftFork, colors[i]));
        } else {
            threads.emplace_back(Philosopher(i, leftFork, rightFork, colors[i]));
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
