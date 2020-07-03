#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>

struct banker
{
    int id;
};

const int MAX_NUMBER_OF_TRANSACTIONS = 5;
const int TRANSACTION_AMMOUNT = 50;
const int INITIAL_BALANCE = 150;

int balance;
pthread_mutex_t balance_mutex;

bool stop;
pthread_mutex_t stop_mutex;

void* withdraw(void* arg);
void* deposit(void* arg);
int msleep(long msec);

int main(int argc, char const *argv[])
{
    int withdrawers_count;
    int depositors_count;

    if (argc == 3) 
    { 
        withdrawers_count = atoi(argv[1]);
        depositors_count = atoi(argv[2]);
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0) 
        {
            printf("showing help : expected arguments  : [#WITHDRAWERS, #DEPOSITORS]\n");
            exit(1);
        }
        else
        {
            printf("showing help : expected arguments  : [#WITHDRAWERS, #DEPOSITORS]\n");
            errx(1, "wrong argument\n");
        }
    }
    else 
    {
        withdrawers_count = 1;
        depositors_count  = 1;   
    }
    
    printf("#withdrawers %d\n", withdrawers_count);
    printf("#depositors %d\n", depositors_count);

    if (depositors_count < 1 || withdrawers_count < 1)
    {
        errx(2, "error : #DEPOSITORS and #WITHDRAWERS must be at least 1");
    }
    
    pthread_t withdrawers[withdrawers_count];
    pthread_t depositors[depositors_count];

    // init balance and balance_mutex
    balance = INITIAL_BALANCE;
    if ( pthread_mutex_init(&balance_mutex, NULL) ) { errx(3, "error : creation of balance_mutex failed\n"); }

    // init stop and stop_mutex
    stop = false;
    if ( pthread_mutex_init(&stop_mutex, NULL) ) { errx(4, "error : creation of stop_mutex failed\n"); }

    for (int withdraw_index = 1; withdraw_index <= withdrawers_count; withdraw_index++)
    {
        struct banker* banker = malloc(sizeof(struct banker));
        banker->id = withdraw_index;
        pthread_create(&withdrawers[withdraw_index], NULL, withdraw, (void*)banker);
    }

    for (int deposit_index = 1; deposit_index <= depositors_count; deposit_index++)
    {
        struct banker* banker = malloc(sizeof(struct banker));
        banker->id = deposit_index;
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

    if ( pthread_mutex_destroy(&balance_mutex) ) { errx(5, "error : destroy balance_mutex"); }
    if ( pthread_mutex_destroy(&stop_mutex) ) { errx(6, "error : destroy stop_mutex"); }

    // pretty much to check if we are correct, when the is no bankrupt
    int check_balance = INITIAL_BALANCE + (depositors_count - withdrawers_count) * MAX_NUMBER_OF_TRANSACTIONS * TRANSACTION_AMMOUNT;
    if (stop)
    {
        printf("bankrupt : remaining money : %d\n", balance);
    }
    else
    {
        printf("(remaining money) %d == %d (check)\n", balance, check_balance);
    }

    return 0;
}

void* withdraw(void* arg) 
{
    struct banker* banker = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(&balance_mutex);
            pthread_mutex_lock(&stop_mutex);
                if (stop)
                {
                    free(banker);
                    pthread_mutex_unlock(&stop_mutex);
                    pthread_mutex_unlock(&balance_mutex);
                    return NULL;
                }
            pthread_mutex_unlock(&stop_mutex);

            if ((balance - TRANSACTION_AMMOUNT) < 0) 
            {
                pthread_mutex_lock(&stop_mutex);
                    stop = true;
                    free(banker);
                pthread_mutex_unlock(&stop_mutex);
                pthread_mutex_unlock(&balance_mutex);
                return NULL;
            }
            else
            {
                msleep(200);
                balance -= TRANSACTION_AMMOUNT;
            }            

        pthread_mutex_unlock(&balance_mutex);

        printf("banker_withdraw %d withdrew $%d from the bank\n", banker->id, TRANSACTION_AMMOUNT);

        transactions_count++;
    }
    
    free(banker);

    return NULL;
}

void* deposit(void* arg) 
{
    struct banker* banker = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(&balance_mutex);
            pthread_mutex_lock(&stop_mutex);
                if (stop)
                {
                    free(banker);
                    pthread_mutex_unlock(&stop_mutex);
                    pthread_mutex_unlock(&balance_mutex);
                    return NULL;
                }
            pthread_mutex_unlock(&stop_mutex);

            msleep(200);
            balance += TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(&balance_mutex);

        printf("banker_deposit %d deposited $%d to the bank\n", banker->id, TRANSACTION_AMMOUNT);

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