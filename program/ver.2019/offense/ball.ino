// ino

void _ball::read(int* b) {
  *b = round((float)analogRead(BALL0) * 0.87);
  *(b + 1) = analogRead(BALL1);
  *(b + 2) = analogRead(BALL2);
  *(b + 3) = analogRead(BALL3);
  *(b + 4) = analogRead(BALL4);
  *(b + 5) = analogRead(BALL5);
  *(b + 6) = analogRead(BALL6);
  *(b + 7) = round((float)analogRead(BALL7) * 0.9);
  *(b + 8) = round((float)analogRead(BALL8) * 0.9);
  *(b + 9) = round((float)analogRead(BALL9) * 0.9);
  *(b + 8) = ((*(b + 7) + *(b + 9)) * 0.5 + *(b + 8)) * 0.65;
  *(b + 10) = analogRead(BALL10);
  *(b + 11) = analogRead(BALL11);
  *(b + 12) = analogRead(BALL12);
  *(b + 13) = analogRead(BALL13);
  *(b + 14) = analogRead(BALL14);
  *(b + 15) = analogRead(BALL15);

  if (ROBOT == 1) {
    *(b + 1) = (*(b + 0) + *(b + 2)) * 0.5;
    *(b + 5) = (*(b + 4) + *(b + 6)) * 0.5;
  }

  if (ROBOT == 2) {
    *(b + 2) = (*(b + 1) + *(b + 3)) * 0.5;
    *(b + 4) = (*(b + 3) + *(b + 5)) * 0.5;
    // *(b + 8) = (*(b + 7) + *(b + 10)) * 0.5 * 0.85;
    // *(b + 9) = (*(b + 8) + *(b + 10)) * 0.5;
    *(b + 12) = (*(b + 11) + *(b + 14)) * 0.5;
    *(b + 13) = (*(b + 11) + *(b + 15)) * 0.5;
  }
}

