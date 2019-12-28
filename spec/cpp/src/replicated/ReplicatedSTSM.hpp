#pragma once
#ifndef LIBBFT_SRC_CPP_REPLICATED_STSM_HPP
#define LIBBFT_SRC_CPP_REPLICATED_STSM_HPP

// system includes
#include <cstddef>
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "../events/Event.hpp"
#include "../machine/TimedStateMachine.hpp"
#include "../single/SingleTimerStateMachine.hpp"

#include "MachineContext.hpp"
#include "MultiContext.hpp"
#include "../events/ScheduledEvent.hpp"

namespace libbft {

template<class Param>
using MultiState = std::vector<State<MultiContext<Param>>*>;

template<class Param = std::nullptr_t>
class ReplicatedSTSM : public TimedStateMachine<MultiState<Param>, MultiContext<Param>>
{
public:
   // includes several internal machines
   std::vector<SingleTimerStateMachine<MultiContext<Param>>*> machines;

   // includes several internal machines
   std::vector<ScheduledEvent> scheduledEvents;

   // requires global transitions here... from inheritance. "Inherit or not inherit, that's the question"
   // create again with other name...
   //vector<Scheduled<Transition<MultiContext<Param>>>> scheduledTransitions;
   // scheduled transitions may perhaps launch events on Action... must see if both are necessary

   // watchdog timer
   Timer* watchdog{ nullptr };

   // MaxTime -1.0 means infinite time
   // positive time is real expiration time
   void setWatchdog(double MaxTime)
   {
      watchdog = (new Timer())->init(MaxTime);
   }

   //void scheduleGlobalTransition(Scheduled<Transition<MultiContext<Param>>> sch)
   //{
   //   scheduledTransitions.push_back(sch);
   //}

   //void scheduleGlobalTransition(Timer* when, int machine, Transition<MultiContext<Param>>* t)
   //{
   //   scheduledTransitions.push_back(Scheduled(t, when, machine));
   //}

   //void scheduleEvent(Timer* when, int machine, Event<MultiContext<Param>>* e)
   void scheduleEvent(double countdown, MachineId machine, std::string _name, std::vector<std::string> eventParams)
   {
      scheduledEvents.push_back(ScheduledEvent(_name, countdown, machine, eventParams));
   }

public:
   ReplicatedSTSM(Clock* _clock = nullptr, MachineId _me = 0, std::string _name = "")
     : TimedStateMachine<MultiState<Param>, MultiContext<Param>>(_clock, _me, _name)
   {
   }

   virtual ~ReplicatedSTSM()
   {
      // TODO: delete lot's of stuff
      // unique_ptr the clock perhaps?
   }

   void registerMachine(SingleTimerStateMachine<MultiContext<Param>>* m)
   {
      // something else?
      machines.push_back(m);
   }

   void launchScheduledEvents(MultiContext<Param>* p)
   {
      std::cout << "launching scheduled events!" << std::endl;
      // launch all scheduled events
      for (unsigned i = 0; i < scheduledEvents.size(); i++) {
         ScheduledEvent e = scheduledEvents[i];
         if (e.machineTo.id == -1) {
            // broadcast event
            p->broadcast(new TimedEvent(e.countdown, e.name, -1, e.eventParams), -1);
         } else {
            // target machine event
            p->sendTo(new TimedEvent(e.countdown, e.name, -1, e.eventParams), e.machineTo);
         }
      }
   }

   // initialize timer, etc, and also, setup first state (if not given)
   virtual MultiState<Param>* initialize(MultiState<Param>* current, MultiContext<Param>* p) override
   {
      // check if there's initial state available
      if (!current)
         current = new MultiState<Param>(machines.size(), nullptr);

      std::cout << std::endl;
      std::cout << "===========" << std::endl;
      std::cout << "begin run()" << std::endl;
      std::cout << "===========" << std::endl;

      std::cout << "initializing multimachine" << std::endl;
      if (watchdog) {
         watchdog->reset();
      } else {
         std::cout << "No watchdog configured!" << std::endl;
      }
      for (unsigned i = 0; i < machines.size(); i++) {
         machines[i]->initialize(current->at(i), p);
      }

      launchScheduledEvents(p);

      return current;
   }

