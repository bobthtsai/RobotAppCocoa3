/*
 * commands.cc
 * 
 * Copyright 2014 Allenc <allenhcchen@msi.com>
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

#include "commands.hpp"

COMMANDS &COMMANDS::instance()
{
  static COMMANDS cmd;
  return cmd;
}
COMMANDS::COMMANDS()
{
  clean_flag = 0;
  clean_name = "";
  power_flag = 0;
  power_name = "";
  map_count = 0;
  vmap_id = 0;
  zmap_id = 0;
  pmap_id = 0;
  goal.px = goal.py = goal.pa = 0.0;
  linear_velocity = 0;
  angular_velocity = 0;
}
uint32_t COMMANDS::GetCleanFlag() const {
  return clean_flag;
}
void COMMANDS::SetCleanFlag(uint32_t flag) {
  clean_flag = flag;
}
std::string COMMANDS::GetCleanName() const {
  return clean_table[clean_flag].name;
}
void COMMANDS::SetCleanName(std::string name) {
  clean_name = name;
}
uint32_t COMMANDS::GetPowerFlag() const {
  return power_flag;
}
void COMMANDS::SetPowerFlag(uint32_t flag) {
  power_flag = flag;
}
std::string COMMANDS::GetPowerName() const {
  return power_table[power_flag].name;
}
void COMMANDS::SetPowerName(std::string name) {
  power_name = name;
}
void COMMANDS::GetGoalPose(player_pose2d_t &g) const {
  g = goal;
}
void COMMANDS::SetGoalPose(player_pose2d_t &g) {
  goal = g;
}
void COMMANDS::GetVelocity(int16_t &linear, int16_t &angular) const {
  linear = linear_velocity;
  angular = angular_velocity;
}
void COMMANDS::SetVelocity(int16_t linear, int16_t angular) {
  linear_velocity = linear;
  angular_velocity = angular;
}
bool COMMANDS::IsValid()
{
  //assert((clean_table.find(clean_flag) == clean_table.end()));
  //assert((power_table.find(power_flag) == power_table.end()));
  if (clean_table.find(clean_flag) == clean_table.end() || power_table.find(power_flag) == power_table.end())
    return false;
  return true;
}

