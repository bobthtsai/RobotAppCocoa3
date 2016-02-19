/*
 * commands.hpp
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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <boost/thread/mutex.hpp>
#include "InfoTable.hpp"

using namespace PlayerCc;
using namespace Algorithm;

typedef enum {
  WF_NORMAL = 0,
  WF_DOCK_SEARCHING
}WF_MODE;

typedef struct {
  WF_MODE mode;
  uint32_t max_allowed_distance;
}WF_POLICY;

typedef struct {
  uint32_t max_allowed_execution_time; // ms
}BCE_POLICY;

typedef struct {
  uint8_t diameter;
}SPIRAL_POLICY;

class COMMANDS {
  private:
    COMMANDS();
    boost::mutex mutex;
    uint32_t clean_flag;
    std::string clean_name;
    uint32_t power_flag;
    std::string power_name;
    player_pose2d_t goal;
    int16_t linear_velocity;
    int16_t angular_velocity;
  public:
    static COMMANDS &instance();
    ~COMMANDS(){}
    uint32_t GetCleanFlag() const;
    void SetCleanFlag(uint32_t flag);
    std::string GetCleanName() const;
    void SetCleanName(std::string name);
    uint32_t GetPowerFlag() const;
    void SetPowerFlag(uint32_t flag);
    std::string GetPowerName() const;
    void SetPowerName(std::string name);
    void GetGoalPose(player_pose2d_t &g) const;
    void SetGoalPose(player_pose2d_t &g);
    void GetVelocity(int16_t &linear, int16_t &angular) const;
    void SetVelocity(int16_t linear, int16_t angular);
    bool IsValid();
    uint32_t map_count;
    uint32_t vmap_id;
    uint32_t zmap_id;
    uint32_t pmap_id;
    WF_POLICY wf_policy;
    BCE_POLICY bce_policy;
    SPIRAL_POLICY spiral_policy;
};

#endif