   // launch when machine is finished
   virtual void OnFinished(const MultiState<Param>& states, MultiContext<Param>* p) override
   {
      std::cout << std::endl;
      std::cout << "=================" << std::endl;
      std::cout << "finished machine!" << std::endl;
      std::cout << "=================" << std::endl;
   }

   virtual bool isFinal(const MultiState<Param>& states, MultiContext<Param>* p) override
   {
      for (unsigned i = 0; i < states.size(); i++) {
         if (!states[i] || !states[i]->isFinal)
            return false;
      }
      return true;
   }

   /*
   // perhaps just processGlobalTransitions here (both scheduled and non-scheduled)
   bool processScheduledGlobalTransitions(MultiState<Param>& states, MultiContext<Param>* p)
   {
      bool r = false;
      for (unsigned i = 0; i < scheduledTransitions.size(); i++) {
         if (scheduledTransitions[i].timer->expired()) // expired timer
         {
            int m = scheduledTransitions[i].machine; // get target machine
            if (scheduledTransitions[i].thing->isValid(*(machines[m]->timer), p, m)) {
               // isValid() transition
               cout << "processing scheduled transition on Machine " << m << ": " << scheduledTransitions[i].thing->toString() << endl;
               State<MultiContext<Param>>* next = scheduledTransitions[i].thing->execute(*(machines[m]->timer), p, m);
               cout << "updating state on machine " << m << " to " << next->toString() << endl;
               states[m] = next;
               // removing processed transition (or keep it?)
               scheduledTransitions.erase(scheduledTransitions.begin() + i);
               i--; // bad practice... use iterators on loop, instead
               r = true;
            }
         }
      }
      return r;
   }


   bool processScheduledEvents(MultiContext<Param>* p)
   {
      bool r = false;
      for (unsigned i = 0; i < scheduledEvents.size(); i++) {
         if (scheduledEvents[i].timer->expired()) // expired timer
         {
            int to = scheduledEvents[i].machine;           // get target machine
            if (to == -1)                                  // broadcast
               p->broadcast(scheduledEvents[i].thing, -1); // from -1 (from system)
            else
               p->sendTo(scheduledEvents[i].thing, to);
            // removing processed event
            scheduledEvents.erase(scheduledEvents.begin() + i);
            i--; // bad practice... use iterators on loop, instead
            r = true;
         }
      }
      return r;
   }
*/

   virtual bool updateState(MultiState<Param>*& states, MultiContext<Param>* p) override
   {
      bool ret = false;
      for (unsigned i = 0; i < machines.size(); i++) {
         // evaluate situation on each machine
         bool r = machines[i]->updateState(states->at(i), p);
         if (r) {
            std::cout << "machine " << i << " moved to state: " << states->at(i)->toString() << std::endl;
            //states->at(i)->onEnter(p); // really useful?
            ret = true;
         }
      }

      return ret;
   }

   virtual void onEnterState(MultiState<Param>& current, MultiContext<Param>* p) override
   {
      std::cout << "updating multi state! STATES:" << std::endl;
      for (unsigned i = 0; i < current.size(); i++) {
         std::cout << "Machine " << i << " => " << current[i]->toString() << std::endl;
      }

      if (watchdog)
         watchdog->reset();
   }

   virtual bool beforeUpdateState(MultiState<Param>& states, MultiContext<Param>* p) override
   {
      // check watchdog
      if (watchdog && watchdog->expired()) {
         std::cout << "StateMachine FAILED: MAXTIME = " << watchdog->getCountdown() << std::endl;
         return true;
      }
      /*
      // process events
      bool re = processScheduledEvents(p);
      if (re) {
         cout << "SOME EVENT HAPPENED!" << endl;
         //watchdog.reset(); // TODO: make watchdog part of this specific class
      }

      // look for a scheduled global transition (or event)
      bool b = processScheduledGlobalTransitions(states, p);
      if (b) {
         cout << "SOME GLOBAL TRANSITION HAPPENED!" << endl;
         //watchdog.reset(); // TODO: make watchdog part of this specific class
      }
*/
      return false;
   }

   virtual std::string toString(std::string format = "") override
   {
      std::stringstream ss;
      if (format == "graphviz") {

      } else {
         // standard text

         ss << "ReplicatedSTSM [";
         for (unsigned i = 0; i < machines.size(); i++)
            ss << machines[i]->toString() << ";";
         ss << "]";
      }
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_REPLICATED_STSM_HPP
