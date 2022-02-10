#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    double x;
    double y;
    bool inside;
} extpoints;

#define SEED 123456

unsigned int seed = 676767676 ; 

void writeCSV(extpoints *points, size_t len) {
 
    FILE *fp = fopen("output.csv", "w+"); 
    
    fprintf(fp,"iteration, X, Y, Inside");
    
    for(int i=0;i<len;i++){
    
        fprintf(fp,"\n%d, %f, %f, %d",i+1, points[i].x, points[i].y, points[i].inside);
    
    }
    
    fclose(fp);
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

    long count=0; 
    double pi;

    extpoints* points = malloc(N * sizeof(extpoints)); 
    //memset(points, sizeof(*points), 0);
    //Running the "simulation" for N times
    for (int i=0; i<N; i++) {       
        //Getting the coordinates y,x ε [0,1]
        double x,y;
        points[i].x = x = randUint(i);
        points[i].y = y  = randUint(i);
        //Checking if in unit circle
        if (x*x+y*y <= 1) {
            points[i].inside = true;
            count++;  
        } else {
            points[i].inside = false;
        } 
    }
    //Calcuting the ratio and as a result the pi
    pi=(double)count/(double)N*4;

    writeCSV(points, N);

    printf("Single : # of trials = %14ld , estimate of pi is %1.16f AND an absolute error of %g\n",N,pi,fabs(pi - M_PI));
            
    return 0;
}