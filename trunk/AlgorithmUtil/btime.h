/*
 * btime.h
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

#ifndef BTIME_H
#define BTIME_H

#include <chrono>
#include "boost/date_time/posix_time/posix_time.hpp"

// This is a new API wapper from boost::posix_time
// Wapper to easy version, millisecond wise
class ptime {
  private:
    boost::posix_time::ptime time;
  public:
    // get the clock while object is building
    ptime() {
      time = boost::posix_time::microsec_clock::universal_time();
    }
    ~ptime() {}
    
    void refresh() {
      time = boost::posix_time::microsec_clock::universal_time();
    }
    long duration() {
      boost::posix_time::time_duration diff = 
        boost::posix_time::microsec_clock::universal_time() - this->time;
      return diff.total_milliseconds();
    }
    long duration(ptime &p) {
      boost::posix_time::time_duration diff = this->time - p.time;
      return diff.total_milliseconds();
    }
    long duration_second() {
      boost::posix_time::time_duration diff = 
        boost::posix_time::microsec_clock::universal_time() - this->time;
      return diff.total_seconds();      
    }
};
// Same wapper API as ptime. But uses std::chrono in c++11.
//
class c_time {
  private:
    std::chrono::high_resolution_clock::time_point time;
  public:
    // get the clock while object is building
    c_time() {
      time = std::chrono::high_resolution_clock::now();
    }
    ~c_time() {}
    
    void refresh() {
      time = std::chrono::high_resolution_clock::now();
    }
    long duration() {
        std::chrono::milliseconds ms = 
          std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - time);
      return ms.count();
    }
    long duration(c_time &c) {
        std::chrono::milliseconds ms = 
          std::chrono::duration_cast<std::chrono::milliseconds> (this->time - c.time);
      return ms.count();
    }
    long duration_microsec() {
        std::chrono::microseconds ms = 
          std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::high_resolution_clock::now() - time);
      return ms.count();      
    }
    long duration_second() {
        std::chrono::seconds s = 
          std::chrono::duration_cast<std::chrono::seconds> (std::chrono::high_resolution_clock::now() - time);
      return s.count();      
    }
    void steady() {
      //std::cout<<"system_clock steady ==> "<<std::chrono::system_clock::is_steady<<std::endl;
      //std::cout<<"high_resolution_clock steady ==> "<<std::chrono::high_resolution_clock::is_steady<<std::endl;
      //std::cout<<"steady_clock steady ==> "<<std::chrono::steady_clock::is_steady<<std::endl;
    }
};


#endif
