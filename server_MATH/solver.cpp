/*
 * solver.cpp
 *
 *  Created on: 6/12/2017
 *      Author: jrugis
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pari/pari.h>
using namespace std;

#include "solver.h"

int zeta(char* buffer)
{
  fprintf(stdout, "SOLVER: zeta\n");
  double* out = (double*)buffer;
  int n = *((int*)(buffer+4));  // count of (2-tuple) complex values
  double* dp = (double*)(buffer+8);
  double ubase = *(dp++);       // u (real part) base value
  double uincrement = *(dp++);  // "    "    "   increment
  double ucount = *(dp++);      // "    "    "   count
  double vbase = *(dp++);       // v (imaginary part) base value
  double vincrement = *(dp++);  // "    "    "   increment
  double vcount = *(dp++);      // "    "    "   count

  pari_sp psp = avma;        // pari stack pointer
  GEN s(cgetc(DEFAULTPREC)); // pari variable
  GEN z(cgetc(DEFAULTPREC));
  for(int i=0, v=vbase; i<vcount; i++){
    for(int j=0, u=ubase; j<ucount; j++){
      //printf("%f %f\n", ubase+(uincrement*j), vbase+(vincrement*i));
      gel(s,1) = dbltor(ubase+(uincrement*j)); // real part
      gel(s,2) = dbltor(vbase+(vincrement*i)); // imaginary part
      z = gzeta(s, DEFAULTPREC);
      *(out++) = gtodouble(greal(z));
      *(out++) = gtodouble(gimag(z));
      //printf("%f %f\n", *(out-2), *(out-1));
    }
  }
  avma = psp;                // clear the pari stack
  return(16*n);
}

int solve(char* buffer)
{
  if(strncmp(buffer, ".L01", 4) == 0) return(zeta(buffer));
  return(sprintf(buffer, "%s", "NULL"));
}

