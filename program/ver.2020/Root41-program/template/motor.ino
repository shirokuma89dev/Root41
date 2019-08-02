// motor.ino

_motor::_motor(void) {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void _motor::drive(int _deg, int _power) {
  //初期化
  val[0] = 0;
  val[1] = 0;
  val[2] = 0;

  //角度取得
  gyro.deg = gyro.read();

  //姿勢制御
  float Kp;
  float Ki;
  float Kd;

  Kp = 0.747;
  Ki = 0.008;
  Kd = 0.1;

  front = gyro.deg;
  front = front + 360;
  front = front >= 360 ? front - 360 : front;

  front = front > 180 ? front - 360 : front;

  integral += front;
  front *= Kp;             //比例制御
  front += integral * Ki;  //積分制御
  front = front - (gyro.differentialRead() * Kd);  //微分制御

  // if (integralTimer + 3000 <= millis()) {
  //   integralTimer = millis();
  //   integral = 0;
  // }

  if (gyro.deg <= 5 && gyro.deg >= 355) {
    integral = 0;
  }

  correctionVal = round(front);
  correctionVal = constrain(correctionVal, -75, 75);

  Serial.println(front);

  float s;
  if (_deg == 0) {
    val[0] = 97;
    val[1] = -100;
    val[2] = 0;
  } else if (_deg == 23 || _deg == 22) {
    val[0] = 100;
    val[1] = -36;
    val[2] = -23;
  } else if (_deg == 45) {
    val[0] = 100;
    val[1] = -15;
    val[2] = -60;
  } else if (_deg == 68 || _deg == 67) {
    val[0] = 100;
    val[1] = 13;
    val[2] = -90;
  } else if (_deg == 90) {
    val[0] = 40;  // 34
    val[1] = 40;
    val[2] = -100;
  } else if (_deg == 113 || _deg == 112) {
    val[0] = 13;
    val[1] = 100;
    val[2] = -90;
  } else if (_deg == 135) {
    val[0] = -20;
    val[1] = 100;
    val[2] = -48;
  } else if (_deg == 158 || _deg == 157) {
    val[0] = -34;
    val[1] = 100;
    val[2] = -27;
  } else if (_deg == 180) {
    val[0] = -90;
    val[1] = 100;
    val[2] = 0;
  } else if (_deg == 203 || _deg == 202) {
    val[0] = -100;
    val[1] = 34;
    val[2] = 27;
  } else if (_deg == 225) {
    val[0] = -100;
    val[1] = 20;
    val[2] = 48;
  } else if (_deg == 248 || _deg == 247) {
    val[0] = -100;
    val[1] = -13;
    val[2] = 90;
  } else if (_deg == 270) {
    val[0] = -40;  //-34
    val[1] = -40;  //-34
    val[2] = 100;
  } else if (_deg == 293 || _deg == 292) {
    val[0] = -13;
    val[1] = -100;
    val[2] = 90;
  } else if (_deg == 315) {
    val[0] = 15;
    val[1] = -100;
    val[2] = 60;
  } else if (_deg == 338 || _deg == 337) {
    val[0] = 34;
    val[1] = -100;
    val[2] = 23;
  } else {
    val[0] = int(sin(radians(_deg - 300)) * 100.0);
    val[1] = int(sin(radians(_deg - 60)) * 100.0);
    val[2] = int(sin(radians(_deg - 180)) * 100.0);

    if (abs(val[0]) < abs(val[1])) {
      if (abs(val[1]) < abs(val[2])) {
        s = 100.0 / (float)abs(val[2]);
      } else {
        s = 100.0 / (float)abs(val[1]);
      }
    } else {
      if (abs(val[0]) < abs(val[2])) {
        s = 100.0 / (float)abs(val[2]);
      } else {
        s = 100.0 / (float)abs(val[0]);
      }
    }
    val[0] = round((float)val[0] * s);
    val[1] = round((float)val[1] * s);
    val[2] = round((float)val[2] * s);
  }

  for (int i = 0; i <= 2; i++) {
    val[i] += correctionVal;
  }

  if (abs(val[0]) < abs(val[1])) {
    if (abs(val[1]) < abs(val[2])) {
      s = 100.0 / (float)abs(val[2]);
    } else {
      s = 100.0 / (float)abs(val[1]);
    }
  } else {
    if (abs(val[0]) < abs(val[2])) {
      s = 100.0 / (float)abs(val[2]);
    } else {
      s = 100.0 / (float)abs(val[0]);
    }
  }
  val[0] = round((float)val[0] * s);
  val[1] = round((float)val[1] * s);
  val[2] = round((float)val[2] * s);

  for (int i = 0; i <= 2; i++) {
    val[i] = map(val[i], -100, 100, -_power, _power);
    val[i] = constrain(val[i], -100, 100);
  }

  if (gyro.deg >= 60 && gyro.deg <= 300) {
    for (int i = 0; i <= 2; i++) {
      if (correctionVal <= 30 && correctionVal >= -30) {
        if (correctionVal >= 0) {
          val[i] = 35;
        } else {
          val[i] = -35;
        }
      } else {
        val[i] = correctionVal;
      }
    }
  }
  gyro.deg = gyro.read();
  if (_deg == NULL && _power == NULL) {
    for (int i = 0; i <= 2; i++) {
      if (gyro.deg <= 5 || gyro.deg >= 355) {
        val[i] = 0;
      } else {
        if (abs(correctionVal) <= 15) {
          if (correctionVal >= 0) {
            val[i] = 15;
          } else {
            val[i] = -15;
          }
        } else {
          val[i] = correctionVal;
        }
      }
    }
  }

  directDrive(val);
}

void _motor::directDrive(int* p) {
  if (*p == 0) {
    PORTG |= _BV(5);
    PORTE |= _BV(3);

    digitalWrite(6, HIGH);
  } else if (*p >= 1) {
    PORTG |= _BV(5);
    PORTE &= ~(_BV(3));

    if (*p >= 100) {
      digitalWrite(6, HIGH);
    } else {
      analogWrite(6, constrain(abs(round((float)*p * 2.55)), 0, 255));
    }
  } else if (*p <= -1) {
    PORTG &= ~(_BV(5));
    PORTE |= _BV(3);

    if (*p <= -100) {
      digitalWrite(6, HIGH);
    } else {
      analogWrite(6, constrain(abs(round((float)*p * 2.55)), 0, 255));
    }
  }

  if (*(p + 1) == 0) {
    PORTH |= _BV(4) | _BV(5);

    digitalWrite(9, HIGH);
  } else if (*(p + 1) >= 1) {
    PORTH |= _BV(4);
    PORTH &= ~(_BV(5));

    if (*(p + 1) >= 100) {
      digitalWrite(9, HIGH);
    } else {
      analogWrite(9, constrain(abs(round((float)*(p + 1) * 2.55)), 0, 255));
    }
  } else if (*(p + 1) <= -1) {
    PORTH &= ~(_BV(4));
    PORTH |= _BV(5);

    if (*(p + 1) <= -100) {
      digitalWrite(9, HIGH);
    } else {
      analogWrite(9, constrain(abs(round((float)*(p + 1) * 2.55)), 0, 255));
    }
  }

  if (*(p + 2) == 0) {
    PORTB |= _BV(4) | _BV(5);

    digitalWrite(12, HIGH);
  } else if (*(p + 2) >= 1) {
    PORTB |= _BV(4);
    PORTB &= ~(_BV(5));

    if (*(p + 2) >= 100) {
      digitalWrite(12, HIGH);
    } else {
      analogWrite(12, constrain(abs(round((float)*(p + 2) * 2.55)), 0, 255));
    }
  } else if (*(p + 2) <= -1) {
    PORTB &= ~(_BV(4));
    PORTB |= _BV(5);

    if (*(p + 2) <= -100) {
      digitalWrite(12, HIGH);
    } else {
      analogWrite(12, constrain(abs(round((float)*(p + 2) * 2.55)), 0, 255));
    }
  }
}