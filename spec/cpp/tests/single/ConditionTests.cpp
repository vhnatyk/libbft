#include <memory>

#include <gtest/gtest.h>

#include "single/Transition.hpp"
#include "timing/Clock.hpp"
#include "timing/Timer.hpp"

using namespace std;
using namespace libbft;

TEST(SingleCondition, ToString) {
   Condition<int>::TimedFunctionType f = [](const Timer &t, std::shared_ptr<int> p, const MachineId &) -> bool {
      return *p % 2 == 0;
   };
   unique_ptr<Condition<int>> condition(new Condition<int>("T", f));

   EXPECT_EQ("T", condition->toString());
}

TEST(SingleCondition, UseTimedFunction) {
   Condition<int>::TimedFunctionType f = [](const Timer &t, std::shared_ptr<int> p, const MachineId &) -> bool {
      return *p % 2 == 0;
   };
   unique_ptr<Condition<int>> condition(new Condition<int>("T", f));
   auto timer = std::shared_ptr<Timer>(new Timer("T", std::unique_ptr<Clock>(new Clock())));
   auto p = std::shared_ptr<int>(new int);
   *p = 1;

   const MachineId &id = MachineId(-1);
   auto rfv = f(*timer, p, id);
   auto rcv = condition->timedFunction(*timer, p, id);
   EXPECT_EQ(rfv, rcv);

   for (auto i = std::shared_ptr<int>(new int{0}); *i < 10; ++(*i)) {
      rfv = f(*timer, i, id);
      rcv = condition->timedFunction(*timer, i, id);
      EXPECT_EQ(rfv, rcv);
   }
}
