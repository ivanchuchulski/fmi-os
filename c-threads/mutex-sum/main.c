#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <errno.h>

struct sum
{
    int result;
    pthread_mutex_t mutex;
};

void* sumFirstHalf(void* arg);
void* sumSecondHalf(void* arg);

int main()
{
    pthread_t firstHalf;
    pthread_t secondHalf;
    struct sum* result = malloc(sizeof(struct sum));

    if (result == NULL) {
        errx(1, "error : malloc failed\n");
    }

    result->result = 0;

    if (pthread_mutex_init(&result->mutex, NULL)) {
         errx(2, "error : creation of first thread failed\n");
    }

    if (pthread_create(&firstHalf, NULL, sumFirstHalf, result)) {
        errx(3, "error : creation of first thread failed\n");
    }   

    if (pthread_create(&secondHalf, NULL, sumSecondHalf, result)) {
        errx(3, "error : creation of first thread failed\n");
    }  

    if (pthread_join(firstHalf, NULL)) {
        errx(4, "error : join of first thread failed\n");
    }

    if (pthread_join(secondHalf, NULL)) {
        errx(5, "error : join of second thread failed\n");
    }

    printf("result is : %d\n", result->result);

    free(result);

    return 0;
}

void* sumFirstHalf(void* arg) {
    struct sum* sum_str = (struct sum*) arg;

    int local_sum = 0;

    for (int i = 1; i <= 49; i++)
    {
        local_sum += i;
    }
    
    pthread_mutex_lock(&sum_str->mutex);
    
    sum_str->result += local_sum;
    
    pthread_mutex_unlock(&sum_str->mutex);

    return NULL;
}

void* sumSecondHalf(void* arg) {
    struct sum* sum_str = (struct sum*) arg;

    int local_sum = 0;

    for (int i = 50; i <= 100; i++) {
        local_sum += i;
    }
    
    pthread_mutex_lock(&sum_str->mutex);
    
    sum_str->result += local_sum;
    
    pthread_mutex_unlock(&sum_str->mutex);

    return NULL;
}
