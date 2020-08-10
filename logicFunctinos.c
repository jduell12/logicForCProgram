#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define SIZE 1000 //size of buffer 
#define END_MARKER -1 //indicated end of producer data

//buffer for input and line separator
int buffer1[SIZE];
//number of items in buffer
int countBuffer1 = 0; 
//index where producer will put next item
int prod1_idx = 0;
//index where consumer will pick up next item
int con1_idx = 0;

//buffer for line separator and plus sign thread
int buffer2[SIZE];
int countBuffer2 = 0;
int prod2_idx = 0;
int con2_idx = 0;

//buffer for plus sign thread and output thread
int buffer3[SIZE];
int countBuffer3 = 0;
int prod3_idx = 0;
int con3_idx = 0;

//initialize mutexes
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

//initialize condition variables
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

pthread_cond_t full2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty2 = PTHREAD_COND_INITIALIZER;

pthread_cond_t full3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty3 = PTHREAD_COND_INITIALIZER;




/***************************************
    Function inputThread()
Performs input on a line by line basis.
Returns an array of char containing 
the line.
***************************************/
void *inputThread(void *args){
    char doneLine[5];
    char line[1000];
    int c, c2, c3, c4, c5, c6;
    int i = 0;
    //checks if reached end of a line
    int endOfLine = 0;

     while((c = getchar()) != EOF){
        if(c == '\n'){
            c2 = getchar();
            if(c2 == 'D'){
                doneLine[0] = c2;
                c3 = getchar();
                if(c3 == 'O'){
                    doneLine[1] = c3;
                    c4 = getchar();
                    if(c4 == 'N'){
                        doneLine[2] = c4;
                        c5 = getchar();
                        if(c5 == 'E'){
                            doneLine[3] = c5;
                            c6 = getchar();
                            if(c6 != '\n'){
                                doneLine[4] = c6;
                                for(int j = 0; j < 5; j++){
                                    line[i] = doneLine[j];
                                    i++;
                                }
                                continue;
                            } else {
                                endOfLine = END_MARKER;
                                break;
                            }
                        }
                    }
                }
            } else if(c2 != 'D'){
                line[i++] = c;
                line[i++] = c2;
                continue;
            }
        }
        line[i] = c;
        i++;

        int sizeLine = strlen(line);

        // printf("%d\n", countBuffer1);

        pthread_mutex_lock(&mutex);
        while(countBuffer1 == SIZE || countBuffer1 == (SIZE - sizeLine)){
            pthread_cond_wait(&empty, &mutex);
        }
        for(int k = 0; k < sizeLine; k++){
            buffer1[prod1_idx] = line[k];
            prod1_idx = (prod1_idx + 1) % SIZE;
            countBuffer1++;
            line[k] = ' ';
        }
        
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);

     }
     
    return NULL;
}

/*********************************************
    Function consumeSeparator()
Replaces line separators with blanks;
**********************************************/
int consumeSeparator(){
        int c = buffer1[con1_idx];
        if( c == '\n'){
            c = ' ';
            buffer1[con1_idx] = c;
        }
        con1_idx = (con1_idx + 1) % SIZE;
        countBuffer1--;
        return c;
}

