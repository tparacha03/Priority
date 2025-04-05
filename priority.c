#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

// Simulate different priority levels
#define LOW_PRIORITY 0
#define MEDIUM_PRIORITY 1
#define HIGH_PRIORITY 2

// Function to simulate CPU-intensive work
void doWork(int iterations) {
    for (int i = 0; i < iterations; i++) {
        // Busy wait to simulate CPU work
        for (volatile long j = 0; j < 10000000; j++);
    }
}

// Structure to pass data to threads
typedef struct {
    int id;
    int priority;
    int workAmount;
} ThreadData;

// Thread function
void* threadFunction(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    // Set thread priority based on data
    int policy;
    struct sched_param param;
    
    pthread_getschedparam(pthread_self(), &policy, &param);
    
    // Map our priorities to POSIX priorities
    // Note: This is a simulation, as real Windows priorities would use Windows API
    switch(data->priority) {
        case HIGH_PRIORITY:
            param.sched_priority = sched_get_priority_max(policy);
            break;
        case MEDIUM_PRIORITY:
            param.sched_priority = (sched_get_priority_max(policy) + 
                                   sched_get_priority_min(policy)) / 2;
            break;
        case LOW_PRIORITY:
            param.sched_priority = sched_get_priority_min(policy);
            break;
    }
    
    pthread_setschedparam(pthread_self(), policy, &param);
    
    // Record start time
    printf("Thread %d (Priority %d) starting work\n", data->id, data->priority);
    time_t startTime = time(NULL);
    
    // Do the work
    doWork(data->workAmount);
    
    // Record end time and report
    time_t endTime = time(NULL);
    printf("Thread %d (Priority %d) finished after %ld seconds\n", 
           data->id, data->priority, (endTime - startTime));
    
    return NULL;
}

int main() {
    pthread_t threads[3];
    ThreadData threadData[3];
    
    printf("Starting priority-based scheduling demonstration\n");
    
    // Create threads with different priorities and workloads
    threadData[0].id = 1;
    threadData[0].priority = LOW_PRIORITY;
    threadData[0].workAmount = 5;
    
    threadData[1].id = 2;
    threadData[1].priority = MEDIUM_PRIORITY;
    threadData[1].workAmount = 5;
    
    threadData[2].id = 3;
    threadData[2].priority = HIGH_PRIORITY;
    threadData[2].workAmount = 5;
    
    // Start threads in reverse priority order to demonstrate preemption
    pthread_create(&threads[0], NULL, threadFunction, &threadData[0]);
    sleep(1); // Small delay between thread creations
    
    pthread_create(&threads[1], NULL, threadFunction, &threadData[1]);
    sleep(1);
    
    pthread_create(&threads[2], NULL, threadFunction, &threadData[2]);
    
    // Wait for all threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads completed\n");
    return 0;
}
