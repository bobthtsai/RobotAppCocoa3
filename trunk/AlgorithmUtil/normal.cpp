// Edit history
// 2010-08-12 stone: [initial] a normal distribution generator.

# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <math.h>

# include "normal.hpp"

/******************************************************************************/

int i4_huge ( void )

/******************************************************************************/
/*
  Purpose:

    I4_HUGE returns a "huge" I4.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    29 August 2006

  Author:

    John Burkardt

  Parameters:

    Output, int I4_HUGE, a "huge" integer.
*/
{
  return 2147483647;
}
/******************************************************************************/

int i4_normal ( float a, float b, int *seed )

/******************************************************************************/
/*
  Purpose:

    I4_NORMAL returns a scaled pseudonormal I4.

  Discussion:

    The normal probability distribution function (PDF) is sampled,
    with mean A and standard deviation B.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    02 July 2006

  Author:

    John Burkardt

  Parameters:

    Input, float A, the mean of the PDF.

    Input, float B, the standard deviation of the PDF.

    Input/output, int *SEED, a seed for the random number generator.

    Output, int I4_NORMAL, a sample of the normal PDF.
*/
{
  int value;

  value = r4_nint ( a + b * r4_normal_01 ( seed ) );

  return value;
}
/******************************************************************************/


/******************************************************************************/

int r4_nint ( float x )

/******************************************************************************/
/*
  Purpose:

    R4_NINT returns the nearest I4 to a R4.

  Example:

        X         R4_NINT

      1.3         1
      1.4         1
      1.5         1 or 2
      1.6         2
      0.0         0
     -0.7        -1
     -1.1        -1
     -1.6        -2

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    05 May 2006

  Author:

    John Burkardt

  Parameters:

    Input, float X, the value.

    Output, int R4_NINT, the nearest integer to X.
*/
{
  int s;
  int value;

  if ( x < 0.0 )
  {
    s = -1;
  }
  else
  {
    s = 1;
  }
  value = s * ( int ) ( fabs ( x ) + 0.5 );

  return value;
}
/******************************************************************************/

float r4_normal ( float a, float b, int *seed )

/******************************************************************************/
/*
  Purpose:

    R4_NORMAL returns a scaled pseudonormal R4.

  Discussion:

    The normal probability distribution function (PDF) is sampled,
    with mean A and standard deviation B.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    29 June 2006

  Author:

    John Burkardt

  Parameters:

    Input, float A, the mean of the PDF.

    Input, float B, the standard deviation of the PDF.

    Input/output, int *SEED, a seed for the random number generator.

    Output, float R4_NORMAL, a sample of the normal PDF.
*/
{
  float value;

  value = a + b * r4_normal_01 ( seed );

  return value;
}
/******************************************************************************/

float r4_normal_01 ( int *seed )

/******************************************************************************/
/*
  Purpose:

    R4_NORMAL_01 returns a unit pseudonormal R4.

  Discussion:

    The standard normal probability distribution function (PDF) has 
    mean 0 and standard deviation 1.

    The Box-Muller method is used, which is efficient, but 
    generates two values at a time.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    10 June 2010

  Author:

    John Burkardt

  Parameters:

    Input/output, int *SEED, a seed for the random number generator.

    Output, float R4_NORMAL_01, a normally distributed random value.
*/
{
# define R4_PI 3.1415926

  float r1;
  float r2;
  static int used = -1;
  static int seed1 = 0;
  static int seed2 = 0;
  static int seed3 = 0;
  float x;
  static float y = 0.0;

  if ( used == -1 )
  {
    used = 0;
  }
/*
  If we've used an even number of values so far, generate two more, return one,
  and save one.
*/
  if ( ( used % 2 )== 0 )
  {
    seed1 = *seed;
    r1 = r4_uniform_01 ( seed );

    if ( r1 == 0.0 )
    {
       r1 = r4_uniform_01 ( seed );
    }

    seed2 = *seed;
    r2 = r4_uniform_01 ( seed );
    seed3 = *seed;
    *seed = seed2;

    x = sqrt ( - 2.0 * log ( r1 ) ) * cos ( 2.0 * R4_PI * r2 );
    y = sqrt ( - 2.0 * log ( r1 ) ) * sin ( 2.0 * R4_PI * r2 );
  }
/*
  Otherwise, return the second, saved, value and the corresponding
  value of SEED.
*/
  else
  {
    x = y;
    *seed = seed3;
  }

  used = used + 1;

  return x;
# undef R4_PI
}
/******************************************************************************/

float r4_uniform_01 ( int *seed )

/******************************************************************************/
/*
  Purpose:

    R4_UNIFORM_01 returns a unit pseudorandom R4.

  Discussion:

    This routine implements the recursion

      seed = 16807 * seed mod ( 2**31 - 1 )
      r4_uniform_01 = seed / ( 2**31 - 1 )

    The integer arithmetic never requires more than 32 bits,
    including a sign bit.

    If the initial seed is 12345, then the first three computations are

      Input     Output      R4_UNIFORM_01
      SEED      SEED

         12345   207482415  0.096616
     207482415  1790989824  0.833995
    1790989824  2035175616  0.947702

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    16 November 2004

  Author:

    John Burkardt

  Reference:

    Paul Bratley, Bennett Fox, Linus Schrage,
    A Guide to Simulation,
    Springer Verlag, pages 201-202, 1983.

    Pierre L'Ecuyer,
    Random Number Generation,
    in Handbook of Simulation
    edited by Jerry Banks,
    Wiley Interscience, page 95, 1998.

    Bennett Fox,
    Algorithm 647:
    Implementation and Relative Efficiency of Quasirandom
    Sequence Generators,
    ACM Transactions on Mathematical Software,
    Volume 12, Number 4, pages 362-376, 1986.

    Peter Lewis, Allen Goodman, James Miller,
    A Pseudo-Random Number Generator for the System/360,
    IBM Systems Journal,
    Volume 8, pages 136-143, 1969.

  Parameters:

    Input/output, int *SEED, the "seed" value.  Normally, this
    value should not be 0.  On output, SEED has been updated.

    Output, float R4_UNIFORM_01, a new pseudorandom variate, strictly between
    0 and 1.
*/
{
  int k;
  float r;

  k = *seed / 127773;

  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

  if ( *seed < 0 )
  {
    *seed = *seed + 2147483647;
  }
/*
  Although SEED can be represented exactly as a 32 bit integer,
  it generally cannot be represented exactly as a 32 bit real number!
*/
  r = ( float ) ( *seed ) * 4.656612875E-10;

  return r;
}

///******************************************************************************/
//
//void timestamp ( void )
//
///******************************************************************************/
///*
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    May 31 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 October 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//*/
//{
//# define TIME_SIZE 40
//
//  static char time_buffer[TIME_SIZE];
//  const struct tm *tm;
//  size_t len;
//  time_t now;
//
//  now = time ( NULL );
//  tm = localtime ( &now );
//
//  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );
//
//  printf ( "%s\n", time_buffer );
//
//  return;
//# undef TIME_SIZE
//}
