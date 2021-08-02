#include <M5Core2.h>
#include <AXP192.h>

AXP192 M5Power;

uint16_t color(uint8_t red, uint8_t green, uint8_t blue) {
  return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

//サーボ制御に使う定数の設定
#define SERVO_PIN    32  //ServoPWMピン
#define RILAY_PIN      33  //リレー制御ピン
#define SERVO_CH_0     0   //チャンネル指定
#define LEDC_TIMER_BIT 10 //LEDCのPWMタイマーの精度設定 10bit=分解能1024
#define LEDC_SERVO_FREQ 50 //サーボ信号の１サイクル　50Hz:20ms


// (26/1024)*20ms ≒ 0.5 ms  (-90°)
int minimum = 28;
// (123/1024)*20ms ≒ 2.4 ms (+90°)
int maximum = 115;
int deg = 0;


//変数の宣言
float ricevol= 0.2 ;
float watervol = 200;
int PumpTime = 3000;
int ServoTimes = 1 ;


void servo(int degree) {
  ledcWrite(SERVO_CH_0, map(degree, -90, 90, minimum, maximum));
//  delay(200);
//  ledcWrite(SERVO_CH_0, 0);
}

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  pinMode(RILAY_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  digitalWrite(RILAY_PIN, HIGH);
  
  M5.Lcd.fillScreen(WHITE);
  //PWMに使う関数のセットアップ
  dacWrite(25, 0); //　ノイズ対策
  ledcSetup(SERVO_CH_0, LEDC_SERVO_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(SERVO_PIN, SERVO_CH_0);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 215);
  M5.Lcd.setTextColor(BLUE, WHITE);
  M5.Lcd.print("   UP     RESET     START");

}

void loop() {
  // put your main code here, to run repeatedly:    
  M5.update();

  ServoTimes = ricevol*5 ;
  watervol = ricevol*200 ;
  PumpTime = watervol*460+4700 ; 
  //この数値を調整して1合当たりの水の量を決める。
  //何回かの測定結果より　Y=460x+4700
  
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 0);
  M5.Lcd.println("kome-bits");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 35);
  M5.Lcd.print("[ ");
  M5.Lcd.print(ricevol);
  M5.Lcd.print(" ]gou");
  M5.Lcd.setCursor(5, 70);
  M5.Lcd.print("[ ");
  M5.Lcd.print(ServoTimes);
  M5.Lcd.print(" ] times measurement");
  M5.Lcd.setCursor(5, 105);
  M5.Lcd.print("[ ");
  M5.Lcd.print(watervol);
  M5.Lcd.print(" ] ml water");
  M5.Lcd.setCursor(5, 140);
  M5.Lcd.print("[ ");
  M5.Lcd.print(PumpTime);
  M5.Lcd.print(" ] ms pump work");


if (M5.Touch.ispressed()) {
    TouchPoint_t pos = M5.Touch.getPressPoint();
    if (pos.y > 240) {
      if (pos.x < 120) { //btnA
        while (M5.Touch.ispressed()) {}
      //Aボタンが押されたときの処理
      M5.Lcd.setCursor(5, 170);
      M5.Lcd.setTextColor(WHITE, RED);
      M5.Lcd.print("btnA pressed!            ");
      servo(90);
      ricevol=ricevol+0.2 ;
      }
      
      else if (pos.x > 240) { //btnC
      //Cボタンが押されたときの処理
      for (int i=1; i<=ServoTimes; i++){
      M5.Lcd.setCursor(5, 170);
      M5.Lcd.setTextColor(WHITE, RED);
      M5.Lcd.print("btnC ");
      M5.Lcd.print(i);
      M5.Lcd.print(" times measured.   ");
      servo(-90);
      delay(1000);
      servo(90);
      delay(1000);      
      }
      digitalWrite(RILAY_PIN, LOW);
      delay(PumpTime);
      digitalWrite(RILAY_PIN, HIGH);

      M5.Lcd.setCursor(5, 170);
      M5.Lcd.setTextColor(WHITE, RED);
//      M5.Lcd.print("1234567890123456789012345"); これで一行：25文字（半角）
      M5.Lcd.print("Procedure finished!!      ");
      
      
      while (M5.Touch.ispressed()) {}
      }
      else if (pos.x >= 150 && pos.x <= 200) { //btn
      //Bボタンが押されたときの処理
      ricevol= 0.2 ;
      servo(90);
      M5.Lcd.setCursor(5, 170);
      M5.Lcd.setTextColor(WHITE, RED);
      M5.Lcd.print("btnB parameter is reseted");

      }
    }
  }
}
