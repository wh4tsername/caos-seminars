#include <pthread.h>
#include <stdio.h>

void* reverse_numbers(void* arg)
{
    int value = 0;
    if (scanf("%d", &value) != 1) {
        return NULL;
    }

    pthread_t next_thread;
    pthread_create(&next_thread, NULL, reverse_numbers, NULL);

    pthread_join(next_thread, NULL);
    printf("%d\n", value);

    return NULL;
}

int main()
{
    pthread_t init_thread;

    pthread_create(&init_thread, NULL, reverse_numbers, NULL);
    pthread_join(init_thread, NULL);

    return 0;
}
