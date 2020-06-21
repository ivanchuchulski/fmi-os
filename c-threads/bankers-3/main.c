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

int balance;
pthread_mutex_t balance_mutex;

bool stop;
pthread_mutex_t stop_mutex;

void* withdraw(void* arg);
void* deposit(void* arg);

int main(int argc, char const *argv[])
{
    int withdrawers_count;
    int depositors_count;

    if (strcmp(argv[1], "-h") == 0) 
    {
        printf("showing help : expected arguments  : [#DEPOSITORS, #WITHDRAWERS]\n");
        exit(1);
    }

    if (argc == 3) 
    { 
        withdrawers_count = atoi(argv[1]);
        depositors_count = atoi(argv[2]);
    }
    else 
    {
        withdrawers_count = 1;
        depositors_count  = 1;
    }
    
    printf("#depositors %d\n", depositors_count);
    printf("#withdrawers %d\n", withdrawers_count);

    pthread_t withdrawers[withdrawers_count];
    pthread_t depositors[depositors_count];

    // init balance and balance_mutex
    balance = 150;
    if (pthread_mutex_init(&balance_mutex, NULL)) 
    {
        errx(2, "error : creation of balance_mutex failed\n");
    }

    // init stop and stop_mutex
    stop = false;
    if (pthread_mutex_init(&stop_mutex, NULL)) 
    {
        errx(2, "error : creation of stop_mutex failed\n");
    }

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

    pthread_mutex_destroy(&balance_mutex);
    pthread_mutex_destroy(&stop_mutex);

    if (stop)
    {
        printf("bankrupt : remaining money : %d\n", balance);
    }
    else
    {
        printf("remaining money : %d\n", balance);
    }

    return 0;
}

void* withdraw(void* arg) {
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
                sleep(2);
                balance -= TRANSACTION_AMMOUNT;
            }            

        pthread_mutex_unlock(&balance_mutex);

        printf("banker_withdraw %d withdrew $%d from the bank\n", banker->id, TRANSACTION_AMMOUNT);

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

            sleep(2);
            balance += TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(&balance_mutex);

        printf("banker_deposit %d deposited $%d to the bank\n", banker->id, TRANSACTION_AMMOUNT);

        transactions_count++;
    }
    
    free(banker);

    return NULL;
}
