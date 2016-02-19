/*
 * EventNotifier.hpp
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

#ifndef EVENTNOTIFIER_HPP
#define EVENTNOTIFIER_HPP

#include <cstdint>
#include <functional>
#include <map>

typedef std::function<void()> EventHandler;

typedef enum {
  NO_EVENT,
  DOCK_STOP_EVENT,
  SPIRAL_STOP_EVENT,
  EXPLORE_STOP_EVENT,
  MOVE2TARGET_STOP_EVENT,
  ZIGZAG_STOP_EVENT,
  WF_EXCEED_DISTANCE_EVENT,
  WF_RTURN_LIMIT_EVENT,
  WF_LTURN_LIMIT_EVENT,
  WF_CIRCULAR_EVENT,
  EXAMPLE_EVENT,
  STC_ZONE_STOP_EVENT,
  SEARCH_DOCK_EVENT,
  BCE_EXCEED_TIME_EVENT
}AI_EVENT;

class EventObserver {
  public:
    static EventObserver *instance() {
      static EventObserver observer;
      return &observer;
    }
    ~EventObserver() {
      
    }
    void registerObserver(const AI_EVENT event, EventHandler obs) {
      observers[event] = obs;
    }
    void unregisterObserver(const AI_EVENT event) {
      observers[event] = NULL;
    }
    void notify(AI_EVENT event) {
      if (observers[event] != NULL)
        observers[event]();
    }
  private:
    EventObserver() {
    }
    std::map<AI_EVENT, EventHandler> observers;
};

class EventNotifier {
  public:
    EventNotifier() {
      obs = EventObserver::instance();
    }
    ~EventNotifier() {
      
    }
    void requestEvent(const AI_EVENT event) {
      obs->notify(event);
    }
  private:
    EventObserver *obs;

};
#endif
