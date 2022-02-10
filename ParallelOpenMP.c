#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <omp.h>
#include <stdbool.h>
#include <unistd.h>

#define SEED 123456

unsigned int seed = 676767676 ; 

typedef struct {
    double x;
    double y;
    bool inside;
    int threadnum;
    int iteration;
} extpoints;

void writeCSV(extpoints *points, size_t len, int numthreads) {
 
    FILE *fp = fopen("outputOMP.csv", "w+"); 
    
    fprintf(fp,"threadnum, iteration, X, Y, Inside");
    
    for(int i=0;i<len;i++){
    
        fprintf(fp,"\n%d, %d, %f, %f, %s", points[i].threadnum, points[i].iteration, points[i].x, points[i].y, points[i].inside ? "true" : "false");
    
    }
    
    fclose(fp);
}

void writeCSVline(FILE *fp, double x, double y, bool inside, int iteration) {
    
        fprintf(fp,"\n%d, %d, %f, %f, %s", iteration,  x, y, inside);
}


//Random number generator with linear congruential generator

double randUint( long i ){
	seed = rand() * 1103515245 + 123456;
    srand48(seed + i);

    return (double)drand48();
}


int main(int argc, char* argv[])
{
    int N = (int) strtol(argv[1], NULL, 10);
    int numthreads = (int) strtol(argv[2], NULL, 10);

    long count=0;
    double pi;
    int index = 0;
    
    //extpoints* points = malloc(N * sizeof(extpoints)); 

    //Init Parallelazation with reduction techinue
    #pragma omp parallel for reduction(+: count) num_threads(numthreads)
    for (int i=0; i<N; i++) {  
        /*
        int threadnum = omp_get_thread_num();
        char prefix[20] = "outputOMP";
        char suffix[5] = ".csv";
        char number[2];

        sprintf(number, "%d", threadnum);        

        strcat( prefix, number);
        strcat( prefix, suffix);

        
        FILE *fp = fopen(prefix, "a");
*/
        double x,y;
        bool inside;
        x = randUint(i);
        y  = randUint(i);
        //Checking if in unit circle
        if (x*x+y*y <= 1) {
            //inside = true;
            count++;  
        } 
        /*else {
            inside = false;
        } 
        index++;
        fprintf(fp,"\n%d, %f, %f, %d", index,  x, y, inside);       
        fclose(fp);     */
    }

    //Calcuting the ratio and as a result the pi
    pi=((double)count/(double)N) * 4.0;
    
    printf("OpenMP : # of trials = %14ld , estimate of pi is %1.16f AND an absolute error of %g\n",N,pi,fabs(pi - M_PI));

    return 0;
}

    //  export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"

