/*
 * mapper.hpp
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

#ifndef MAPPER_HPP
#define MAPPER_HPP

#include <libplayerc++/playerc++.h>
#include <boost/thread/mutex.hpp>
#include <vector>

using namespace PlayerCc;
using namespace std;

namespace Algorithm {
  
const int32_t VW_INDEX_START = 10;
const int32_t VW_INDEX_END = 29;
const int32_t ZONE_INDEX_START = 50;
const int32_t ZONE_INDEX_END = 69;
const int32_t POWER_INDEX_START = 30;
const int32_t POWER_INDEX_END = 49;

typedef enum {
  kRawMap = 0,
  kCoverageMap,
  kBoundaryMap, //aka. VW map
  kPowerMap,
  kZoneMap,
  kRefMap,
  kPlanMap
}TypeMaps;

enum OCCUPANCY {
  EMPTY = -1,
  UNKNOWN = 0,
  OBSTACLE = 1
};

// Multition
class Mapper {
  private:
    Mapper(TypeMaps type);
    boost::mutex mutex;
    MapProxy *mp;
    int8_t *map;
    TypeMaps _type;
    uint32_t index;

    static uint32_t size;
    static uint32_t height;
    static uint32_t width;
    static double scale;
    static double origin_x;
    static double origin_y;
    static vector<int8_t> List_X,List_Y;

    void EllipseGrid(const int8_t a, const int8_t b, vector<int8_t> &X , vector<int8_t> &Y);

  public:
    static Mapper *instance(TypeMaps type);
    ~Mapper();
    
    void Reset(int8_t data = 0);
    
    // TODO: what will happen if user uses map without set map index?
    void SetMapIndex(uint32_t idx);
    
    void Save(int8_t *m = 0, uint32_t col = 0, uint32_t row = 0, int partial_width = 0);
    //void PartialSave(int8_t *m, uint32_t col, uint32_t row, int partial_width);
    
    int8_t &operator[](const uint32_t index) {
      boost::lock_guard<boost::mutex> lock(mutex);
      return map[index];
    }
    
    uint32_t GetOccupancyCount(OCCUPANCY occu) const;
        
    uint32_t GetHeight() const;
    
    uint32_t GetWidth() const;
    
    double GetResolution() const;
    
    double GetOriginX() const;
    
    double GetOriginY() const;
    
    uint32_t GetMapSize() const;

    int8_t *RequestRawMap();
    int8_t *RequestPartialMap(player_pose2d_t &pose, const uint32_t width_grid);
    int8_t *RequestVirtualMap();

    int8_t *GetRef() {
      return map;
    }
    uint32_t GetMapArrayIndex() {
      return index;
    }
    uint32_t trans_coordinate(double x);
    uint32_t GetPositionIndex(player_pose2d_t pose);
    bool check_index_in_bound(int32_t index) {
      return ((index >= 0) && (index < size));
    }
    bool check_coord_in_bound(player_pose2d_t p) {
      check_index_in_bound(GetPositionIndex(p));
    }

   bool check_virtual_wall(player_pose2d_t pose);
  };   
}
#endif
