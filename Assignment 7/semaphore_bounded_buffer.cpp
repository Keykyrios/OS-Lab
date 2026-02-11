#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <random>

const int BUFFER_SIZE = 5;
const int NUM_ITEMS_TO_PRODUCE = 10;
const int NUM_PRODUCERS = 2;
const int NUM_CONSUMERS = 2;

class BoundedBuffer {
private:
    std::vector<int> buffer;
    int in;
    int out;
    sem_t mutex;
    sem_t empty;
    sem_t full;

public:
    BoundedBuffer(int size) : buffer(size), in(0), out(0) {
        sem_init(&mutex, 0, 1);
        sem_init(&empty, 0, size);
        sem_init(&full, 0, 0);
    }

    ~BoundedBuffer() {
        sem_destroy(&mutex);
        sem_destroy(&empty);
        sem_destroy(&full);
    }

    void produce(int item, int producerId) {
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        std::cout << "\033[1;32m[Producer " << producerId << "]\033[0m "
                  << "Produced Item: " << item
                  << " at Index: " << in << std::endl;
        in = (in + 1) % buffer.size();

        sem_post(&mutex);
        sem_post(&full);
    }

    int consume(int consumerId) {
        int item;
        sem_wait(&full);
        sem_wait(&mutex);

        item = buffer[out];
        std::cout << "\033[1;36m[Consumer " << consumerId << "]\033[0m "
                  << "Consumed Item: " << item
                  << " from Index: " << out << std::endl;
        out = (out + 1) % buffer.size();

        sem_post(&mutex);
        sem_post(&empty);
        return item;
    }
};

BoundedBuffer sharedBuffer(BUFFER_SIZE);

void* producer_routine(void* arg) {
    int id = *((int*)arg);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 99);

    for (int i = 0; i < NUM_ITEMS_TO_PRODUCE; ++i) {
        int item = dis(gen);
        usleep(rand() % 100000);
        sharedBuffer.produce(item, id);
    }
    return nullptr;
}

void* consumer_routine(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < NUM_ITEMS_TO_PRODUCE; ++i) {
        usleep(rand() % 150000);
        sharedBuffer.consume(id);
    }
    return nullptr;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    std::vector<int> p_ids(NUM_PRODUCERS);
    std::vector<int> c_ids(NUM_CONSUMERS);

    std::cout << "--- Bounded Buffer Simulation Started ---\n";
    std::cout << "Buffer Size: " << BUFFER_SIZE << "\n\n";

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        p_ids[i] = i + 1;
        pthread_create(&producers[i], nullptr, producer_routine, &p_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        c_ids[i] = i + 1;
        pthread_create(&consumers[i], nullptr, consumer_routine, &c_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; ++i) pthread_join(producers[i], nullptr);
    for (int i = 0; i < NUM_CONSUMERS; ++i) pthread_join(consumers[i], nullptr);

    std::cout << "\n--- Simulation Finished ---" << std::endl;
    return 0;
}
