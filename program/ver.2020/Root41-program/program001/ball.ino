void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    if (*(b + i) <= 120) {
      *(b + i) = 1000;
    }
  }

  if (device.robot) {
    val[7] = (val[6] + val[9] + val[6]) / 3.3;
    val[8] = (val[6] + val[9] + val[9]) / 3.3;
    val[15] *= 0.95;
    val[13] *= 0.95;
  } else {
    val[7] = (val[6] + val[9] + val[6]) / 3.3;
    val[8] = (val[6] + val[9] + val[9]) / 3.3;
    val[9] /= 1.1;
    val[0] *= 0.98;
    val[1] *= 0.93;
    val[15] *= 0.93;
    // val[8] *= 100;
  }

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  speed = 100;
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  if (top >= 3 && top <= 5) {
    left = -1;
  } else if (top >= 11 && top <= 13) {
    left = 1;
  }

  deg = top * 22.5;
  _deg = deg;

  turn = false;
  if (top > 1 && top < 15 && millis() - holdTimer >= 200) {
    if ((val[top] <= 150) || min(val[7], min(val[8], val[9])) <= 230 ||
        min(val[6], val[10]) <= 240) {
      if (deg >= 180) {
        deg -= 85;
      } else {
        deg += 85;
      }
      dist = 4;
    } else if ((val[top] <= 165) || min(val[7], min(val[8], val[9])) <= 250 ||
               min(val[6], val[10]) <= 260) {
      if (deg >= 180) {
        deg -= 65;
      } else {
        deg += 65;
      }
      dist = 4;
    } else if ((val[top] <= 220)) {
      if (deg >= 180) {
        deg -= 55;
      } else {
        deg += 55;
      }
      dist = 3;
    } else if (val[top] <= 280) {
      if (deg >= 180) {
        deg -= 45;
      } else {
        deg += 45;
      }
      dist = 2;
    } else if (val[top] <= 320) {
      if (deg >= 180) {
        deg -= 30;
      } else {
        deg += 30;
      }
      dist = 1;
    } else {
      dist = 0;
    }

    if (dist > 1 && (top < 6 || top > 10)) {
      turn = true;
      degLPF = 0.25;
    } else {
      degLPF = 0.7;
    }
  }

  // emg = false;
  // if (top > 5 && top < 11 && millis() - holdTimer >= 200) {
  //   if (val[top] < 210) {
  //     emg = true;
  //     if (top >= 8) {
  //       deg -= 45;
  //     } else {
  //       deg += 45;
  //     }
  //   }
  // }

  if ((top <= 3 || top >= 13) && digitalRead(BALL_HOLD) && val[top] <= 260) {
    if (tof.dist >= 600) {
      if (left == 1) {
        deg = 30;
      } else if (left == -1) {
        deg = 330;
      } else {
        deg = 0;
      }
    }
    hold = true;
    holdTimer = millis();
  } else {
    hold = false;
  }

  if (top == 0) {
    holdTimer = millis() + 50;
  }

  LCD.data = ball.top;
  LCD.unit = "deg";

  exist = true;
  if (val[top] <= 600) {
    // exCount = 0;
    exist = true;
  } else {
    // exCount++;
    exist = false;
  }

  // if (exCount >= 3) {
  //   exist = false;
  // }
  if (ball.val[ball.top] >= 400) {
    ball.deg = ball.top * 22.5;
  }

  // if (line._deg != 1000 && millis() - ball.lineOut <= 2000) {
  //   if (abs(line._deg - _deg) >= 300 || abs(line._deg - _deg) <= 60) {
  //     exist = false;
  //   } else {
  //     line._deg = 1000;
  //   }
  // } else {
  //   line._deg = 1000;
  // }
}

void _ball::keeper(void) {
  speed = 85;

  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }
  Serial.println(top);

  x = 0;
  y = 0;
  for (int i = 1; i <= 15; i++) {
    if (!(i > 4 && i < 12)) {
      // if (top == i) {
      //   x += sin(radians(i * 22.5)) * val[i];
      // }
      if (i <= 8) {
        x -= val[i];
      } else {
        x += val[i];
      }
      // if (val[i] >= 400) {
      //   x += sin(radians(i * 22.5)) * 100;
      // }
    }
  }

  if (val[top] > 600) {
    exist = false;
  } else {
    exist = true;
  }
  if (top <= 6 || top >= 10) {
    if (x >= 0) {
      if (tof.dist >= 200) {
        deg = 113;
      } else if (tof.dist <= 250) {
        deg = 68;
      } else {
        deg = 90;
      }
    } else {
      if (tof.dist >= 200) {
        deg = 248;
      } else if (tof.dist <= 250) {
        deg = 293;
      } else {
        deg = 270;
      }
    }
  } else {
    if (top >= 8) {
      deg = 120;
    } else {
      deg = 240;
    }
    speed = 30;
    // exist = false;
  }

  if (top <= 3 || top >= 13) {
    speed = 40;
  }

  if (top == 0 && abs(val[1] - val[15]) <= 40) {
    exist = false;
  }

  if (tof.dist >= 550) {
    deg = 180;
    exist = true;
  }

  turn = false;
  emg = false;

  if (top <= 2 || top >= 14 && tof.dist <= 550) {
  } else {
    device.keeperTimer1 = millis();
  }

  if ((top <= 2 || top >= 14) && digitalRead(BALL_HOLD) && tof.dist <= 550) {
    device.keeperTimer1 = millis() - 2100;
  }
}
