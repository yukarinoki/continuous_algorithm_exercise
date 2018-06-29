#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define LMD 3
#define BETA 0.1
double func(double x) {
  return exp(pow(x, 4)) - M_PI;
}
double func_derv(double x) {
  return 4 * pow(x, 3) * exp(pow(x, 4));
}
double newton(double x) {
  return x - func(x) / func_derv(x);
}
double dmpd_newton(double x){
  int j=1;
  double rcp;
  while(1){
    rcp = 1.0 / pow(LMD,j);
    if(fabs(func(x - (rcp*func(x) / func_derv(x)))) <= (1.0 - BETA*rcp)*fabs(func(x))) break;
    else j++;
    // printf("%d¥n",j); 
  }
  return x - (rcp * func(x) / func_derv(x)); 
}
int main(int argc, char *argv[]) {
  double x,dx;
  int i = 0;
  // x = atof(argv[1]); dx = x;
  int n=0,dn=0;
  double s,slu,sld;
  slu = 1.03436963;
  sld = 1.03436961;
  for (s=3.0; s >= 0.49; s-=0.1){
    n=0;dn=0;
    x = s;
    dx = s;
    while(sld > x||x > slu){
      x = newton(x);
      n++;
      //     if(n>1000000) break;
    }
    while(sld > dx||dx > slu){
      dx= dmpd_newton(dx);
      dn++; 
    }
    printf("%lf,%d,%d¥n", s, n, dn);
  }
  return 0; 
}
