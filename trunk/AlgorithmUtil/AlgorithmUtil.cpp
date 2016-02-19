/*
 * AlgorithmUtil.cpp
 * 
 * Copyright 2013 Allenc <allenhcchen@msi.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <math.h>
#include "AlgorithmUtil.hpp"

namespace Algorithm {

//uint8_t IsBound(int8_t *m, player_pose2d_t cur_pose)
//{
  //player_pose2d_t p, t;
  //p.px = cur_pose.px + (0.02 * 5);
  //for (int y = -2; y <= 2; y++)
  //{
    //p.py = cur_pose.py + (y * 0.02);
    //t.px = (p.px * cos(cur_pose.pa)) - (p.py * sin(cur_pose.pa));
    //t.py = (p.px * sin(cur_pose.pa)) + (p.py * cos(cur_pose.pa));
    //int32_t index = m->GetPositionIndex(t);
    //if ((*m)[index] > 0)
      //return 1;
  //}
  //return 0;
//}
double angle_diff(double current, double target)
{
  double a = DTOR(current);
  double b = DTOR(target);
  
	double d1, d2;
	a = NORMALIZE(a);
	b = NORMALIZE(b);
	d1 = a-b;
	d2 = 2*M_PI - fabs(d1);
	if(d1 > 0)
		d2 *= -1.0;
  return RTOD((fabs(d1) < fabs(d2)?d1:d2));
}
void ToggleBitIf(bool flag, uint32_t &w, uint32_t b) 
{
  w = (w & ~b) | (-flag & b);
}
int8_t ComparePose(player_pose2d_t a, player_pose2d_t b)
{
	return (((a.px == b.px) && (a.py == b.py) && (a.pa == b.pa)) > 0? 0:-1);
}
// Naive way
//
uint32_t CountBit(uint32_t number)
{
	uint8_t count = 0;
	for (; number; number >>= 1)
		count += (number & 1);
	return count;
}
uint32_t CountBit_Fast(uint32_t number)
{
	uint8_t count = 0;
	for (; number; count++)
		number &= (number - 1);
	return count;
}
uint16_t Norm360(int16_t raw_angle)
{
	return (raw_angle + 360) % 360;
}
int16_t Norm180(int16_t raw_angle)
{
	return (Norm360(raw_angle + 180) - 180);
}
TurnDirection Direction2Target(uint16_t current_degree,uint16_t target_degree)
{    
	if (current_degree == target_degree)
		return kNOTURN;
	else 
    return Norm180(target_degree - current_degree) > 0?kTURNLEFT:kTURNRIGHT;
}
TurnDirection Degree2Direction(int16_t deg)
{
  return (TurnDirection)sign(deg);
	//if (deg == 0)
		//return kNOTURN;
	//else
	//if (deg < 0)
		//return kTURNRIGHT;
	//else
		//return kTURNLEFT;
}
bool SameSign(int a, int b)
{
	if (a >= 0 && b >= 0)
		return true;
	else
	if (a <= 0 && b <= 0)
		return true;
	return false;
}
//-------------------------------------------------------------------------------------------------
// 13:55 2011/11/24, Allen
// fast then SameSign()
//-------------------------------------------------------------------------------------------------
bool OppositeSign(int16_t a, int16_t b)
{
	return (a ^ b) < 0? true:false;
}
//-------------------------------------------------------------------------------------------------
void Sorting(uint16_t array[], uint8_t left, uint8_t right)
{ 
  uint16_t key, i, j, temp; 		
  if(left < right)
  {   
     key=array[left];   
     i=left; 		
     j=right;    	
     while(i<j){			
       while(i<right && array[i]<=key) i++;
       while(j>left && array[j]>=key) j--; 
       if(i<j){
         temp=array[i];
         array[i]=array[j];
         array[j]=temp;
       } 
    }   
    temp=array[left];
    array[left]=array[j];
    array[j]=temp;
    Sorting(array,left,j-1);
    Sorting(array,j+1,right);
 }		
}
double DistanceP2P(player_pose2d_t a, player_pose2d_t b)
{
	return sqrt(pow((a.px - b.px), 2) + pow((a.py - b.py), 2));
}
int16_t PDControlYaw(PD_FIX_POINT &c, const double &current, const double &desire)
{
  c.Ref = _IQ12(desire);
  c.Fdb = _IQ12(current);
  c.Calc(&c);
  return _IQ15toI(c.Out);       
}
int16_t PDControlTurn(PD_FIX_POINT &c, const double &current, const double &desire)
{
  c.Ref = _IQ12(desire);
  c.Fdb = _IQ12(current);
  c.Calc(&c);
  int16_t angular_velocity = _IQ15toI(c.Out);
  if (abs(c.Err) <= _IQ15(3))
  {
    return 0;
  }
  return angular_velocity;
}

}//end of namespace
