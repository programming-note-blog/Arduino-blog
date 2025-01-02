#ifndef CYCLE_H
#define CYCLE_H

#define PERIODIC_INTERVAL 10 // ms

#include "state.h"

struct SCycleFunc {
  void (*func)();
  unsigned long timeAfterStart;
  bool shouldCall;
};

void CycleSetFunc(void (*func)(), EState state, unsigned long timeAfterStart);
static void CycleReset();
void CycleCall();
void CycleSetState(EState state);

#endif // CYCLE_H
