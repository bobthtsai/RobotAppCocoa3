#ifndef NORMAL_HPP
#define NORMAL_HPP

int i4_huge ( void );
int i4_normal ( float a, float b, int *seed );
long int i8_normal ( double a, double b, long long int *seed );
int r4_nint ( float x );
float r4_normal ( float a, float b, int *seed );
float r4_normal_01 ( int *seed );
float r4_uniform_01 ( int *seed );

//void timestamp ( void );

#endif