/*********************************************
    Function lineSeparatorThread()
Consumer that reads from buffer one and is 
a producer that writes to buffer two.
**********************************************/
void *lineSeparatorThread(void *args){
    int c = ' ';
    int i = 0;
    int value = 0;

    while(value != END_MARKER){
        pthread_mutex_lock(&mutex);

        //buffer is empty - wait for producer to signal that the buffer has data
        while(countBuffer1 == 0){
            printf("mutex empty");
            pthread_cond_wait(&full, &mutex);
        }

        value = consumeSeparator();

        printf("%c ", value);

        //lock second mutex before checking if there's space
        // pthread_mutex_lock(&mutex2);
        // while(countBuffer2 == SIZE){
        //     pthread_cond_wait(&empty2, &mutex2);
        // }

        // buffer2[prod2_idx] = value;
        // prod2_idx = (prod2_idx + 1) % SIZE;
        // countBuffer2++;

        // //signal to consumer that buffer is no longer empty
        // pthread_cond_signal(&full2);
        // //unlock mutex
        // pthread_mutex_unlock(&mutex2);

        // //signal to producer that the buffer has space
        // pthread_cond_signal(&empty);

        // //unlocak mutex
        // pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

/*********************************************
    Function consumePlustSign()
Replaces every adjacent pair of plus signs 
aka ++ to ^
**********************************************/

// int consumePlusSign(){
//     int value = buffer2[con2_idx];
//     int i = con2_idx;
//     int sizeLine = sizeof(buffer2)/sizeof(char);

//     if(value == '+'){
//         con2_idx = (con2_idx + 1) % SIZE;
//         if(buffer2[con2_idx] == '+'){
//             countBuffer2--;
//             buffer2[(con2_idx - 1) % SIZE] = '^';
//             for(int j = i+1; j < sizeLine; j++){
//                 buffer2[j] = buffer2[j+1];
//             }
//         }
//     }
//     con2_idx = (con2_idx + 1) % SIZE;
//     countBuffer2--;
//     return value;
// }

// /*********************************************
//     Function plustSignThread()
// Consumer that reads from buffer two and 
// is a producer that writes to buffer three.

// Replaces every adjacent pair of plus signs 
// aka ++ to ^
// **********************************************/
// void *plusSignThread(void *args){
//     int value = 0;

//     while(value != END_MARKER){
//         //locak mutex before checking if buffer has data
//         pthread_mutex_lock(&mutex2);

//         while(countBuffer2 == 0){
//             pthread_cond_wait(&full2, &mutex2);
//         }

//         value = consumePlusSign();

//         //lock mutex before checking if there is space on the buffer
//         pthread_mutex_lock(&mutex3);
//         while(countBuffer3 == SIZE){
//             pthread_cond_wait(&empty3, &mutex3);
//         }
//         buffer3[prod3_idx] = value;
//         prod3_idx = (prod3_idx + 1) % SIZE;
//         countBuffer3++;

//         pthread_cond_signal(&full3);
//         pthread_mutex_unlock(&mutex3);

//         pthread_cond_signal(&empty2);
//         pthread_mutex_unlock(&mutex2);
//     }

//     return NULL;

// }

// /*********************************************
//     Function outputThread()
// Consumes from buffer3 and writes output lines 
// to standard output of 80 characters with a line 
// separator at the end;
// **********************************************/
// void *outputThread(void *args){
//     int c = ' ';
//     int i = 0;
//     int charCount = 0;
//     char outputLine [81];

//     int value = 0;
//     while( value != END_MARKER){
//         //lock mutex before checking if buffer has data
//         pthread_mutex_lock(&mutex3);
//         while(countBuffer3 == 0){
//             //wait for producer to signal that the buffer has data
//             pthread_cond_wait(&full3, &mutex3);
//         }

//         value = buffer3[con3_idx];

//         if(charCount != 80){
//             outputLine[charCount] = value;
//             charCount++;
//             con3_idx = (con3_idx + 1) % SIZE;
//         } else {
//             outputLine[charCount] = '\0';
//             printf("%s\n", outputLine);
//             //clears string that will be send to output
//             for(int j = 0; j < 81; j++){
//                 outputLine[j] = ' ';
//             }

//             charCount = 0;
//         }

//         pthread_cond_signal(&empty3);
//         pthread_mutex_unlock(&mutex3);
//     }

//     return NULL;

// }

int main(int argc, char *argv[]){
    //create producer and consumer threads
    pthread_t p1, c1, p2, c2, p3, c3;
    pthread_create(&p1, NULL, inputThread, NULL);
    pthread_create(&c1, NULL, lineSeparatorThread, NULL);

    pthread_join(p1, NULL);
    pthread_join(c1, NULL);

    // pthread_create(&p2, NULL, lineSeparatorThread, NULL);
    // pthread_create(&c2, NULL, plusSignThread, NULL);

    // pthread_join(p2, NULL);
    // pthread_join(c2, NULL);

    // pthread_create(&p3, NULL, plusSignThread, NULL);
    // pthread_create(&c3, NULL, outputThread, NULL);
    

    // pthread_join(p3, NULL);
    // pthread_join(c3, NULL);

    
    return 0; 
}
