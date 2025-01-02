#include <Arduino.h>

#include <stdio.h>
#include <stdbool.h>

#include "state.h"

// 現在の状態の保持
static EState currentState = STATE_STANDBY;

// 各種機能のON/OFFを管理する変数
static bool moveFunctionOn = false;
static bool endoscopeLockOn = false;
static bool alertOn = false;
static bool forgetAlertOn = false;

// イベントのプロトタイプ宣言
static bool isEndoscopeChecked();

// 現在の状態を取得する関数
EState StateGetCurrentstate(void) {
    return currentState;
}

void StateOnButtonPress() {
    switch(currentState) {
        case STATE_STANDBY:
            currentState = STATE_LINETRACING;
            Serial.println("Standby -> LineTracing\n");
            moveFunctionOn = true;
            endoscopeLockOn = true;
            alertOn = true;
            break;
        case STATE_LINETRACING:
            currentState = STATE_STANDBY;
            Serial.println("LineTracing -> Standby\n");
            moveFunctionOn = false;
            endoscopeLockOn = false;
            alertOn = false;
            break;
        case STATE_STOPPED:
            if(isEndoscopeChecked()) {
                currentState = STATE_STANDBY;
                Serial.println("Stopped -> Standby\n");
                forgetAlertOn = false;
            } else {
              Serial.print("[error]");
              Serial.println(currentState);
            }
            break;
    }
}

void StateOnArrive(void) {
    if(currentState == STATE_LINETRACING) {
        currentState = STATE_STOPPED;
        Serial.println("LineTracing -> Stopped\n");
        moveFunctionOn = false;
        endoscopeLockOn = false;
        alertOn = false;
        forgetAlertOn = true;
    }
	else
	{
        Serial.print("[error]");
        Serial.println(currentState);
	}
}

static bool isEndoscopeChecked() {
    // この関数は内視鏡が確認されたかどうかの実装を行う。
    // ここでは常に確認されたものと仮定。
    return true;
}