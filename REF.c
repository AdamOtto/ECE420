/*
    Test the result stored in "data_output" against a serial implementation.

    -----
    Compiling:
    Include "Lab4_IO.c" to compile. Set the macro "LAB4_EXTEND" defined in the "Lab4_IO.c" file to include the extended functions
    $ gcc serialtester.c Lab4_IO.c -o serialtester -lm 

    -----
    Return values:
    0      result is correct
    1      result is wrong
    2      problem size does not match
    253    no "data_output" file
    254    no "data_input" file
*/
#define LAB4_EXTEND

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "Lab4_IO.h"
#include "timer.h"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

const int MAX_STRING = 100;


int main (int argc, char* argv[]){
    struct node *nodehead;
    int nodecount;
    int *num_in_links, *num_out_links;
    double *r, *r_pre;
    int i, j;
    double damp_const;
    int iterationcount = 0;
    int collected_nodecount;
    double *collected_r;
    double cst_addapted_threshold;
    double error;
    double start, finish;
    FILE *fp;

    // Load the data and simple verification
    if ((fp = fopen("data_input", "r")) == NULL ){
    	printf("Error loading the data_output.\n");
        return 253;
    }
    fscanf(fp, "%d\n%lf\n", &collected_nodecount, &error);
    if (get_node_stat(&nodecount, &num_in_links, &num_out_links)) return 254;
    if (nodecount != collected_nodecount){
        printf("Problem size does not match!\n");
        free(num_in_links); free(num_out_links);
        return 2;
    }
    collected_r = malloc(collected_nodecount * sizeof(double));
    for ( i = 0; i < collected_nodecount; ++i)
        fscanf(fp, "%lf\n", &collected_r[i]);
    fclose(fp);

    // Adjust the threshold according to the problem size
    cst_addapted_threshold = THRESHOLD;
    
    // Calculate the result
    if (node_init(&nodehead, num_in_links, num_out_links, 0, nodecount)) return 254;
    
    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        r[i] = 1.0 / nodecount;
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;


    /*-------------------------------------------------------------------*/
    int my_rank, comm_sz;
    char greeting[MAX_STRING];  /* String storing message */

    /* Start up MPI */
    MPI_Init(NULL, NULL); 

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 



    //This is for processing and sending data
    if (my_rank != 0) {
       //Do iterations
       sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);

       //Send Message
    }




    //This is "master" thread that does all the receiving
    else {  
       /* Print my message */
       printf("Greetings from process %d of %d!\n", my_rank, comm_sz);

       int q;
       for (q = 0; q < nodecount; q++) {
          //Receive Message
       } 

       printf("Node Count: %d\n", nodecount);
       //printf("r[0]: %f\n", r[0]);

       GET_TIME(start);
       // CORE CALCULATION
       do{
           ++iterationcount;
           vec_cp(r, r_pre, nodecount);
           for ( i = 0; i < nodecount; ++i){
               r[i] = 0;
               for ( j = 0; j < nodehead[i].num_in_links; ++j)
                   r[i] += r_pre[nodehead[i].inlinks[j]] / num_out_links[nodehead[i].inlinks[j]];
               r[i] *= DAMPING_FACTOR;
               r[i] += damp_const;
           }
	   //printf("r(%d)[0]: %f\n", iterationcount, r[0]);
       }while(rel_error(r, r_pre, nodecount) >= EPSILON);
       GET_TIME(finish);

       printf("Program converges at %d th iteration.\n", iterationcount);

       printf("Done\n%lf\n", finish - start);

       // post processing
       node_destroy(nodehead, nodecount);
       free(num_in_links); free(num_out_links);

    }
    /* Shut down MPI */
    MPI_Finalize(); 
    /*-------------------------------------------------------------------*/
}
