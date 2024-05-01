#include <Arduino.h>

#define NUM_LOOPS 100  // 100周分を計測する

int numLoops = NUM_LOOPS;
unsigned long loopTime[NUM_LOOPS];

void setup()
{
  // 初期化処理
  Serial.begin(9600);
}

void loop()
{
  loopTime[numLoops] = micros();

  if(numLoops > 0)
  {
    numLoops--; // 残りのループ回数を減らす
  }
  else
  {
    // 100周期終了後に合計実行時間を出力
    for(int i = NUM_LOOPS; --i >= 0;)
    {
      Serial.println(loopTime[i]);
    }

    while(true)
    {
      // 無限ループでプログラムを停止
    }
  }
}