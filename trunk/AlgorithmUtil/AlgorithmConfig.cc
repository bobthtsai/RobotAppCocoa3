/*
 * AlgorithmConfig.cc
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

#include <libconfig.h++>
#include <boost/filesystem.hpp>
#include <fstream>
#include <map>
#include "AlgorithmConfig.hpp"
//#include <typeinfo>

using namespace libconfig;

#define BOOST_FILE_SYSTEM 1
#define ERROR_QUEUE_SIZE 20

const std::string in_file_name = "algorithm.cfg";
const std::string out_file_name = "product_information.log";
const std::string x86_prefix = "brain/RobotRootFs/opt/etc/";
const std::string arm_prefix = "/opt/etc/";
const std::string out_prefix = "/tmp/etc/";

namespace Algorithm {

// Singleton
AlgorithmConfig *AlgorithmConfig::instance()
{
	static AlgorithmConfig config;
	return &config;
}
// constructor
AlgorithmConfig::AlgorithmConfig()
{
  printf("--------------------------------------------------\r\n");
  //std::cout<<"BOOST :"<<BOOST_VERSION<<std::endl;
  error_queue.resize(ERROR_QUEUE_SIZE);
  if (LoadConfig() < 0)
  {
    printf("[AlgorithmConfig] Loading Failure...\r\n");
    return;
  }
  LoadInfo();
  printf("[AlgorithmConfig] Loading done\r\n");
  printf("[AlgorithmConfig] The machine information will be record into\r\n");
  printf("\t\"%s\"\r\n", outfile.c_str());
  printf("--------------------------------------------------\r\n");
  //printf("TYPE %s\r\n", typeid(a_config).name());
  //printf("SIZE \r\n");
}
AlgorithmConfig::~AlgorithmConfig()
{
  printf("Closing %s...\r\n", __FUNCTION__);
}
int32_t AlgorithmConfig::LoadConfig()
{
  // Check file existance
  //
  if (FileExists(x86_prefix + in_file_name))
  {
    infile = x86_prefix + in_file_name;
  }
  else
  if (FileExists(arm_prefix + in_file_name))
  {
    infile = arm_prefix + in_file_name;
  }
  else
  {
    printf("[AlgorithmConfig] File \"%s\" is not exist.\r\n", in_file_name.c_str());
    printf("[AlgorithmConfig] In x86, please check folder \"%s\"\r\n", x86_prefix.c_str());
    printf("[AlgorithmConfig] In ARM, please check folder \"%s\"\r\n", arm_prefix.c_str());
    return -1;
  }
  
  // Try to read file
  //
  Config cfg;
  try
  {
    cfg.readFile(infile.c_str());
  }
  catch (...)
  {
    return -1;
  }
  printf("[AlgorithmConfig] Loads configuration file \r\n");
  printf("\t\"%s\"\r\n", infile.c_str());
  printf("[AlgorithmConfig] Load config parameters...\r\n");
  
  int i;
  i = (resolution > 0.02)?1:0;
  const Setting& root = cfg.getRoot();
  
  
  std::string paragraph;
  // Versoin
  //
  paragraph = "Version";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  const Setting &ver = root[paragraph];
  cfg.lookupValue("Version.version", version);
  cfg.lookupValue("Version.resolution", resolution);
  cfg.lookupValue("Version.laser_type", laser_type);
  printf("[AlgorithmConfig] Config version: %s\r\n", version.c_str());
  printf("[AlgorithmConfig] Resolution: %.3f\r\n", resolution);
  printf("[AlgorithmConfig] LaserType: %u\r\n", laser_type);

  
  // DEBUG behaviors & sensors
  //
  paragraph = "DEBUG_BEHAVIOR";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  const Setting &d = root[paragraph];
  //
  //Setting &a = d["DEBUG_NAME"];
  //std::string s = a[1];
  //std::cout<<"S1 "<<s<<std::endl;
  //printf("LEN %d\r\n", d.getLength());
  //
  std::map<uint32_t, INFO_NODE>::iterator it;
  for (it = behavior_table.begin(); it != behavior_table.end(); it++)
  {
    d.lookupValue((it->second).name, (it->second).debug);
  }
  
  for (it = sensor_table.begin(); it != sensor_table.end(); it++)
  {
    d.lookupValue((it->second).name, (it->second).debug);
  }

  
  // AlgorithmTask
  //
  printf("\tAlgorithmTask...\r\n");
  paragraph = "AlgorithmTask";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  cfg.lookupValue("AlgorithmTask.max_speed", a_config.max_speed);
  cfg.lookupValue("AlgorithmTask.max_turnrate", a_config.max_turnrate);
  cfg.lookupValue("AlgorithmTask.driving_proxy_index", a_config.driving_proxy_index);
  cfg.lookupValue("AlgorithmTask.position_proxy_index", a_config.position_proxy_index);
  cfg.lookupValue("AlgorithmTask.laser_proxy_index", a_config.laser_proxy_index);
  cfg.lookupValue("AlgorithmTask.planner_proxy_index", a_config.planner_proxy_index);
  cfg.lookupValue("AlgorithmTask.rawmap_proxy_index", a_config.rawmap_proxy_index);
  cfg.lookupValue("AlgorithmTask.maparray_size", a_config.maparray_size);
  cfg.lookupValue("AlgorithmTask.dispatcher_proxy_inex", a_config.dispatcher_proxy_inex);
  cfg.lookupValue("AlgorithmTask.bumper_proxy_index", a_config.bumper_proxy_index);
  cfg.lookupValue("AlgorithmTask.ir_proxy_index", a_config.ir_proxy_index);

  // ZZ
  //
  printf("\tZigzag...\r\n");
  paragraph = "BehaviorZigzag";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  const Setting &beh = root[paragraph][i];
  //const Setting &beh = r[i];

  z_config.resolution = resolution;
  beh.lookupValue("min_shift_dis", z_config.min_shift_dis);
  beh.lookupValue("shift_dis", z_config.shift_dis);
  beh.lookupValue("normal_speed", z_config.normal_speed);
  beh.lookupValue("warning_dis", z_config.warning_dis);
  beh.lookupValue("sweep_avoid_dis", z_config.sweep_avoid_dis);
  beh.lookupValue("shift_avoid_dis", z_config.shift_avoid_dis);
  beh.lookupValue("covered_test_range", z_config.covered_test_range);
  beh.lookupValue("covered_set_range", z_config.covered_set_range);
  beh.lookupValue("covered_threshold", z_config.covered_threshold);
  beh.lookupValue("direction_search_range", z_config.direction_search_range);
  beh.lookupValue("search_shift_offset", z_config.search_shift_offset);
  beh.lookupValue("cover_map_test_range", z_config.cover_map_test_range);
  beh.lookupValue("raw_map_test_range", z_config.raw_map_test_range);
  
  // Mapper
  //
  printf("\tMapper...\r\n");
  paragraph = "BehaviorMapper";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  const Setting &m = root[paragraph][i];

  m.lookupValue("covered_radius", m_config.covered_radius);
  m_config.covered_radius_m = m_config.covered_radius*resolution;

  // Escape
  //
  printf("\tEscape...\r\n");
  paragraph = "BehaviorEscape";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  cfg.lookupValue("BehaviorEscape.bumper_hold_time", e_config.bumper_hold_time);
  cfg.lookupValue("BehaviorEscape.cliff_hold_time", e_config.cliff_hold_time);
  cfg.lookupValue("BehaviorEscape.drop_hold_time", e_config.drop_hold_time);
  cfg.lookupValue("BehaviorEscape.mainbrush_hold_time", e_config.mainbrush_hold_time);
  cfg.lookupValue("BehaviorEscape.left_sidebrush_hold_time", e_config.left_sidebrush_hold_time);
  cfg.lookupValue("BehaviorEscape.right_sidebrush_hold_time", e_config.right_sidebrush_hold_time);
  cfg.lookupValue("BehaviorEscape.rapid_wheel_oc_thres", e_config.rapid_wheel_oc_thres);
  cfg.lookupValue("BehaviorEscape.wheel_oc_acc_thres", e_config.wheel_oc_acc_thres);

  // STC
  //
  printf("\tSTC...\r\n");
  paragraph = "BehaviorSTC";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  cfg.lookupValue("BehaviorSTC.brush_width", s_config.brush_width);
  cfg.lookupValue("BehaviorSTC.robot_length", s_config.robot_length);
  cfg.lookupValue("BehaviorSTC.map_threshold", s_config.map_threshold);
  cfg.lookupValue("BehaviorSTC.linear_speed", s_config.linear_speed);
  cfg.lookupValue("BehaviorSTC.smooth_turn_flag", s_config.smooth_turn_flag);
  cfg.lookupValue("BehaviorSTC.avoid_obstacles_flag", s_config.avoid_obstacles_flag);
  
  // WallFollow
  //
  printf("\tWallFollow...\r\n");
  paragraph = "BehaviorWallFollow";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  const Setting &wf1 = root["BehaviorWallFollow"][(int)laser_type];
  wf1.lookupValue("wf_right_ref_point", wf_config.wf_right_ref_point);
  wf1.lookupValue("wf_right_point", wf_config.wf_right_point);
  wf1.lookupValue("wf_front_point", wf_config.wf_front_point);
  wf1.lookupValue("wf_left_ref_point", wf_config.wf_left_ref_point);
  wf1.lookupValue("wf_left_point", wf_config.wf_left_point);
  wf1.lookupValue("wf_reference", wf_config.wf_reference);
  wf1.lookupValue("wf_warnning", wf_config.wf_warnning);
  wf1.lookupValue("wf_front_oa", wf_config.wf_front_oa);


  // Spiral
  //
  printf("\tSpiral...\r\n");
  paragraph = "BehaviorSpiral";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  cfg.lookupValue("BehaviorSpiral.spiral_radius", sp_config.spiral_radius);
  cfg.lookupValue("BehaviorSpiral.spiral_range_small", sp_config.spiral_range_small);
  cfg.lookupValue("BehaviorSpiral.spiral_range_big", sp_config.spiral_range_big);

  // Explore
  //
  printf("\tExplore...\r\n");
  paragraph = "BehaviorExplore";
  if (!root.exists(paragraph))
  {
    printf("[AlgorithmConfig] Paragraph %s is not exist.\r\n", paragraph.c_str());
    return -1;
  }
  cfg.lookupValue("BehaviorExplore.ex_pose_dur_time", ex_config.ex_pose_dur_time);
  cfg.lookupValue("BehaviorExplore.ex_harz_dur_time", ex_config.ex_harz_dur_time);

  return 0;
}
// Show the information
void AlgorithmConfig::PrintInformation()
{
  LoadInfo();
  int i = 0;
  std::map<uint32_t, INFO_NODE>::iterator it;
  printf("Sensor_Information:\r\n");
  it = sensor_table.begin();
  for (; it != sensor_table.end(); it++)
    printf("\t%s = %u\r\n", (it->second).name.c_str(), (it->second).count);

  //i = -1;
  printf("Clean_Information:\r\n");
  it = clean_table.begin();
  for (; it != clean_table.end(); it++)
    printf("\t %s = %d\r\n", (it->second).name.c_str(), (it->second).count);
  
  i = error_queue.size();
  printf("Error_Information: %d errors\r\n", i);
  for (i = 0; i < ERROR_QUEUE_SIZE; i++)
    printf("Error %d = %d (%s)\r\n", i+1, error_queue[i], error_table[error_queue[i]].name.c_str());

}
void AlgorithmConfig::setSensorInfo(const uint32_t hazard)
{
  int i = 0;
  int tmp = hazard;
  while (tmp)
  {
    sensor_table[i].count += (tmp & 1);
    tmp >>= 1;
    i++;
  }
}
void AlgorithmConfig::setSensorInfo(const uint32_t sensor, const int32_t increment, const int32_t value)
{
  if (increment == 0)
    sensor_table[sensor].count = value;
  else
    sensor_table[sensor].count += increment;
  SaveInfo();
}
void AlgorithmConfig::setCleanInfo(const uint32_t cmd, const uint32_t power, const int32_t value_c, const int32_t value_p)
{
  //std::map<uint32_t, INFO_NODE>::iterator it;
  //// assert clean
  //it = clean_table.find(cmd);
  //assert((it == clean_table.end()));
  
  //// assert power
  //it = power_table.find(cmd);
  //assert((it == power_table.end()));
   
  if (value_c != 0)
    clean_table[cmd].count = value_c;
  else
    clean_table[cmd].count += 1;
  
  if (value_p != 0)
    power_table[power].count = value_p;
  else
    power_table[power].count += 1;
  printf("\t clean: %s\r\n", clean_table[cmd].name.c_str());
  printf("\t power: %s\r\n", power_table[power].name.c_str());
  SaveInfo();
}
void AlgorithmConfig::setErrorInfo(const uint32_t error)
{
  //std::map<uint32_t, INFO_NODE>::iterator it;
  //// assert error
  //it = error_table.find(error);
  //assert((it == clean_table.end()));
  
  printf("ERROR: %s\r\n", error_table[error].name.c_str());
  if (error_queue.size() > ERROR_QUEUE_SIZE)
  {
    error_queue.pop_back();
    error_queue.resize(ERROR_QUEUE_SIZE);
  }
  error_queue.push_front((int32_t)error);
  
  SaveInfo();
}
void AlgorithmConfig::LoadInfo()
{
  Config cfg;
  
  outfile = out_prefix + out_file_name;
  if (!boost::filesystem::is_directory(out_prefix))
  {
    boost::filesystem::path dir(out_prefix);
    if (boost::filesystem::create_directory(dir))
      printf("%s does not exist, create it...\r\n", out_prefix.c_str());
  }
  if (FileExists(outfile))
  {
    cfg.readFile(outfile.c_str());
  }
  else
  {
    // if not exist, create one.
    cfg.writeFile(outfile.c_str());
  }
  
  // Read Sensor
  //
  Setting& root = cfg.getRoot();
  if (!root.exists("Sensor_Information"))
    root.add("Sensor_Information", Setting::TypeGroup);
  Setting &sensor = root["Sensor_Information"];
  
  std::map<uint32_t, INFO_NODE>::iterator it;
  int i;
  for (it = sensor_table.begin(); it != sensor_table.end(); it++)
  {
    sensor.lookupValue((it->second).name, (it->second).count);
  }
  
  // Read Clean
  //
  if (!root.exists("Clean_Information"))
    root.add("Clean_Information", Setting::TypeGroup);
  Setting &clean = root["Clean_Information"];
  
  for (it = clean_table.begin(); it != clean_table.end(); it++)
  {
    clean.lookupValue((it->second).name, (it->second).count);
  }
  
  // Read Error
  if (!root.exists("Error_Information"))
    root.add("Error_Information", Setting::TypeGroup);
  Setting &error = root["Error_Information"];
  
  if (!error.exists("Error_list"))
    error.add("Error_list", Setting::TypeList);
  Setting &list = error["Error_list"];
  
  //printf("LIST %d\r\n", list.getLength());
  for (i = 0; i < list.getLength(); i++)
  {
    Setting &a = list[i];
    a.lookupValue("ErrorID", error_queue[i]);
  }  
}
void AlgorithmConfig::SaveInfo()
{
  Config cfg;
  Setting& root = cfg.getRoot();
  
  // Sensors
  //
  if (!root.exists("Sensor_Information"))
    root.add("Sensor_Information", Setting::TypeGroup);
  Setting &sensor = root["Sensor_Information"];
  
  std::map<uint32_t, INFO_NODE>::iterator it;
  int i;
  for (it = sensor_table.begin(); it != sensor_table.end(); it++)
  {
    sensor.add((it->second).name, Setting::TypeInt) = (it->second).count;
  }
  
  // Clean
  //
  if (!root.exists("Clean_Information"))
    root.add("Clean_Information", Setting::TypeGroup);
  Setting &clean = root["Clean_Information"];
  
  
  it = clean_table.begin();
  for (; it != clean_table.end(); it++)
  {
    clean.add((it->second).name, Setting::TypeInt) = (it->second).count;
  }
  
  // Error
  //
  if (!root.exists("Error_Information"))
    root.add("Error_Information", Setting::TypeGroup);
  Setting &error = root["Error_Information"];

  error.add("Error_list", Setting::TypeList);
  Setting &list = error["Error_list"];
  for(i = 0; i < ERROR_QUEUE_SIZE; i++)
  {
    Setting &l = list.add(Setting::TypeGroup);
    l.add("ErrorSequence", Setting::TypeInt) = i+1;
    l.add("ErrorID", Setting::TypeInt) = error_queue[i];
    l.add("ErrorName", Setting::TypeString) = error_table[error_queue[i]].name;
  }

  cfg.writeFile(outfile.c_str());
}
bool AlgorithmConfig::FileExists(std::string file)
{
#if BOOST_FILE_SYSTEM
  return boost::filesystem::exists(file);
#else
  std::ifstream f(file.c_str());
  return f.is_open();
#endif
}

bool IsCreatedOnStartup(typeBehavior type_beh)
{
  return (behavior_table[type_beh].debug & (1<<0));
}
bool IsEnabledOnStartup(typeBehavior type_beh)
{
  return (behavior_table[type_beh].debug & (1<<1));
}
bool IsBehaviorDebugOn(typeBehavior type_beh)
{
  return (behavior_table[type_beh].debug & (1<<2));
}
bool IsSensorDebugOn(uint32_t type_sensor)
{
  return (bool)(sensor_table[type_sensor].debug);
}
std::string GetBehaviorName(typeBehavior type_beh)
{
  return behavior_table[type_beh].name;
}
std::string GetErrorName(uint32_t error)
{
  return error_table[error].name;
}


}// end of namespace
