#include "math.h"

int abs(int x) {
  if(x < 0) {
    return x * (-1);
  } 
  return x; 
}

int min2(int a, int b) {
  return ((a < b) ? a : b); 
}
int min3(int a, int b, int c) {
  return min2(min2(a,b),c);
}
