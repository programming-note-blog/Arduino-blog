#include <Arduino.h>

#include "cycle.h"
#include "state.h"

#define MAX_FUNC_COUNT 5

struct SStateCycle {
  SCycleFunc funcList[MAX_FUNC_COUNT];
  int funcCount;
};

static unsigned long gs_current_time;
static unsigned long gs_last_time = 0;
static EState current_state;
static SStateCycle stateCycles[STATE_COUNT] = {0};

void CycleSetFunc(void (*func)(), EState state, unsigned long timeAfterStart) {
  if (stateCycles[state].funcCount < MAX_FUNC_COUNT) {
    int index = stateCycles[state].funcCount;
    stateCycles[state].funcList[index].func = func;
    stateCycles[state].funcList[index].timeAfterStart = timeAfterStart;
    stateCycles[state].funcList[index].shouldCall = false; 
    stateCycles[state].funcCount++;
  }
  else {
    Serial.print("[error]:over MAX_FUNC_COUNT:");
    Serial.println(state);
  }
}

static void CycleReset() {
  for (int i = 0; i < stateCycles[current_state].funcCount; i++) {
    if (stateCycles[current_state].funcList[i].shouldCall) {
      Serial.print("[error]not done:");
      Serial.println(i);
      gs_current_time = millis(); // シリアル出力の時間を無視
    }
    stateCycles[current_state].funcList[i].shouldCall = true;
  }
  gs_last_time = gs_current_time;
}

void CycleCall() {
  gs_current_time = millis();

  if (gs_current_time - gs_last_time >= PERIODIC_INTERVAL) {
    CycleReset();
  }

  for (int i = 0; i < stateCycles[current_state].funcCount; i++) {
    if (stateCycles[current_state].funcList[i].shouldCall &&
        (gs_current_time - gs_last_time >= stateCycles[current_state].funcList[i].timeAfterStart)) {
      stateCycles[current_state].funcList[i].func();
      stateCycles[current_state].funcList[i].shouldCall = false;
    }
  }
}

void CycleSetState(EState state) {
    current_state = state;
}
