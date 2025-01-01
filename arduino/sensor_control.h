/**
 * @file sensor_control.h
 * @brief フォトリフレクタアレイ制御モジュールのヘッダファイル
 */

#ifndef SENSOR_CONTROL_H
#define SENSOR_CONTROL_H

/**
 * @brief センサモジュールを初期化します。
 * @return 0 成功
 */
unsigned short SensorControlInit(unsigned short pin);

/**
 * @brief 白色表面のキャリブレーションを実行します。
 * @return 0 成功
 */
unsigned short SensorControlCalibrateWhite();

/**
 * @brief 黒色表面のキャリブレーションを実行します。
 * @return 0 成功
 */
unsigned short SensorControlCalibrateBlack();

/**
 * @brief しきい値を更新します。
 * @return 0 成功
 */
unsigned short SensorControlUpdateThresholds();

/**
 * @brief 8ビットのバイナリ出力を取得します。
 * @return unsigned char 黒色検出のバイナリ値
 */
unsigned char SensorControlGetBinaryOutput();

#endif // SENSOR_CONTROL_H
