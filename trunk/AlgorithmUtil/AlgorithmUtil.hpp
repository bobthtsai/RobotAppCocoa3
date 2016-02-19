/*
 * AlgorithmUtil.hpp
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

#ifndef ALGORITHMUTIL_HPP
#define ALGORITHMUTIL_HPP

#include <libplayerc++/playerc++.h>
#include "math/PD_FixPoint.h"

using namespace PlayerCc;

namespace Algorithm {
  
#define VALUE(val) (1<<val)
// vw
#define ATFRONTVW(val) ((val&(1<<31))!=0)
// drop
#define ATLEFTDROP(val) ((val&VALUE(LEFT_WHEEL_OFFROAD))!=0)
#define ATRIGHTDROP(val) ((val&VALUE(RIGHT_WHEEL_OFFROAD))!=0)
// cliff
#define ATRLCLIFF(val) ((val&VALUE(REAR_LEFT_CLIFF))!=0)
#define ATRRCLIFF(val) ((val&VALUE(REAR_RIGHT_CLIFF))!=0)
#define ATFLCLIFF(val) ((val&VALUE(FORWARD_LEFT_CLIFF))!=0)
#define ATFFCLIFF(val) ((val&VALUE(FORWARD_FRONT_CLIFF))!=0)
#define ATFRCLIFF(val) ((val&VALUE(FORWARD_RIGHT_CLIFF))!=0)
// bumper
#define ATLEFT(val)  ((val&VALUE(LEFT_BUMPER))!=0)
#define ATFRONT(val) ((val&VALUE(FORWARD_BUMPER))!=0)
#define ATRIGHT(val) ((val&VALUE(RIGHT_BUMPER))!=0)

#define VEL_BOUND(speed) (int16_t)(std::min(MAXALLOW_VELOCITY,std::max(-1*MAXALLOW_VELOCITY,speed)))
#define ANG_BOUND(angular) (int16_t)(std::min(MAXALLOW_ANGULAR,std::max(-1*MAXALLOW_ANGULAR,angular)))

enum TurnDirection {
	kTURNRIGHT = -1,
	kNOTURN = 0,
  kTURNLEFT = 1,
  kTURNNOWAY = 2
};
struct SensingData {
  player_pose2d_t global_pose;  //senser global pose 
  uint16_t r;            //senser reading  
};

template <typename T>
int sign(T value) {
  return (value > T(0)) - (value < T(0));
}

//void IsBound(int8_t *m, player_pose2d_t cur_pose);
uint16_t Norm360(int16_t raw_angle);
int16_t Norm180(int16_t raw_angle);
void ToggleBitIf(bool flag, uint32_t &w, uint32_t b);
int8_t ComparePose(player_pose2d_t a, player_pose2d_t b);
uint32_t CountBit(uint32_t number);
uint32_t CountBit_Fast(uint32_t number);
TurnDirection Direction2Target(uint16_t ,uint16_t);
TurnDirection Degree2Direction(int16_t);
double angle_diff(double current, double target); // degree
bool SameSign(int16_t, int16_t);
void SwapNumber(uint16_t, uint16_t);
void Sorting(uint16_t [], uint8_t, uint8_t);
double DistanceP2P(player_pose2d_t, player_pose2d_t);
int16_t PDControlYaw(PD_FIX_POINT &c, const double &current, const double &desire);
int16_t PDControlTurn(PD_FIX_POINT &c, const double &current, const double &desire);


}//end of namespace

#endif
