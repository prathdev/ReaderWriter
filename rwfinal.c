/* Program to implement Readers-writers algorithm*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#define MAX 10  //no of threads
#define READCOUNT 6 //max no of threads handling read op
#define WRITECOUNT 4   //max no of threads handling write op
#define READMAX 3  //max no of read operations on a file
#define WRITEMAX 3 

sem_t mutex1;
sem_t wrt1;
sem_t master;
sem_t mutex2;
sem_t wrt2;

sem_t mutex3;
sem_t wrt3;
int arr[]={0,0,1,1,0,1,0,1,0,0};  //contains mode of op of thread read/write
int fileArr[]={0,0,1,2,1,1,0,1,2,2}; //contains file no handled by thread
int readcount[3]={0,0,0};
int writecount[3]={0,0,0};

void *Writeprocess(void *i)
{
	int j,x;
	FILE *fp;
	
	long tid=(long)i;
	int filename=fileArr[tid];
	
	
	
	
	if(filename==0)
	sem_wait(&wrt1);
	
	else if(filename==1)
	sem_wait(&wrt2);
	
	else
	sem_wait(&wrt3);
	
	if(filename==0)
	sem_wait(&mutex1);
	else if(filename==1)
	sem_wait(&mutex2);
	else if(filename==2)
	sem_wait(&mutex3);
	
	
	writecount[filename]++;
	sem_wait(&master);

		
	fp=fopen("masterfile","a");
	fprintf(fp,"\n%-8ld\tw\t\tfile%-4d\t%-9d\t%-9d\n",tid,filename, readcount[filename],writecount[filename]);
	fclose(fp);
	sem_post(&master);
	
	
	if(filename==0)
	system("bash scripttt.sh 1 file0");
	
	else if(filename==1)
	system("bash scripttt.sh 1 file1");
	
	else
	system("bash scripttt.sh 1 file2");
	
	writecount[filename]--;
	
	
	if(filename==0)
	sem_post(&wrt1);
	else if(filename==1)
	sem_post(&wrt2);
	else
	sem_post(&wrt3);
	
	if(filename==0)	
	sem_post(&mutex1);
	else if(filename==1)
	sem_post(&mutex2);
	else
	sem_post(&mutex3);
	
	
	
}


	void *Readprocess(void *i)
{
	
	
	int x;
	
	int j;
	long tid=(long)i;
	FILE *fp;	
	int filename=fileArr[tid];
	
	if(filename==0)
	sem_wait(&mutex1);
	else if(filename==1)
	sem_wait(&mutex2);
	else if(filename==2)
	sem_wait(&mutex3);
	
	readcount[filename]++;
	
	if(readcount[filename]==1)
	{
		if(filename==0)
		sem_wait(&wrt1);
		else if(filename==1)
		sem_wait(&wrt2);
		else if(filename==2)
		sem_wait(&wrt3);
	}
	
	if(filename==0)
	sem_post(&mutex1);
	else if(filename==1)
	sem_post(&mutex2);
	else if(filename==2)
	sem_post(&mutex3);
	
	//write to master file
	sem_wait(&master);
	//write.. thread no, filename, accessmode, readcount of the file
	
	
	fp=fopen("masterfile","a");
	fprintf(fp,"\n%-8ld\tr\t\tfile%-4d\t%-9d\t%-9d\n",tid,filename,readcount[filename],writecount[filename]);
	fclose(fp);
	sem_post(&master);
	
	
	
	if(filename==0)
	system("bash scripttt.sh 0 file0");
	else if(filename==1)
	system("bash scripttt.sh 0  file1");
	else
	system("bash scripttt.sh 0 file2");
	
	if(filename==0)
	sem_wait(&mutex1);
	else if(filename==1)
	sem_wait(&mutex2);
	else
	sem_wait(&mutex3);
	
	readcount[filename]--;
	
	if(readcount[filename]==0)
	{
		if(filename==0)
		system("chmod 644 file0");
		else if(filename==1)
		system("chmod 644 file1");
		else
		system("chmod 644 file2");
		
		if(filename==0)
		sem_post(&wrt1);
		else if(filename==1)
		sem_post(&wrt2);
		else
		sem_post(&wrt3);
	}
	
	if(filename==0)	
	sem_post(&mutex1);
	else if(filename==1)
	sem_post(&mutex2);
	else
	sem_post(&mutex3);
	
}

void assignFile(int *arr)
 {
 	int i,rd;
 	int rc[3],wc[3];
 	for(i=0;i<3;i++)
 		rc[i]=wc[i]=0;
 	printf("\nFiles assigned to threads and their access mode is as follows:\n");
 	srand(time(NULL));
 	for(i=0;i<10;i++)
 	{
 		rd=random()%3; 
 		if(arr[i]==0)
 		{
 			if(rc[rd]<READMAX)
 			{
 			printf("Thread %d assigned to file %d in read mode\n\n",i,rd); 
 			fileArr[i]=rd;
 			rc[rd]++;
 			}			
 			else i--;
 		}
 		else
 		{
 		if(wc[rd]<WRITEMAX)
 		{	
 			printf("Thread %d assigned to file %d in write mode\n\n",i,rd); 
 			fileArr[i]=rd;
 			wc[rd]++;		
 		}		
 		else i--;
 		}
 	}
 }



void initialize(int *arr)
 {
 	int i,rd;
 	int rCount[]={0,0,0,0,0,0,0,0,0,0};
 	int wCount[]={0,0,0,0,0,0,0,0,0,0};
 	int rc=0,wc=0;
 	srand(time(NULL));
 	for(i=0;i<10;i++)
 	{
 		rd=random()%2;
 		if(rd==0)
 		{
 			if(READCOUNT>rc)
 			{
 				if(rCount[i]<READMAX) 
 				{
 				arr[i]=0;
 				rc++;
 				rCount[i]++;
 				}
 			}
 			else
 			{
 			
 			if(WRITECOUNT>wc)
 			{
 				if(wCount[i]<WRITEMAX)
 				{
 				arr[i]=1;
 				wc++;
 				wCount[i]++;
 				}
 			}
 			}
 		}
 		else
 		{
 			if(WRITECOUNT>wc)
 			{
 				if(wCount[i]<WRITEMAX)
 				{
 				arr[i]=1;
 				wc++;
 				wCount[i]++;
 				}
 			}
 			else
 			{
 			
 				if(READCOUNT>rc)
 			{
 				if(rCount[i]<READMAX) 
 				{
 				arr[i]=0;
 				rc++;
 				rCount[i]++;
 				}
 			}
 			
 			}
 		}
 	}
 	
 assignFile(arr);
 }
 				
 
int main()
{
	pthread_t thread[10];
	int i,rc1;
	sem_init(&mutex1,0,1);
	sem_init(&wrt1,0,1);
	sem_init(&mutex2,0,1);
	sem_init(&wrt2,0,1);
	sem_init(&mutex3,0,1);
	sem_init(&wrt3,0,1);
	
	sem_init(&master,0,1);
	
	FILE *fp;
	fp=fopen("masterfile","w");
	fprintf(fp,"ThreadNo.  AccessMode   Filename   ReadCount   WriteCount\n");
	fclose(fp);
	
	initialize(arr);
	for(i=0;i<10;i++)
	{
	if(arr[i]==0)
	{
   rc1=pthread_create( &thread[i], NULL, &Readprocess,(void *)i);   
   if( rc1 )
   {
      printf("Thread creation failed: %d\n", rc1);
   }
   }
   else
   {
   rc1=pthread_create( &thread[i], NULL, &Writeprocess,(void *)i);
   if( rc1 )
   {
      printf("Thread creation failed: %d\n", rc1);
   }
   }
	}
	
	
	
for(i=0;i<10;i++)
{
   pthread_join( thread[i], NULL);

}
	
	
}
