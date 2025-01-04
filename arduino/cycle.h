#ifndef CYCLE_H
#define CYCLE_H

#define PERIODIC_INTERVAL 20 // ms

#include "state.h"

void CycleSetup(void (*func)(), EState state, unsigned long timeAfterStart);
void CycleLoop();
void CycleSetState(EState state);

#endif // CYCLE_H
