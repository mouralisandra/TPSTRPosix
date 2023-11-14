#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char* t0[10] = {NULL};
char* t1[10] = {NULL};

sem_t vide0;
sem_t vide1;

sem_t plein0;
sem_t plein1;

pthread_mutex_t mutex0;
pthread_mutex_t mutex1;

char* encrypter(char* m, int shift) {
    int length = strlen(m);
    char* result = strdup(m);

    for (int i = 0; i < length; i++) {
        if (isalpha(result[i])) {
            if (islower(result[i])) {
                result[i] = 'a' + (result[i] - 'a' + shift) % 26;
            } else {
                result[i] = 'A' + (result[i] - 'A' + shift) % 26;
            }
        }
    }

    return result;
}

void* p0(void* arg) {
    char** messages = (char**)arg;
    int ip = 0;
    for (int i = 0; messages[i] != NULL; i++) {
        char* mc = encrypter(messages[i], 4); // Shift value is 1
        sem_wait(&vide0);
        pthread_mutex_lock(&mutex0);
        t0[ip] = mc;
        printf("t0[%d]  = %s \n", ip, t0[ip]);
        pthread_mutex_unlock(&mutex0);
        ip = (ip + 1) % 10;
        sem_post(&plein0);
        usleep(1000000);
    }

    pthread_exit(NULL);
}

void* p1(void* arg) {
    int ipc = 0;
    while (1) {
        sem_wait(&plein0);
        sem_wait(&vide1);
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex0);
        t1[ipc] = t0[ipc];
        printf(" du t0 a t1[%d]  = %s \n", ipc, t1[ipc]);
        pthread_mutex_unlock(&mutex0);
        pthread_mutex_unlock(&mutex1);
        ipc = (ipc + 1) % 10;
        sem_post(&plein1);
        sem_post(&vide0);
        usleep(1000000);
    }

    pthread_exit(NULL);
}

void* p2(void* arg) {
    int ic = 0;
    while (1) {
        sem_wait(&plein1);
        pthread_mutex_lock(&mutex1);
        printf("de t1[%d]  = %s \n", ic, t1[ic]);
        pthread_mutex_unlock(&mutex1);
        ic = (ic + 1) % 10;
        sem_post(&vide1);
        usleep(3000000);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t id1, id2, id3;

    sem_init(&vide0, 0, 10);
    sem_init(&vide1, 0, 10);
    sem_init(&plein0, 0, 0);
    sem_init(&plein1, 0, 0);

    pthread_mutex_init(&mutex0, 0);
    pthread_mutex_init(&mutex1, 0);

    char* messages[] = {"hey", "cc", "BON", NULL};

    pthread_create(&id1, NULL, p0, (void*)messages);
    pthread_create(&id2, NULL, p1, NULL);
    pthread_create(&id3, NULL, p2, NULL);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    pthread_join(id3, NULL);

    return 0;
}
