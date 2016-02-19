/*
 * mapper.cc
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

#include <algorithm>
#include "mapper.hpp"
#include "btime.h"
#include "AlgorithmConfig.hpp"
#include "robot/robot.hpp"
#include <vector>

using namespace Algorithm;
using namespace std;

uint32_t Mapper::size = 0;
uint32_t Mapper::height = 0;
uint32_t Mapper::width = 0;
double Mapper::scale = 0.0;
double Mapper::origin_x = 0.0;
double Mapper::origin_y = 0.0;

int8_t Long_Ellipse = 20;
int8_t Short_Ellipse = 10;
vector<int8_t> Mapper::List_X ,Mapper::List_Y;
//--------------------------------------------------------------------
//EllipseGrid Transformation
void Mapper::EllipseGrid(const int8_t a, const int8_t b, vector<int8_t> &X, vector<int8_t> &Y)
{
  int8_t i,j;
  double x,m;
  bool E[b][a];
  bool p[2*b*a];

  for( i = 0 ; i < b ; ++i ){
    for( j = 0 ; j < a ; ++j ){
      //initialize
      E[i][j] = false;

      //calculation
      x = sqrt(pow(a,2)*(1-pow(i,2)/pow(b,2)));
      if(j <= x-m)
        E[i][j] = true;

      //transformation
      p[a*(b-i-1) + j] = E[i][j];
      p[a*(b+i) + j] = E[i][j];
    } 
  }

  for( i = 0 ; i < 2*b; ++i ){

    //build (X,Y) ellipse list 
    for( j = 0 ; j < a ; ++j){  
      if(p[i*a+j] == 1){  
          Y.push_back(b-i-1);
          X.push_back(j);
      }
    }
  } 
}
//---------------------------------------------------------------------
Mapper *Mapper::instance(TypeMaps type)
{
  if (type == kRawMap)
  {
    static Mapper raw_mapper(type);
    return &raw_mapper;
  }
  else
  if (type == kCoverageMap)
  {
    static Mapper cover_mapper(type);
    return &cover_mapper;
  }
  else
  if (type == kBoundaryMap)
  {
    static Mapper boundary_mapper(type);
    return &boundary_mapper;
  }
  else
  if (type == kPowerMap)
  {
    static Mapper power_mapper(type);
    return &power_mapper;
  }
  else
  if (type == kZoneMap)
  {
    static Mapper zone_mapper(type);
    return &zone_mapper;
  }
  else
  if (type == kRefMap)
  {
    static Mapper ref_mapper(type);
    return &ref_mapper;    
  }
  else
  if (type == kPlanMap)
  {
    static Mapper plan_mapper(type);
    return &plan_mapper;
  }
  else
  {
    printf("[Mapper] Unknow Mapper instance %d\r\n", type);
  }
}
Mapper::Mapper(TypeMaps type)
{
  //printf("Mapper type %u initialing...\r\n", type);
  map = NULL;
  
  if (type == kRawMap)
  {
    Robot *robot = Robot::instance();
    mp = robot->GetMapProxy();
    mp->RequestMap();
    
    height = mp->GetHeight();
    width = mp->GetWidth();
    size = height * width;
    scale = mp->GetResolution();
    origin_x = mp->GetOriginX();
    origin_y = mp->GetOriginY();
    EllipseGrid(Long_Ellipse, Short_Ellipse, List_X, List_Y);

    if (DEBUG_MAPPER)
      printf("[Mapper] RawMap INFO width %d, scale %.3f, origin %.3f\r\n", width, scale, origin_x);
  }
  _type = type;
  map = new int8_t[size];
  memset(map, 0, sizeof(int8_t)*size);
}
Mapper::~Mapper()
{
  delete [] map;
}
void Mapper::Reset(int8_t data)
{
  memset(map, data, sizeof(int8_t)*size);
}
uint32_t Mapper::GetOccupancyCount(OCCUPANCY occu) const 
{
  uint32_t c = std::count(&map[0], &map[size-1], occu);
  return c;
}
int8_t *Mapper::RequestRawMap()
{
  if (_type == kRawMap)
  {
    c_time c;
    if (COMPRESSED)
      mp->RequestMap(0, 0, 512, 512, false);
    else 
      mp->RequestMap();
    mp->GetMap(map);
    if (DEBUG_MAPPER)
      printf("Request whole MAP %ld ms\r\n", c.duration());
  }
  return map; //return array pointer
}
int8_t *Mapper::RequestPartialMap(player_pose2d_t &pose, const uint32_t width_grid)
{
  if (_type == kRawMap && width_grid <= width)
  {
    c_time c;
    
    uint32_t col = trans_coordinate(pose.px);
    uint32_t row = trans_coordinate(pose.py);
    col = std::max((uint32_t)0, col-width_grid);
    row = std::max((uint32_t)0, row-width_grid);
    mp->RequestMap(col, row, width_grid, width_grid);
    mp->GetMap(map);
    if (DEBUG_MAPPER)
      printf("Request Partial MAP %ld ms\r\n", c.duration());
  }
  return map;  
}


void Mapper::SetMapIndex(uint32_t idx)
{
  if (_type != kRawMap && idx != 0)
  {
    c_time c;
    index = idx;
    Robot *robot = Robot::instance();
    mp = robot->GetMapProxy(index);
    mp->RequestMap(0, 0, width, height);
    
    if (map != NULL) delete [] map;
    
    map = new int8_t[size];
    mp->GetMap(map);
    //if (DEBUG_MAPPER)
      printf("[Mapper] Aquire map(%d), width %d, cost %ld ms\r\n", index, width, c.duration());
  }
}
void Mapper::Save(int8_t *m, uint32_t col, uint32_t row, int partial_width)
{
  if (_type == kRawMap) return; /// do not allow to write raw map (SLAM MAP)
  
  c_time c;
  if (m != 0 && partial_width != 0)
  {
    mp->SetPartialMap(m, col, row, partial_width, partial_width);
    if (DEBUG_MAPPER)
      printf("[Mapper] save partial map %d, w = %d, col = %d, row = %d, cost %.3f s\r\n", index, partial_width, col, row, c.duration()/1000.0);
  }
  else
  {
    //mp->SetWholeMap(map);
    mp->SetPartialMap(map, 0, 0, width, height);
    if (DEBUG_MAPPER)
      printf("[Mapper] save whole map %d w = %d, cost %.3f s\r\n", index, width, c.duration()/1000.0);
  }
}

uint32_t Mapper::GetHeight() const { return height; }

uint32_t Mapper::GetWidth() const { return width; }

double Mapper::GetResolution() const { return scale; }

double Mapper::GetOriginX() const { return origin_x; }

double Mapper::GetOriginY() const { return origin_y; }

uint32_t Mapper::GetMapSize() const { return size; }
uint32_t Mapper::trans_coordinate(double x)
{
  return ((uint32_t)(((x) - origin_x) / scale + 0.5));
}
uint32_t Mapper::GetPositionIndex(player_pose2d_t pose)
{
  return (trans_coordinate(pose.py) * height) + trans_coordinate(pose.px);
}

bool Mapper::check_virtual_wall(player_pose2d_t pose){

  int8_t i, j, tmp;
  player_pose2d_t Vir_pose;

  //transformation for Ellipse
  for( i = 0 ; i < List_X.size() ; ++i){    
      //Calculation
      Vir_pose.px = pose.px + List_X[i]*cos(pose.pa) - List_Y[i]*sin(pose.pa) ;
      Vir_pose.py = pose.py + List_X[i]*sin(pose.pa) + List_Y[i]*cos(pose.pa) ;
      tmp = GetPositionIndex(Vir_pose);
      if( map[tmp] == 1 )
        return true; 
    
  }
  
}
