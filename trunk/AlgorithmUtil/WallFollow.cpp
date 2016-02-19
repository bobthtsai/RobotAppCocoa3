#include "WallFollow.hpp"
#include "../../../core/math/MathUtils.hpp"


//-------------------------------------------------------------------------------------------------
void CalculateWALL_FOLLOW(void *WF_Str)
{
WALL_FOLLOW *v= (WALL_FOLLOW *)WF_Str;



    // Compute the error
    v->Err = v->Ref - v->Fdb;
    
    // Compute the proportional output
    v->Up = v->Kp*v->Err;

    
    // Compute the derivative output
    v->Ud = v->Kd*(v->Up - v->Up1);

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ud;     
    
    // Saturate the output
    if ( v->OutPreSat > _IQ15(v->OutMax) )                   
      v->Out =  _IQ15(v->OutMax);
    else if ( v->OutPreSat < _IQ15(v->OutMin) )
      v->Out =  _IQ15(v->OutMin);  
    else
      v->Out = v->OutPreSat;                                 
    
    // Update the previous proportional output 
    v->Up1 = v->Up; 
    
    if ( v->Sel )
      v->Out = -_IQ15toI(v->Out); 
    else
      v->Out = _IQ15toI(v->Out); 
  
}
//-------------------------------------------------------------------------------------------------
