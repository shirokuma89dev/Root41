// 2019.02.27作成 template.ino
//チームでプログラム開発するときに、構造が同じほうが
//プログラムが見やすくなるから、このテンプレートをベースに
//開発しよう。コーディングはできるだけ、
//見やすくなるように努力しよう。

//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <FaBoLCDmini_AQM0802A.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Root41_Lib.h>
#include <Rotary.h>
#include <Timer5.h>
#include <Wire.h>

//ジャイロセンサのオフセット値
#define Gyro_X 87
#define Gyro_Y -66
#define Gyro_Z 70
#define Accel_X -4775
#define Accel_Y -2060
#define Accel_Z 1456

//ピン番号定義
#define BALL0 1
#define BALL1 0
#define BALL2 3
#define BALL3 2
#define BALL4 5
#define BALL5 4
#define BALL6 7
#define BALL7 6
#define BALL8 9
#define BALL9 8
#define BALL10 11
#define BALL11 10
#define BALL12 13
#define BALL13 12
#define BALL14 15
#define BALL15 14
#define BALL_RESET 22

#define LINE1 24
#define LINE2 25
#define LINE3 26
#define LINE4 27

#define SW_TOGGLE 28
#define SW_RIGHT 29
#define SW_LEFT 30

#define ROTARY1 31
#define ROTARY2 32

#define RGBLED_OUTPIN 23
#define NUMRGBLED 16

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 23, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;
Rotary rotary = Rotary(ROTARY1, ROTARY2);
Root41_Lib Root41;

class _ball {
 public:
  void read(int* b);
  bool exist = true;

  int val[16];
  int top;
  int deg;
  int old_top;
  int top_backup;

  float x;
  float y;

  unsigned long resetTimer = 0;
  unsigned long resettingTimer = 0;

 private:
  // none
} ball;

class _line {
 public:
  _line(void);
  void read(void);

  bool flug = false;
  bool near = false;
  bool touch = false;
  bool val[4];

  int deg = 1000;
  int outMove = 1000;
  int mode = 0;
  int offset = 0;
  int first = 5;
  int second = 5;

  unsigned long inTimer;
  unsigned long outTimer;

 private:
  // none
} line;

class _motor {
 public:
  _motor(void);
  void directDrive(int* p);
  void drive(int _deg,
             int _power,
             bool stop = false,
             bool correctionDeg = false);

  bool correction = true;

  int val[3];
  int correctionVal;
  int deg;
  int power;
  int move = 15;
  int integral = 0;

  float front;

  unsigned long integralTimer = 0;

 private:
  // none
} motor;

class _gyro {
 public:
  int read(void);
  int differentialRead(void);
  void setting(void);

  int deg;
  int differentialDeg = 0;
  int offset;

 private:
  // none
} gyro;

class _LCD {
 public:
  void init(void);

  byte character1[8];
  byte character2[8];
  byte character3[8];
  byte character4[8];
  byte character5[8];
  byte character6[8];
  byte character7[8];
  byte character8[8];

  int output = 0;

  unsigned long timer = 0;

 private:
  // none
} LCD;

class _device {
 public:
  void initialize(void);
  void monitor(void);

  bool boot = false;

  int process = HIGH;
  int mode = 0;
  int rotary = 0;
  int rotaryResult = 0;

 private:
  // none
} device;

class _LED {
 public:
  int bright = 60;
  int i, j;

  unsigned long timer;

 private:
  // none
} LED;

class _kickoff {
 public:
  int deg = 0;
  int go = 0;

  long ledTimer;
  long moveTime;
  bool ledShow = false;

 private:
  // none
} kickoff;

void setup(void) {}

void loop(void) {}
