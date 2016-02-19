/*
 * AlgorithmConfig.hpp
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

#ifndef ALGORITHMCONFIG_HPP
#define ALGORITHMCONFIG_HPP

#include <libplayerc++/playerc++.h>
#include <deque>
#include "InfoTable.hpp"
#include "config.hpp"

using namespace PlayerCc;

namespace Algorithm {

  class AlgorithmConfig {
    private:
      AlgorithmConfig();
      bool FileExists(std::string file);
      int32_t LoadConfig();
      void LoadInfo();
      void SaveInfo();
      std::string infile;
      std::string outfile;
      std::string version;
      uint32_t laser_type;
      double resolution;
      ALGORITHMTASK_CONFIG a_config;
      Z_CONFIG z_config;
      MAPPER_CONFIG m_config;
      ESCAPE_CONFIG e_config;
      STC_CONFIG s_config;
      WALL_FOLLOW_CONFIG wf_config;
      SPIRAL_CONFIG sp_config;
      EXPLORE_CONFIG ex_config;
      std::deque<int32_t> error_queue;
    public:
      ~AlgorithmConfig();
      static AlgorithmConfig *instance();
      void PrintInformation();
      void GetConfig(ALGORITHMTASK_CONFIG &a) { a = a_config; }
      void GetConfig(Z_CONFIG &c) { c = z_config; }
      void GetConfig(MAPPER_CONFIG &m) { m = m_config; }
      void GetConfig(ESCAPE_CONFIG &e) { e = e_config; }
      void GetConfig(STC_CONFIG &s) { s = s_config; }
      void GetConfig(WALL_FOLLOW_CONFIG &wf) { wf = wf_config; }
      void GetConfig(SPIRAL_CONFIG &sp) { sp = sp_config; }
      void GetConfig(EXPLORE_CONFIG &ex) { ex = ex_config; }
 
      uint32_t GetLaserType() const { return laser_type; }

      void setSensorInfo(const uint32_t hazard);
      void setSensorInfo(const uint32_t sensor, const int32_t increment, const int32_t value);
      void setCleanInfo(const uint32_t cmd, const uint32_t power, const int32_t value_c, const int32_t value_p);
      void setErrorInfo(const uint32_t error);
  };
  
  bool IsCreatedOnStartup(typeBehavior type_beh);
  bool IsEnabledOnStartup(typeBehavior type_beh);  
  bool IsBehaviorDebugOn(typeBehavior type_beh);
  bool IsSensorDebugOn(uint32_t type_sensor);
  std::string GetBehaviorName(typeBehavior type_beh);
  std::string GetErrorName(uint32_t error);
}
#endif