void _ball::calc(void) {
  // degは deg = round((float)top * 22.5);まで使用不可
  // motor.power -= 15;

  deg = 1000;

  // motor.power -= 45;

  top = 0;

  x = 0;
  y = 0;

  // x +=
  for (int i = 0; i <= 15; i++) {
    if (val[top] >= val[i]) {
      top = i;
    }
  }
  top_backup = top;

  if (val[top] > 540) {
    exist = false;
    device.attack = false;
  } else {
    exist = true;
  }

  distance = 0;
  for (int i = 0; i <= 15; i++) {
    if (450 >= val[i]) {
      distance++;
    }
  }

  if (millis() - device.keeperTimeout >= 1300) {
    device.attack = true;
  }
  if (top <= 2 || top >= 14) {
    if ((val[top] + val[(top + 1) % 16] + val[(top + 15) % 16]) / 3 < 245) {
      device.attack = true;
    }
  }

  if (device.attack) {
    if (millis() - device.attackTimeout >= 6000) {
      if (val[0] >= 240) {
        device.attack = false;
        device.keeperTimeout = millis();
        device.attackTimeout = millis();
      }
    }
  }

  if (!device.keeper || device.attack) {
    //回り込み
    // if(turn){
    //   motor.power -= 30;
    // }
    // if (device.keeper) {
    //   motor.power -= 20;
    // }

    if (top > 2 && top < 14) {
      if ((val[top] + val[(top + 1) % 16] + val[(top + 15) % 16]) / 3 < 275 ||
          val[top] <= 264 || (top <= 9 && top >= 7 && val[8] <= 388)) {
        // if (distance >= 8) {
        motor.move = 5;
        motor.power -= 15;
        turnTimer = millis();
        if (top > 8) {
          if (top >= 12) {
            top -= 5;
            motor.power -= 15;
          } else {
            top -= 4;
          }
        } else {
          if (top <= 4) {
            top += 5;
            motor.power -= 15;
          } else {
            top += 4;
          }
        }

        top += 16;
        top %= 16;

        turn = 1;
      } else {
        // turn = 0;
        if (turnTimer + 400 <= millis()) {
          turn = 0;
        }
      }
    } else {
      if (turnTimer + 400 <= millis()) {
        turn = 0;
      }

      // if (turn != 0) {
      //   top = 0;
      // }
    }

    deg = round((float)top * 22.5);

    if (line.near && !line.flag) {
      if (line.inTimer + 1000 > millis()) {
        if (line.highPin == 0) {
          if (top_backup <= 2 || top_backup >= 14) {
            line.near = true;
          } else {
            if (exist) {
              line.near = false;
            }
          }
        } else if (line.highPin == 2) {
          if (top_backup >= 2 && top_backup <= 6) {
            line.near = true;
          } else {
            if (exist) {
              line.near = false;
            }
          }
        } else if (line.highPin == 3) {
          if (top_backup >= 6 && top_backup <= 10) {
            line.near = true;
          } else {
            if (exist) {
              line.near = false;
            }
          }
        } else {
          if (top_backup >= 10 && top_backup <= 14) {
            line.near = true;
          } else {
            if (exist) {
              line.near = false;
            }
          }
        }
      } else {
        line.near = false;
      }
    }

    if (line.near) {
      exist = false;
    }
  } else {
    device.attack = false;
    line.near = false;

    motor.power -= 25;

    pauseTimer5();
    usonic.distance = usonic.getDistance();
    startTimer5(50);

    if (val[top] > 560) {
      exist = false;
    } else {
      exist = true;
    }
    if (top <= 8) {
      if (usonic.distance >= 20) {
        deg = 113;
      } else if (usonic.distance <= 25) {
        deg = 68;
      } else {
        deg = 90;
      }
    } else {
      if (usonic.distance >= 20) {
        deg = 248;
      } else if (usonic.distance <= 25) {
        deg = 293;
      } else {
        deg = 270;
      }
    }

    // if (top <= 1 || top >= 15) {
    //   exist = false;
    // }

    if (top == 0) {
      exist = false;
    }

    if (top == (device.keeperExit + 17) % 16) {
      device.attackTimeout = millis();
    } else if (top == device.keeperExit) {
      device.attackTimeout = millis();
    } else if (top == (device.keeperExit + 1) % 16) {
      device.attackTimeout = millis();
    } else if (top == 9 || top == 8) {
      device.attackTimeout = millis();
    } else {
      device.keeperExit = top;
      device.keeperTimeout = millis();
    }

    if (top <= 2 || top <= 14) {
      motor.power -= 30;
    }

    if (top >= 6 && top <= 10) {
      exist = false;
    }

    if (usonic.distance <= 3) {
      // motor.power -= 20;
      deg = 0;
      exist = true;
    }

    if (usonic.distance >= 45) {
      motor.power -= 20;
      if (top >= 5 && top <= 11 && exist) {
        if (top >= 8) {
          deg = 135;
        } else {
          deg = 225;
        }
      } else {
        deg = 180;
      }

      exist = true;
    }

    if (line.near) {
      if (line.inTimer + 3000 > millis()) {
        if (line.highPin == 2) {
          if (top <= 8) {
            line.near = true;
          } else {
            if (exist) {
              line.near = false;
            }
          }
        } else if (line.highPin == 1) {
          if (top >= 8) {
            line.near = true;
          } else {
            if (exist) {
              line.near = false;
            }
          }
        }
      } else {
        line.near = false;
      }
    }

    if (line.near) {
      exist = false;
    }
  }
}

void _ball::reset(void) {
  // if (turn == 0) {
  if (millis() - resetTimer >= 100) {
    digitalWrite(BALL_RESET, LOW);
    resettingTimer = millis();
    while (millis() - resettingTimer <= 7) {
      if (!line.flag) {
        if (exist) {
          motor.drive(motor.deg, motor.power);
        } else {
          motor.drive(NULL, NULL, false, true);
        }
      } else {
        break;
      }
    }
    digitalWrite(BALL_RESET, HIGH);
    resettingTimer = millis();
    while (millis() - resettingTimer <= 7) {
      if (!line.flag) {
        if (exist) {
          motor.drive(motor.deg, motor.power);
        } else {
          motor.drive(NULL, NULL, false, true);
        }
      } else {
        break;
      }
    }
    resetTimer = millis();
    motor.move -= 10;
  } else {
    return;
  }
  // } else {
  //   return;
  // }
}
