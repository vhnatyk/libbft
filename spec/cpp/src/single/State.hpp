#pragma once
#ifndef LIBBFT_SRC_CPP_STATE_HPP
#define LIBBFT_SRC_CPP_STATE_HPP

// system includes
#include <memory>
#include <cstddef>
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <random>

// standard Transition
#include "timing/Timer.hpp"
#include "Transition.hpp"

// standard State

// every state is a Timed state (states that allow timed transitions)

namespace libbft {

template<class Param = std::nullptr_t>
class State
{
public:
   using TParam = std::shared_ptr<Param>;
   using TTransition = std::shared_ptr<Transition<Param>>;

   /** should only access for get string, etc (on graphviz)... TODO: design better protection here */
   std::vector<TTransition> transitions;
public:
   std::string name;
   bool isFinal;

   explicit State(bool _isFinal = false, std::string _name = "")
     : name(std::move(_name))
     , isFinal(_isFinal)
   {
   }

   void addTransition(TTransition t)
   {
      transitions.push_back(t);
   }

   TTransition tryGetTransition(Timer& timer, TParam p, MachineId me)
   {
      // cout << "Trying to Get Transition" << endl;
      // should be non-deterministic and asynchronous...
      // TODO: simulate this with random, at least, to avoid getting stuck on tests by chance
      std::vector<TTransition> _transitions = transitions;

      auto rng = std::default_random_engine{};
      std::shuffle(std::begin(_transitions), std::end(_transitions), rng);

      for (unsigned i = 0; i < _transitions.size(); i++) {
         if (_transitions[i]->isValid(timer, p, me))
            return _transitions[i];
      }
      return nullptr;
   }

   std::string toString(bool recursive = true) const
   {
      std::stringstream ss;
      ss << "state:{";
      ss << "name='" << name << "';";
      if (isFinal)
         ss << "FINAL";
      ss << ";";
      if (recursive) {
         ss << "transitions=[";
         for (unsigned i = 0; i < transitions.size(); i++)
            ss << transitions[i]->toString() << ";";
         ss << "]";
      } else
         ss << "...";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_STATE_HPP
