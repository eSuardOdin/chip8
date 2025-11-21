#include <stdio.h>
#include <pthread.h>

int num = 0;

void *inc_num(void* num);
void *display_num(void* num);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int main() {

    pthread_t t60;
    pthread_t t500;
    pthread_create(&t60, NULL, display_num, (void*) &num);
    pthread_create(&t500, NULL, inc_num, (void*) &num);


    pthread_join(t60, NULL);
    pthread_join(t500, NULL);
    return 0;
}

void *inc_num(void *num) {
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 2000000};
    int* n = (int *)num;
    while(1) {
        pthread_mutex_lock(&lock);
        (*n)++;
        pthread_mutex_unlock(&lock);

        clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
    }
}

void *display_num(void *num) {
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 16666666};
    int* n = (int *)num;
    while(1) {
        pthread_mutex_lock(&lock);
        printf("Num is %d.\n", *n);
        pthread_mutex_unlock(&lock);

        clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
    }
}