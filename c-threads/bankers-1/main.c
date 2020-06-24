#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>
#include <errno.h>

struct banker
{
    int balance;
    pthread_mutex_t mutex;
};

const int MAX_NUMBER_OF_TRANSACTIONS = 5;
const int TRANSACTION_AMMOUNT = 50;

void* withdraw(void* arg);
void* deposit(void* arg);

int main()
{
    pthread_t withdraw_thread;
    pthread_t deposit_thread;
    struct banker* bank_account = malloc(sizeof(struct banker));

    if (bank_account == NULL) { errx(1, "error : malloc failed\n"); }

    bank_account->balance = 10000;
    if (pthread_mutex_init(&bank_account->mutex, NULL)) { errx(2, "error : creation of mutex failed\n"); }

    if ( pthread_create(&withdraw_thread, NULL, withdraw, bank_account) ) { errx(3, "error : creation of withdraw_thread failed\n"); };
    if ( pthread_create(&deposit_thread, NULL, deposit, bank_account) ) { errx(4, "error : creation of deposit_thread failed\n"); };

    if ( pthread_join(withdraw_thread, NULL) ) { errx(5, "error : join of withdraw_thread failed\n"); }
    if ( pthread_join(deposit_thread, NULL) ) { errx(6, "error : join of deposit_thread failed\n"); }

    if ( pthread_mutex_destroy(&bank_account->mutex) ) { errx(7, "error : mutex destroy failed\n");}

    printf("remaining money : %d\n", bank_account->balance);

    free(bank_account);

    return 0;
}

void* withdraw(void* arg) {
    struct banker* bank_account = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(&bank_account->mutex);
            sleep(2);
            bank_account->balance -= TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(&bank_account->mutex);

        printf("bank withdrawer 1 withdrew $%d from the bank\n", TRANSACTION_AMMOUNT);

        transactions_count++;
    }

    return NULL;
}

void* deposit(void* arg) {
    struct banker* bank_account = (struct banker*) arg;
    int transactions_count = 0;
    
    while (transactions_count < MAX_NUMBER_OF_TRANSACTIONS)
    {
        pthread_mutex_lock(&bank_account->mutex);
            sleep(2);
            bank_account->balance += TRANSACTION_AMMOUNT;
        pthread_mutex_unlock(&bank_account->mutex);

        printf("bank depositer 1 deposited $%d to the bank\n", TRANSACTION_AMMOUNT);

        transactions_count++;
    }
    
    return NULL;
}
