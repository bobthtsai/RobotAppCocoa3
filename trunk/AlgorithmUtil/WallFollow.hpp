/*!
 * @date  
 *   2009/03/19
 *
 * @version 
 *   0.02
 *
 * @author 
 *   LeonLi @2010/02/26
 *
 * @todo
 *   ©µÀð¦æ¨«
 *
 *
 * @defgroup navigation
 * @defgroup WallFollow
 * @ingroup navigation
 * 
 *   Copyright (c) 2009 by <LeonLi/MSI>
 */
//-------------------------------------------------------------------------------------------------
#include "math/IQmathLib.h"
//-------------------------------------------------------------------------------------------------
#ifndef WALL_FOLLOW_H
#define WALL_FOLLOW_H
//-------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//-------------------------------------------------------------------------------------------------
/**                                    	 
  * @ingroup 
  *   WallFollow  
  *                   	 
  *   WallFollow object‚Æ
  */                   
typedef struct {
                int  Kp;                /**< PD_FixPoint P gain. */                                                                 
                int  Kd;                /**< PD_FixPoint D gain. */                                                                 
                int  Up;                /**< Variable: Proportional output. */                                                      
                int  Up1;               /**< History: Previous proportional output. */                                              
                int  Ud;                /**< Variable: Derivative output. */                                                        
                int  OutMax;            /**< Parameter: Maximum output <unsit deg/s>. */                                            
                int  OutMin;            /**< Parameter: Minimum output <unsit deg/s>. */                                            
                int  OutPreSat;         /**< Variable: Pre-saturated output. */                                                     
                int  Fdb;               /**< Input: Feedback input [Between wall distance unit<mm>]. */                             
                int  Ref;               /**< Input: Reference distance between wall [Desire distance between wall unit<mm>]. */     
                int  Out;               /**< Output: PD output [Angular velocity unit<deg/s> ]. */                                  
                int  Err;               /**< Variable: Error unit<mm>. */                                                           
                char Sel;              /**< Variable: Define Wall follow left/right right=0 left=1. */
                void (*Calc)(void *);   /**< Pointer to calculation function. */
              }WALL_FOLLOW;
//-------------------------------------------------------------------------------------------------
/**                                    	 
  * @ingroup 
  *   WallFollow  
  *                   	 
  *   Prototypes for the functions in WallFollow.C
  */                

void CalculateWALL_FOLLOW(void *WF_Str);
//-------------------------------------------------------------------------------------------------
/**                                    	 
  * @ingroup 
  *   WallFollow  
  *                   	 
  *   Default initalizer for the object.
  */                
#define WALL_FOLLOW_DEFAULTS { \
                           _IQ15(0.30), \
                           _IQ15(0.0),  \
                           0,  \
                           0,  \
                           0,  \
                           45, \
                           -45,\
                           0,  \
                           0,  \
                           0,  \
                           0,  \
                           0,  \
                           0,  \
                           &CalculateWALL_FOLLOW };            
//-------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // extern "C" 
//-------------------------------------------------------------------------------------------------
#endif  //end WALL_FOLLOW_H
