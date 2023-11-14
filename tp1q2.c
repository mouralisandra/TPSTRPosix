#include <stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define camions	8
#define voitures	5
#define vehicules  (camions+voitures)
#define en_attente 1
#define rien 2
#define traverser 3

pthread_mutex_t sectionCritique;
sem_t semaphore[vehicules];
int state[vehicules];
int camions_bloque=0; //nombre camions bloquees
int limit=0; 

void entrer(int tonnes, int id) 
{
	pthread_mutex_lock(&sectionCritique);

	if(limit+tonnes <= 15)
	{
		limit=limit+tonnes;
		state[id]=traverser;
		sem_post(&semaphore[id]);
	}
	else	{
		state[id]=en_attente;
		if(tonnes==15)
			camions_bloque++;
		}

	pthread_mutex_unlock(&sectionCritique);
	sem_wait(&semaphore[id]);
}



void sortir(int tonnes, int pid)
{
	int i;

	pthread_mutex_lock(&sectionCritique);

	state[pid]=rien;
	limit=limit-tonnes;

	for(i=0; i<camions;i++)
		if ( (state[i]==en_attente) && (limit==0) )
		{
			sem_post(&semaphore[i]);
			limit=15;
			camions_bloque--;
		}

	for(i=camions; i<vehicules;i++)
		if ( (limit<15) && (camions_bloque==0)	&& (state[i]==en_attente) )
			{
			limit=limit+5;
			sem_post(&semaphore[i]);
			}

	pthread_mutex_unlock(&sectionCritique);
}

void* voiture(void* arg)
{
	entrer(5, *((int*)arg));
	printf(" Voiture %d entre le pont \n",*((int*)arg)-camions);
	usleep(1000000) ;
	printf(" Voiture %d sort du pont\n",*((int*)arg)-camions);
	sortir(5, *((int*)arg));
	pthread_exit(NULL);
}

void* camion(void* arg)
{
	entrer(15,*((int*)arg));
	printf(" Camion %d entre le pont\n",*((int*)arg));
	usleep(1000000) ;
	printf(" Camion %d sort du pont\n",*((int*)arg));
	sortir(15,*((int*)arg));
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int i;
	pthread_t id;


	for(i=0; i<13;i++)
	{
		state[i]=rien;
		sem_init(&semaphore[i],0,0);
	}

	pthread_mutex_init(&sectionCritique,0);

	for(i=0; i<13;i++)
	{
		int* j=(int*)malloc(sizeof(int));
		*j=i;
		if (i<camions)
			pthread_create(&id,NULL,camion,j);
		else	pthread_create(&id,NULL,voiture,j);
	}


	pthread_exit(NULL);
};