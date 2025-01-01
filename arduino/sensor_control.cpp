/**
 * @file sensor_control.cpp
 * @brief フォトリフレクタアレイ制御モジュール
 */

#include "sensor_control.h"
#include "pin_control.h"
#include "led_control.h"

#define NUM_SENSORS 8 // センサの数
#define CALIBRATION_SAMPLES 8 // キャリブレーションのサンプル数

static unsigned short sensorPins[NUM_SENSORS] = {PIN_SENSOR_0, PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4, PIN_SENSOR_5, PIN_SENSOR_6, PIN_SENSOR_7};
static unsigned short whiteCalibrated[NUM_SENSORS]; // 白色キャリブレーション値
static unsigned short blackCalibrated[NUM_SENSORS]; // 黒色キャリブレーション値
static unsigned short thresholds[NUM_SENSORS]; // 検出用のしきい値

/**
 * @brief センサモジュールを初期化します。
 * @return 0 成功
 */
unsigned short SensorControlInit(unsigned short pin) {
    for (unsigned short i = 0; i < NUM_SENSORS; i++) {
        whiteCalibrated[i] = 0;
        blackCalibrated[i] = 1023;
        thresholds[i] = 512; // デフォルトの中間値
    }
    return LEDOn(pin);
}

/**
 * @brief 白色表面のキャリブレーションを実行します。
 * @return 0 成功
 */
unsigned short SensorControlCalibrateWhite() {
    unsigned short sampleSums[NUM_SENSORS] = {0};
    for (unsigned short sample = 0; sample < CALIBRATION_SAMPLES; sample++) {
        for (unsigned short i = 0; i < NUM_SENSORS; i++) {
            sampleSums[i] += PinControlAnalogRead(sensorPins[i]);
        }
    }
    for (unsigned short i = 0; i < NUM_SENSORS; i++) {
        whiteCalibrated[i] = sampleSums[i] >> 3; // 平均値をビットシフトで計算
    }
    return 0; // 成功
}

/**
 * @brief 黒色表面のキャリブレーションを実行します。
 * @return 0 成功
 */
unsigned short SensorControlCalibrateBlack() {
    unsigned short sampleSums[NUM_SENSORS] = {0};
    for (unsigned short sample = 0; sample < CALIBRATION_SAMPLES; sample++) {
        for (unsigned short i = 0; i < NUM_SENSORS; i++) {
            sampleSums[i] += PinControlAnalogRead(sensorPins[i]);
        }
    }
    for (unsigned short i = 0; i < NUM_SENSORS; i++) {
        blackCalibrated[i] = sampleSums[i] >> 3; // 平均値をビットシフトで計算
    }
    return 0; // 成功
}

/**
 * @brief しきい値を更新します。
 * @return 0 成功
 */
unsigned short SensorControlUpdateThresholds() {
    for (unsigned short i = 0; i < NUM_SENSORS; i++) {
        thresholds[i] = (whiteCalibrated[i] + blackCalibrated[i]) / 2;
    }
    return 0; // 成功
}

/**
 * @brief 8ビットのバイナリ出力を取得します。
 * @return unsigned char 黒色検出のバイナリ値
 */
unsigned char SensorControlGetBinaryOutput() {
    unsigned char binaryOutput = 0;
    for (unsigned short i = 0; i < NUM_SENSORS; i++) {
        unsigned short value = PinControlAnalogRead(sensorPins[i]);
        if (value < thresholds[i]) {
            binaryOutput |= (1 << i);
        }
    }
    return binaryOutput;
}
