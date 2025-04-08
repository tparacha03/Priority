#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

// Simulate different priority levels
#define LOW_PRIORITY 0
#define MEDIUM_PRIORITY 1
#define HIGH_PRIORITY 2

// Structure to pass data to threads
typedef struct {
    int id;
    int priority;
    int workAmount;
} ThreadData;

// Function to get current time in milliseconds
long long currentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

// Simulated CPU-intensive work
void doWork(int iterations) {
    for (int i = 0; i < iterations; i++) {
        for (volatile long j = 0; j < 10000000; j++);
    }
}

// Thread function
void* threadFunction(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    long long startTime = currentTimeMillis();
    printf("Thread %d (Priority %d) starting work\n", data->id, data->priority);

    doWork(data->workAmount);

    long long endTime = currentTimeMillis();
    printf("Thread %d (Priority %d) finished after %lld ms\n",
           data->id, data->priority, endTime - startTime);

    return NULL;
}

int main() {
    pthread_t threads[3];
    ThreadData threadData[3];

    printf("Simulated priority scheduling demo (no root required)\n");

    // Define thread parameters
    threadData[0] = (ThreadData){ .id = 1, .priority = LOW_PRIORITY, .workAmount = 5 };
    threadData[1] = (ThreadData){ .id = 2, .priority = MEDIUM_PRIORITY, .workAmount = 5 };
    threadData[2] = (ThreadData){ .id = 3, .priority = HIGH_PRIORITY, .workAmount = 5 };

    // Simulate scheduling: High priority starts first
    pthread_create(&threads[2], NULL, threadFunction, &threadData[2]);
    usleep(500000); // 0.5 second delay

    pthread_create(&threads[1], NULL, threadFunction, &threadData[1]);
    usleep(500000);

    pthread_create(&threads[0], NULL, threadFunction, &threadData[0]);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed\n");
    return 0;
}
