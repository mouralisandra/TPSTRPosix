#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

sem_t s1;
pthread_mutex_t mutex_camion;


void entrer(int tonnes) 
{
	if (tonnes==15)
	{
		pthread_mutex_lock(&mutex_camion);
    		sem_wait(&s1);
    		sem_wait(&s1);
    		sem_wait(&s1);
		pthread_mutex_unlock(&mutex_camion);
	}
	else  sem_wait(&s1);
}

void sortir(int tonnes)
{
	while(tonnes>0)
	{
    	sem_post(&s1);
    	tonnes=tonnes-5;
	};
}

void* voiture(void* arg)
{
	entrer(5);
	printf(" Voiture %d entre le pont \n",*((int*)arg));
	usleep(1000000);
	printf(" Voiture %d sort du pont \n",*((int*)arg));
	sortir(5);
	pthread_exit(NULL);
}
void* camion(void* arg)
{
	entrer(15);
	printf(" Camion %d entre le pont\n",*((int*)arg));
	usleep(1000000);
	printf(" Camion %d sort du pont \n",*((int*)arg));
	sortir(15);
	pthread_exit(NULL);
}



int main(int argc, char* argv[])
{
    int i;
	pthread_t id;

	sem_init(&s1,0,3);

	for(i=0; i<10;i++){

		int* j=(int*)malloc(sizeof(int));
		*j=i;
		if (i<5)
		    pthread_create(&id,NULL,camion,j);
		else 
		{
		    *j = *j -5 ;
		    pthread_create(&id,NULL,voiture,j);
		}	
	}
	pthread_exit(NULL);
	return 0;
};