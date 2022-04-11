
#include <math.h>

#include "complex.h"
/* ID = 0 - Print a complex number*/
void read_comp(complex *complexNum, double givenReal, double givenImg) {
  (*complexNum).img = givenImg;
  (*complexNum).real = givenReal;
}
/* ID = 1 - Print a complex number*/
void print_comp(complex  complexNum) {
  if (complexNum.img < 0) {
    printf("%f %fi\n", complexNum.real, complexNum.img);
  } else {
    printf("%f +%fi\n", complexNum.real, complexNum.img);
  }
}
/* ID = 2 Adds  two complex numbers*/
complex  add_comp(complex firstComplexNum, complex secondComplexNum) {
    complex result;
    result.real = firstComplexNum.real + secondComplexNum.real;
    result.img = firstComplexNum.img + secondComplexNum.img;
    return result;
}
/* ID = 3 subtracts  two complex numbers*/
complex  sub_comp(complex firstComplexNum, complex secondComplexNum) {
    complex result;
    result.real = firstComplexNum.real - secondComplexNum.real;
    result.img = firstComplexNum.img - secondComplexNum.img;
  return result;
}
/* ID = 4 multiplies a complex num and a real num*/
complex  mult_comp_real(complex firstComplexNum, double realNum) {
    complex result;
    result.real = firstComplexNum.real*realNum;
    result.img = firstComplexNum.img*realNum;
  return result;
}
/* ID = 5 multiplies a complex num and an img num*. (a+bi)* ci = -bc + aci*/
complex  mult_comp_img(complex firstComplexNum, double imgNum) {
    complex result;
    result.img = firstComplexNum.real * imgNum;
    result.real = -1 * firstComplexNum.img* imgNum;
  return result;
}
/* ID = 6 multiplies two complex nums - (a+bi)*c = a*c + b*c*i*/
complex  mult_comp_comp(complex firstComplexNum, complex secondComplexNum) {
    complex result;
    result = mult_comp_real(firstComplexNum, secondComplexNum.real);
    result = add_comp(result, mult_comp_img(firstComplexNum, secondComplexNum.img));
  return result;
}
/* ID = 7 calculates abs value of a complex number*/
complex abs_comp(complex complexNum) {
  double beforeSqrt;
  complex result;
  result.img = 0;
  beforeSqrt = pow(complexNum.real,2) + pow(complexNum.img,2);
  result.real = sqrt(abs(beforeSqrt));
  return result;

}
