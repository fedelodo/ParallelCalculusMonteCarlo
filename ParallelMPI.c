#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <mpi.h>
#include <stdbool.h>


#define SEED 123456

unsigned int seed = 2432324;



void writeCSV(double *xs, double *ys, bool *insides, size_t len) {
 
    FILE *fp = fopen("outputMPI.csv", "w+"); 
    
    fprintf(fp,"iteration, X, Y, Inside");
    
    for(int i=0;i<len;i++){
       //printf("%f\n", xs[i]);
      fprintf(fp,"\n%d, %d, %f, %f, %s", i+1, xs[i], ys[i], insides[i] ? "true" : "false");
    
    }   
    fclose(fp);
}

double randUint( long i ){
	seed = rand() * 1103515245 + 123456;
    srand48(seed + i);

    return (double)drand48();
}

int main (int argc, char* argv[]) {

    int N = strtol(argv[1], NULL, 10);

    int pid, numberOfNodes, error, i, count = 0, sum = 0;
    double pi = 0.0, begin = 0.0, end = 0.0, x, y, z;

    error = MPI_Init(&argc, &argv);
    /* 
    	1. Get process ID
    	2. Get processes number
    	3. Synchonize all processes
    	4. Get begin time
    */
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes);
    MPI_Barrier(MPI_COMM_WORLD);
    int elements_per_proc = N/numberOfNodes;

    MPI_Wtime();
    
    srand((int)time(0));
    //memset(points, sizeof(*points), 0);
    //Running the "simulation" for N times
    MPI_Barrier(MPI_COMM_WORLD);
    printf("%d\n", elements_per_proc);
    for (i = pid; i < elements_per_proc; i += 1) {
        double x,y;
        x = randUint(i);
        y  = randUint(i);
        //Checking if in unit circle
        if (x*x+y*y <= 1) {
            count++;  
        } 
    }

    /* 
    	1. Sum all the results
    	2. Synchonize all processes
    	3. Get end time
    */
	MPI_Reduce(&count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);   

    if (pid == 0)
    {
        pi=((double)sum/(double)N) * 4.0;
        printf("MPI : # of trials = %14ld,  num of Processors = %2d; estimate of pi is %1.16f AND an absolute error of %g\n",N, numberOfNodes, pi,fabs(pi - M_PI));
    }

    error=MPI_Finalize();
    
    return 0;
}