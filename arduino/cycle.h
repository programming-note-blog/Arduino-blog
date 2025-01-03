#ifndef CYCLE_H
#define CYCLE_H

#define PERIODIC_INTERVAL 20 // ms

#include "state.h"

struct SCycleFunc {
  void (*func)();
  unsigned long timeAfterStart;
  bool shouldCall;
};

void CycleSetup(void (*func)(), EState state, unsigned long timeAfterStart);
static void CycleReset();
void CycleLoop();
void CycleSetState(EState state);

#endif // CYCLE_H
