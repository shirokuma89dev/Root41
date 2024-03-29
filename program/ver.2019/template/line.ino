// line.ino

_line::_line(void) {
  for (int i = 0; i <= 3; i++) {
    val[i] = false;
  }
}

void _line::read(void) {
  touch = false;

  val[0] = !digitalRead(LINE1);
  val[1] = !digitalRead(LINE2);
  val[2] = !digitalRead(LINE3);
  val[3] = !digitalRead(LINE4);

  //どれか一つでも反応しているならば
  if (val[0] | val[1] | val[2] | val[3]) {
    touch = true;
  }
}
