/*
 gcc rkc_thd.c -o rkc_thd.exe
 ./rkc_thd.exe 1000000 10 10 rkc_rslt.txt
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZ 128


long int sum=0; /* this data is shared by the thread(s)*/


void *runner(void *param);/*threads call this function*/

//This struct is passed as an argument in thread creation to be
    //able to pass multiple values
struct sum_runner_struct{
    long int start;
    long int limit;
    long int answer;
};

//Runner method
void *runner (void *param){
    
    //We take the struct arguments (which have our indices) using sum_runner_struct
    struct sum_runner_struct *arg_struct =
                        (struct sum_runner_struct*) param;
    //get the startindex
    long int startIdx = arg_struct -> start;
    //get the endIndex
    long int endIdx = arg_struct -> limit;
    //initialize the sum
    long int sum=0;
    
    for(long int i = startIdx; i<=endIdx; i++){
        sum += i;
        //Do sum for each values
    }
    //Save the sum to answer which will be saved on the the struct args array
    arg_struct -> answer = sum;
    pthread_exit(0);
}

void run_part2(int argc, char *arg1, char *arg2, char *rsltFile){

    
    //    printf("Grand Child Threads: %li",numGrandChildThreads);
        //create/open the writable file and initialize pointer
    FILE *fptr = fopen(rsltFile, "w");
    if(fptr == NULL){
        printf("Could not open File");
    }
    
    /* sumupto from command line arguments and num of threads*/
    long int sumTill = atol(arg1);
    
    long int numThreads = atoi(arg2);
    //make an array of our struct to pass values for each thread
    struct sum_runner_struct args[numThreads];

    // insert code here...
    pthread_t tid[numThreads]; /* the thread identifier array */
    pthread_attr_t attr[numThreads]; /*set of thread attributes array */

   
    long int addsBy = (sumTill/numThreads)-1;
    
    /*set the default attributes of the thread*/
    for(int i =0; i< numThreads; i++){
        //calculate the start and end indices for each thread and pass it to struct args
        args[i].start = ((addsBy+1)*i)+1;
        args[i].limit = args[i].start + addsBy;
        
    pthread_attr_init(&attr[i]); /*create the thread */
        
    /*create the thread using all the arguments inside args struct */
    pthread_create(&tid[i], &attr[i], runner, &args[i]);

    }
    

    //initialize total sum
    long int totalSum=0;
    //For each threads find the start and end index and print them out
    for(int i =0; i< numThreads; i++){
    /*wait for the thread to exit*/
    pthread_join(tid[i], NULL);
        //printouts of the each threads created with individual sum
        printf("Sum for thread %d is %li\n", i, totalSum += args[i].answer);
        printf("Child start=: %li\n",args[i].start);
        printf("Child End=: %li\n", args[i].limit);
       
        //Write the prinouts on the file pointr fptr

        fprintf(fptr,"Sum for thread %d is %li\n", i, totalSum += args[i].answer);
        fprintf(fptr,"Child start=: %li\n",args[i].start);
        fprintf(fptr,"Child End=: %li\n", args[i].limit);
    }
    //prinout and file write the total sum of all threads
    fprintf(fptr,"Total Sum is %li\n", totalSum);
    printf("Total Sum is %li\n", totalSum);


}

void run_part3(int argc, char *arg1, char *arg2, char *arg3, char *rsltFile){
    //indices of the sum that needs to be calculated upto
    long int sumTill = atol(arg1);
    //number of childrenThreads
    long int numChildThreads = atoi(arg2);
    //number of grandChildren threads
    long int numGrandChildThreads = atoi(arg3) * numChildThreads;
    
//    printf("Grand Child Threads: %li",numGrandChildThreads);
    //create/open the writable file and initialize pointer
    FILE *fptr = fopen(rsltFile, "w");
    if(fptr == NULL){
        printf("Could not open File");
    }
    
    //make an array of our struct to pass values for each thread
    struct sum_runner_struct args[numGrandChildThreads];
    
    // insert code here...
    //Create number of threads for grandchildren
    pthread_t tid[numGrandChildThreads]; /* the thread identifier array */
    pthread_attr_t attr[numGrandChildThreads]; /*set of thread attributes */

   //calculate the difference of sums between each threads
    long int addsBy = (sumTill/numGrandChildThreads)-1;
    
    /*set the default attributes of the threads to pass on struct*/
    for(int i =0; i< numGrandChildThreads; i++){
        args[i].start = ((addsBy+1)*i)+1;
        args[i].limit = args[i].start + addsBy;
      
        //arrays of attributes
    pthread_attr_init(&attr[i]);
        
        
    /*create the thread using all the arguments inside args struct */
    pthread_create(&tid[i], &attr[i], runner, &args[i]);

    }
    

    long int totalSum=0;
    printf("Parent: Start of Execution part 3\n");
    fprintf(fptr, "Parent: Start of Execution part 3\n");
    
    for(int i =0; i< numGrandChildThreads; i++){
    /*wait for the thread to exit*/
    pthread_join(tid[i], NULL);
        //Printouts of total sum for parent, start of each grandchild and end of them
        printf("Sum for thread %d is %li\n", i, totalSum += args[i].answer);
        printf("GrandChild start=: %li\n",args[i].start);
        printf("GrandChild End=: %li\n", args[i].limit);
        //Write the prinouts un filr pointr fptr
        fprintf(fptr,"Sum for thread %d is %li\n", i, totalSum += args[i].answer);
        fprintf(fptr,"GrandChild start=: %li\n",args[i].start);
        fprintf(fptr,"GrandChild End=: %li\n", args[i].limit);
    }
    //Printout and writeout of total summ of all threads
    printf("Total Sum is %li\n", totalSum);
    fprintf(fptr,"Total Sum is %li\n", totalSum);
    
    
}


int main(int argc,  char * argv[]) {
    /*Check the right number of arguments*/
    if(argc != 5){
        printf("Wrong number of command line arguments");
        return 1;
    }
    //Run part2 of project takes in the arguments from cmd line including resultfile
    run_part2(argc, argv[1], argv[2], argv[4]);
    
    //Run part3 of project takes in the arguments from cmd line including resultfile
    run_part3(argc, argv[1], argv[2], argv[3], argv[4]);
    
    return 0;

}

