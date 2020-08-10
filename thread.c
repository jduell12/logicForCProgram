#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

//buffer size
#define SIZE 10000
#define END_MARKER -1

char input[1000];

//buffer1
int buffer[SIZE];
int count = 0;
int prod_idx = 0;
int con_idx = 0;

//buffer 2
int buffer2[SIZE];
int count2 = 0;
int prod2_idx = 0;
int con2_idx = 0;

//buffer3
int buffer3[SIZE];
int count3 = 0;
int prod3_idx = 0;
int con3_idx = 0;

void *inputThread(void *arg);
void *lineThread(void *arg);
void *plusThread(void *arg);
void *outputThread(void *arg);

//mutexes
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

//condition variables
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

int main(void){
    int c;
    int j = 0;
    int counter = 0;
    int end = 0;

    while(counter <= 1000 && end != 1){
        while((c = getchar()) != EOF){
            input[j] = c;
            j++;
            counter++;
        }
        end = 1;
    }

    // printf("%s\n", input);

    //threads
    pthread_t i, l, p, o;
    pthread_create(&i, NULL, inputThread, NULL);
    pthread_create(&l, NULL, lineThread, NULL);
    pthread_create(&p, NULL, plusThread, NULL);
    // pthread_create(&o, NULL, outputThread, NULL);

    pthread_join(i, NULL);
    pthread_join(l, NULL);
    pthread_join(p, NULL);
    // pthread_join(o, NULL);

    return 0;
}



void *inputThread(void *args){

    //lock mutex 
    pthread_mutex_lock(&mutex);

    for(int i = 0; i < strlen(input); i++){
        buffer[prod_idx] = input[i];
        prod_idx = (prod_idx + 1) % SIZE;
        count++;  
    }
    buffer[prod_idx] = END_MARKER;
    prod_idx = (prod_idx + 1) % SIZE;
    count++;   
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex); 
    //signal consumer

    pthread_mutex_lock(&mutex);
    for(int i = 0; i < strlen(input); i++){
        printf("%c", buffer[i]);
    }
    printf("\n\n");
    pthread_mutex_unlock(&mutex);
    
}

//changes new line to space
int process(){
    int value = buffer[con_idx];
    con_idx = (con_idx + 1) % SIZE;
    count--;

    if(value == 10){
        value = 32;
    }
    return value;
}

void *lineThread(void *arg){
    char line [1000];

    //takes data from buffer 1
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < strlen(input); i++){
        int c = process();
        line[i] = c;
    }
    pthread_mutex_unlock(&mutex);

    //puts data received from buffer 1 to buffer 2
    pthread_mutex_lock(&mutex2);
    for(int i = 0; i < strlen(input); i++){
        buffer2[prod2_idx] = line[i];
        prod2_idx = (prod2_idx + 1) % SIZE;
        count2++;
    }
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex2);
    for(int i = 0; i < strlen(input); i++){
        printf("%c", buffer2[i]);
    }
    printf("\n\n");
    pthread_mutex_unlock(&mutex2);
   
}

int changePlus(int i){
    
}

void *plusThread(void *arg){

}

// void *outputThread(void *arg){

// }
