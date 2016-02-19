/*
 * config.hpp
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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>

namespace Algorithm {

const uint16_t DEFAULT_CYCLE_TIME = 100;
const uint16_t MAPPER_CYCLE_TIME = 200;
const uint8_t COMPRESSED = 1;

const int16_t MAX_SPEED = 200;
const int16_t MAX_TURNRATE = 45;

const uint32_t DEBUG_ZIGZAG = 0;
const uint32_t DEBUG_ESCAPE = 0;
const uint32_t DEBUG_EXPLORE = 0;
const uint32_t DEBUG_MAPPER = 0;

const int32_t REMOTE_FORWARD_SPEED = 150;
const int32_t REMOTE_SLOW_FORWARD_SPEED = 100;
const int32_t REMOTE_ANGULAR_SPEED = 25;
#define ENABLE_CLIFF_CHECK 1
#define ENABLE_BUMPER_CHECK 1
#define ENABLE_WHEEL_DROP_CHECK 1
#define ENABLE_BIN_CHECK 0
#define ENABLE_BIN_COVER_CHECK 0
#define ENABLE_BIN_FULL_CHECK 0
#define ENABLE_DUSTSENSOR_CHECK 0
#define ENABLE_WHEEL_OC_CHECK 1
#define SAVE_WORK_PATH_PARTIALLY 0

typedef struct {
  int32_t max_speed;
  int32_t max_turnrate;
  uint32_t driving_proxy_index;
  uint32_t position_proxy_index;
  uint32_t laser_proxy_index;
  uint32_t planner_proxy_index;
  uint32_t rawmap_proxy_index;
  uint32_t maparray_size;
  uint32_t dispatcher_proxy_inex;
  uint32_t bumper_proxy_index;
  uint32_t ir_proxy_index;
}ALGORITHMTASK_CONFIG;

typedef struct {
  double resolution;
  double min_shift_dis;
  double shift_dis;
  int32_t normal_speed;
  uint32_t warning_dis;
  uint32_t sweep_avoid_dis;
  uint32_t shift_avoid_dis;
  int32_t covered_test_range;
  int32_t covered_set_range;
  int32_t covered_threshold;
  uint32_t direction_search_range;
  double search_shift_offset;
  int32_t cover_map_test_range;
  int32_t raw_map_test_range;
}Z_CONFIG;

typedef struct {
  int32_t covered_radius;
  int32_t covered_radius_m;
}MAPPER_CONFIG;

typedef struct {
  uint32_t bumper_hold_time;
  uint32_t cliff_hold_time;
  uint32_t drop_hold_time;
  uint32_t mainbrush_hold_time;
  uint32_t right_sidebrush_hold_time;
  uint32_t left_sidebrush_hold_time;
  uint32_t rapid_wheel_oc_thres;
  uint32_t wheel_oc_acc_thres;
}ESCAPE_CONFIG;

typedef struct {  
  uint32_t brush_width;
  double robot_length; // 15 cm
  double map_threshold; //  2.5 cm
  int32_t linear_speed;
  int smooth_turn_flag;
  int avoid_obstacles_flag;
}STC_CONFIG;

typedef struct {  
  int32_t wf_right_ref_point;
  int32_t wf_right_point;
  int32_t wf_front_point;
  int32_t wf_left_ref_point;
  int32_t wf_left_point;
  int32_t wf_reference; // Distance between wall
  int32_t wf_warnning; // 離牆警戒值
  int32_t wf_front_oa;
}WALL_FOLLOW_CONFIG;

typedef struct {  
  double spiral_radius;
  double spiral_range_small;
  double spiral_range_big;
}SPIRAL_CONFIG;

typedef struct {
  uint32_t ex_pose_dur_time; // 300
  uint32_t ex_harz_dur_time; // 5
}EXPLORE_CONFIG;

}
#endif
