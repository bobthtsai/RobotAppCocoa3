/*
 * HazardNotifier.hpp
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

#ifndef HAZARDNOTIFIER_HPP
#define HAZARDNOTIFIER_HPP

#include <cstdint>
#include <functional>
#include <map>

typedef std::function<void(uint32_t)> ObserverHandler;

class HazardNotifier {
  public:
    static HazardNotifier *instance() {
      static HazardNotifier notifier;
      return &notifier;
    }
    ~HazardNotifier() {
      
    }
    void attachObserver(const uint32_t type, ObserverHandler obs) {
      observers[type] = obs;
    }
    void detachObserver(const uint32_t type) {
      observers[type] = NULL;
    }
    void SetHazard(const uint32_t val) {
      hazard = val;
      notify(hazard);
    }
    uint32_t GetHazard() const {
      return hazard;
    }
  private:
    HazardNotifier() {
      hazard = 0;
    }
    void notify(uint32_t hazard) {
      std::map<uint32_t, ObserverHandler>::iterator it;
      for (it = observers.begin(); it != observers.end(); it++) {
        it->second(hazard);
      }
    }
    std::map<uint32_t, ObserverHandler> observers;
    uint32_t hazard;
};

class HazardObserver {
  public:
    HazardObserver() {
      notifier = HazardNotifier::instance();
    }
    ~HazardObserver() {
      
    }
    void registerObserver(const uint32_t type, ObserverHandler obs) {
      notifier->attachObserver(type, obs);
    }
    void unregisterObserver(const uint32_t type) {
      notifier->detachObserver(type);
    }
  private:
    HazardNotifier *notifier;
};
#endif
