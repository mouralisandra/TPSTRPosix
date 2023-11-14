#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include  <stdlib.h>

pthread_mutex_t AB,BC,CD,EC,FB;


void  parcours_de_A(int* arg) {
    int id = *arg;
    printf("metro %d dans la station A et attend le segment AB\n", id);

    pthread_mutex_lock(&AB);
    printf("metro %d dans le segment AB\n", id);
    usleep(6000000);
    printf("metro %d dans la station B et attend le segment BC\n", id);
    pthread_mutex_unlock(&AB);

    pthread_mutex_lock(&BC);
    printf("metro %d dans le segment BC\n", id);
    usleep(6000000);
    printf("metro %d dans la station C et attend le segment CD\n", id);
    pthread_mutex_unlock(&BC);

    pthread_mutex_lock(&CD);
    printf("metro %d dans le segment CD\n", id);
    usleep(6000000);
    printf("metro %d dans la station D\n", id);
    pthread_mutex_unlock(&CD);

}

void  parcours_de_E(int* arg) {
    int id = *arg;
    printf("metro %d dans la station E et attend le segment EC\n", id);

    pthread_mutex_lock(&EC);
    printf("metro %d dans le segment EC\n", id);
    usleep(6000000);
    printf("metro %d dans la station C et attend le segment CB\n", id);
    pthread_mutex_unlock(&EC);

    pthread_mutex_lock(&BC);
    printf("metro %d dans le segment CB\n", id);
    usleep(6000000);
    printf("metro %d dans la station B\n", id);
    pthread_mutex_unlock(&BC);
}
void  parcours_de_F(int* arg) {
    int id = *arg;
    printf("metro %d dans la station F et attend le segment FB\n", id);
    
    pthread_mutex_lock(&FB);
    printf("metro %d dans le segment FB\n", id);
    usleep(6000000);
    printf("metro %d dans la station B et attend le segment BA\n", id);
    pthread_mutex_unlock(&FB);

    pthread_mutex_lock(&AB);
    printf("metro %d dans le segment BA\n", id);
    usleep(6000000);
    printf("metro %d dans la station A\n", id);
    pthread_mutex_unlock(&AB);

}


int main(int argc,char ** argv) {

    pthread_t metro1, metro2, metro3, metro4;

    pthread_mutex_init(&AB,NULL);
    pthread_mutex_init(&BC,NULL);
    pthread_mutex_init(&CD,NULL);
    pthread_mutex_init(&EC,NULL);
    pthread_mutex_init(&FB,NULL);

    int id1 = 1;
    pthread_create(&metro1, NULL, parcours_de_A,&id1 );
    int id2 = 2;
    pthread_create(&metro2, NULL, parcours_de_E, &id2);
    int id3 = 3 ;
    pthread_create(&metro3, NULL, parcours_de_F, &id3);
    int id4 = 4;
    pthread_create(&metro4, NULL, parcours_de_A,&id4 );

    pthread_join(metro1, NULL);
    pthread_join(metro2, NULL);
    pthread_join(metro3, NULL);
    pthread_join(metro4, NULL);

    return 0;
}