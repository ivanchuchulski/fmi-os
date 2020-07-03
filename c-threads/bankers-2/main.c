#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>
#include <errno.h>
#include <time.h>

struct banker
{
    int id;
    int* balance;
    pthread_mutex_t* mutex;
};

const int MAX_NUMBER_OF_TRANSACTIONS = 5;
const int TRANSACTION_AMMOUNT = 50;

void* withdraw(void* arg);
void* deposit(void* arg);
int msleep(long msec);

int main(int argc, char const *argv[])
{
    if (argc != 3) { errx(1, "error : number of arguments should be two : <depositors#> <withdrawers#>"); }

    int withdrawers_count = atoi(argv[1]);
    int depositors_count = atoi(argv[2]);

    if (depositors_count < 1 || withdrawers_count < 1)
    {
        errx(2, "error : #DEPOSITORS and #WITHDRAWERS must be at least 1");
    }

    pthread_t withdrawers[withdrawers_count];
    pthread_t depositors[depositors_count];

    // init balance and mutex
    int* balance = malloc(sizeof(int));
    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));

    *balance = 10000;
    if (pthread_mutex_init(mutex, NULL)) { errx(2, "error : creation of mutex failed\n"); }

    for (int withdraw_index = 1; withdraw_index <= withdrawers_count; withdraw_index++)
    {
        struct banker* banker = malloc(sizeof(struct banker));

        banker->id = withdraw_index;
        banker->balance = balance;
        banker->mutex = mutex;

        pthread_create(&withdrawers[withdraw_index], NULL, withdraw, (void*)banker);
    }
    
    for (int deposit_index = 1; deposit_index <= depositors_count; deposit_index++)
    {
        struct banker* banker = malloc(sizeof(struct banker));
        
        banker->id = deposit_index;
        banker->balance = balance;
        banker->mutex = mutex;
        
        pthread_create(&depositors[deposit_index], NULL, deposit, (void*)banker);
    }
    
    for (int withdraw_index = 1; withdraw_index <= withdrawers_count; withdraw_index++)
    {
        pthread_join(withdrawers[withdraw_index], NULL);
    }

    for (int deposit_index = 1; deposit_index <= depositors_count; deposit_index++)
    {
        pthread_join(depositors[deposit_index], NULL);
    }

    if ( pthread_mutex_destroy(mutex) ) { errx(3, "error : destroy mutex"); }

    printf("remaining money : %d\n", *balance);

    free(balance);
    free(mutex);

    return 0;
}

void* withdraw(void* arg) {
    struct banker* banker = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(banker->mutex);
            // sleep(1);
            msleep(200);
            banker->balance -= TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(banker->mutex);

        printf("bank withdrawer %d withdrew $%d from the bank\n", banker->id, TRANSACTION_AMMOUNT);

        transactions_count++;
    }
    
    free(banker);

    return NULL;
}

void* deposit(void* arg) {
    struct banker* banker = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(banker->mutex);
            // sleep(1);
            msleep(200);
            banker->balance += TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(banker->mutex);

        printf("bank depositer %d deposited $%d to the bank\n", banker->id, TRANSACTION_AMMOUNT);

        transactions_count++;
    }
    
    free(banker);

    return NULL;
}

// source : https://stackoverflow.com/a/1157217
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do 
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}