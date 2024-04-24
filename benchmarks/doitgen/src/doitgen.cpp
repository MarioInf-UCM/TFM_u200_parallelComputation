#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "utilities/polybench.hpp"
#include "doitgen.hpp"

#define SIZE_R 15
#define SIZE_Q 14
#define SIZE_P 16

using namespace std;


// FUNCTION HEADERS
//*************************************
bool initArrays(double ****A, double ***C4, double **sum);
void printArrays(double ***A, double **C4, double *sum);
void freeArrays(double ***A, double **C4, double *sum);
void kernel_doitgen_CPU(double ***A, double **C4, double *sum, int nr, int nq, int np);

//*************************************
// MAIN FUNCTION - START
//*************************************
int main(int argc, char** argv){

  int nr = SIZE_R, nq = SIZE_Q, np=SIZE_P;
  double ***A=NULL;  //SIZE_R x SIZE_Q x SIZE_P
  double **C4=NULL;  //SIZE_P x SIZE_P
  double *sum=NULL;  //SIZE_P
  bool result;

  result = initArrays(&A, &C4, &sum);
  if(!result){
    cerr << "ERROR..: We couldn't init arrays" << endl;
    return 0;
  }
  
  kernel_doitgen_CPU(A, C4, sum, nr, nq, np);
  printArrays(A, C4, sum);
  freeArrays(A, C4, sum);

  return 0;
}
//*************************************
// MAIN FUNCTION - END
//*************************************


bool initArrays(double ****A, double ***C4, double **sum){
  
    *A = (double***)malloc(SIZE_R * sizeof(double**));
    if(A==NULL){ return false;}

    for (int r = 0; r < SIZE_R; r++) {
      (*A)[r] = (double**)malloc(SIZE_Q * sizeof(double*));
      if((*A)[r]==NULL){ return false;}

      for (int q = 0; q < SIZE_Q; q++) {
        (*A)[r][q] = (double*)malloc(SIZE_P * sizeof(double));
        if((*A)[r][q]==NULL){ return false;}

        for (int p = 0; p < SIZE_P; p++){
          (*A)[r][q][p] = (double) ((r*q + p)%SIZE_P) / SIZE_P;
        }
      }
    }

    *C4 = (double**)malloc(SIZE_P * sizeof(double*));
    if((*C4)==NULL){ return false;}
    
    for (int p1 = 0; p1 < SIZE_P; p1++) {
      (*C4)[p1] = (double*)malloc(SIZE_P * sizeof(double));
      if((*C4)[p1]==NULL){ return false;}

      for (int p2 = 0; p2 < SIZE_P; p2++) {
        (*C4)[p1][p2] = (double) (p1*p2 % SIZE_P) / SIZE_P;
      }
    }


    *sum = (double*)malloc(SIZE_P * sizeof(double));
    if(*sum==NULL){ return false;}
   
    for (int p = 0; p < SIZE_P; p++) {
      (*sum)[p] = 0.0;
    }

  return true;
}


void printArrays(double ***A, double **C4, double *sum){

  cout << "*******************" << endl;
  cout << "* RESULTS ARRAY A *" << endl;
  cout << "*******************" << endl;

  for (int r = 0; r < SIZE_R; r++) {
    cout << "\nARRAY A, DIMENSION R=" << r+1 << " of " << SIZE_R << endl;
    for (int q = 0; q < SIZE_Q; q++) {
        for (int p = 0; p < SIZE_P; p++) {
            printf("%.2f  ", A[r][q][p]);
        }
        printf("\n");
    }
    printf("\n");
  }

  cout << endl;
  cout << "********************" << endl;
  cout << "* RESULTS ARRAY C4 *" << endl;
  cout << "********************" << endl;

  for (int p1 = 0; p1 < SIZE_P; p1++) {
    for (int p2 = 0; p2 < SIZE_P; p2++) {
      printf("%.2f  ", C4[p1][p2]);
    }
    printf("\n");
  }
  printf("\n");

  cout << endl;
  cout << "*********************" << endl;
  cout << "* RESULTS ARRAY SUM *" << endl;
  cout << "*********************" << endl;

  for (int p1 = 0; p1 < SIZE_P; p1++) {
    printf("%.2f  ", sum[p1]);
  }
  printf("\n");

  return;
}


void freeArrays(double ***A, double **C4, double *sum){

  for (int r = 0; r < SIZE_R; r++) {
    for (int q = 0; q < SIZE_Q; q++) {
      free( A[r][q]);
    }
    free(A[r]);
  }
  free(A);

  for (int p = 0; p < SIZE_P; p++) {
    free(C4[p]);
  }
  free(C4);

  free(sum);

  return; 
}


void kernel_doitgen_CPU(double ***A, double **C4, double *sum, int nr, int nq, int np) {

    for (int r = 0; r < nr; r++){
      for (int q = 0; q < nq; q++){
        for (int p = 0; p < np; p++){
          
          for (int s = 0; s < np; s++){
            sum[p] += A[r][q][s] * C4[s][p];
          }
        }

        for (int p = 0; p < np; p++){
          A[r][q][p] = sum[p];
        }
      }
    }

  return;
}





