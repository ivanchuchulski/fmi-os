#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>
#include <errno.h>

struct banker
{
    int id;
};

const int MAX_NUMBER_OF_TRANSACTIONS = 5;
const int TRANSACTION_AMMOUNT = 50;

int balance;
pthread_mutex_t mutex;

void* withdraw(void* arg);
void* deposit(void* arg);

int main(int argc, char const *argv[])
{
    if (argc != 3) { errx(1, "error : number of arguments should be two : <depositors#> <withdrawers#>"); }

    int withdrawers_count = atoi(argv[1]);
    int deposittors_count = atoi(argv[2]);

    pthread_t withdrawers[withdrawers_count];
    pthread_t depositors[deposittors_count];

    // init balance and mutex
    balance = 10000;
    if (pthread_mutex_init(&mutex, NULL)) 
    {
        errx(2, "error : creation of mutex failed\n");
    }

    for (int withdraw_index = 1; withdraw_index <= withdrawers_count; withdraw_index++)
    {
        struct banker* banker = malloc(sizeof(struct banker));
        banker->id = withdraw_index;
        pthread_create(&withdrawers[withdraw_index], NULL, withdraw, (void*)banker);
    }
    
    for (int deposit_index = 1; deposit_index <= deposittors_count; deposit_index++)
    {
        struct banker* banker = malloc(sizeof(struct banker));
        banker->id = deposit_index;
        pthread_create(&depositors[deposit_index], NULL, deposit, (void*)banker);
    }
    
    for (int withdraw_index = 1; withdraw_index <= withdrawers_count; withdraw_index++)
    {
        pthread_join(withdrawers[withdraw_index], NULL);
    }

    for (int deposit_index = 1; deposit_index <= deposittors_count; deposit_index++)
    {
        pthread_join(depositors[deposit_index], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("remaining money : %d\n", balance);

    return 0;
}

void* withdraw(void* arg) {
    struct banker* banker = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(&mutex);
            sleep(1);
            balance -= TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(&mutex);

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
        pthread_mutex_lock(&mutex);
            sleep(1);
            balance += TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(&mutex);

        printf("banker_deposit %d deposited $%d to the bank\n", banker->id, TRANSACTION_AMMOUNT);

        transactions_count++;
    }
    
    free(banker);

    return NULL;
}
