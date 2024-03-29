//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <FaBoLCDmini_AQM0802A.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Timer5.h>

//ピン番号定義
int BALL[16] = {A0, A1, A2,  A3,  A4,  A5,  A6,  A7,
                A8, A9, A10, A11, A12, A13, A14, A15};
#define BALL_RESET 26

int LINE[20] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                40, 41, 42, 43, 44, 45, 46, 47, 48, 49};

#define SW_2 22
#define SW_1 23
#define SW_RESET 25

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;

class _ball {
 public:
  int val[16];

 private:
} ball;

class _line {
 public:
  bool val[20];

 private:
} line;

class _motor {
 public:
  _motor(void);

  void directDrive(int* p);
  void drive(int _deg, int _power);

  int val[3];

 private:
  int front = 0;
  int _front;
  float Kp;
  float Ki;
  float Kd;

  int correctionVal = 0;
  int integral = 0;
} motor;

class _gyro {
 public:
  int read(void);
  int differentialRead(void);
  void setting(void);

  int deg;
  int differentialDeg = 0;
  int eeprom[6];
  int offset;

 private:
  // none
} gyro;

class _device {
 public:
  void initialize(void);
  void check(void);

  bool robot;  // trueが宮里

  int process = LOW;
  int mode = 0;

 private:
} device;

class _LED {
 public:
  void gyroShow(void);
  void changeAll(int red, int green, int blue);

  bool white = false;

  int bright = 100;
  int i, j;

  unsigned long timer;

 private:
  // none
} LED;

void setup(void) {
  device.initialize();
  device.mode = 0;

  Serial.begin(115200);

  Wire.begin();
  gyro.setting();

  //起動イルミネーション
  for (int i = 0; i <= 15; i++) {
    RGBLED.begin();
    RGBLED.setBrightness(LED.bright);

    LED.changeAll(0, 0, 0);

    for (int k = 0; k <= i; k++) {
      RGBLED.setPixelColor(k, 255, 255, 255);
    }

    RGBLED.show();

    delay(15);
  }

  delay(500);
}

void loop(void) {
  device.check();

  if (device.mode == 0) {
    gyro.deg = gyro.read();

    LED.gyroShow();

    Serial.println(gyro.deg);
  } else if (device.mode == 1) {
    LED.changeAll(0, 0, 255);
    motor.drive(0, 100);
  }
}
