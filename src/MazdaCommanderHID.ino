#include <Arduino.h>
#include <HID-Project.h>

#define LIN_BAUD 19200
#define LIN_ID 0xC1
#define FRAME_LEN 9

#define DEBUG_MODE 0
#define DISABLE_PIN 9

uint8_t idleFrame[FRAME_LEN] = {0x55,0xC1,0x00,0x7C,0x00,0xC0,0x00,0x00,0x01};
uint8_t frame[FRAME_LEN];

struct Result {
  const char *name;
  uint8_t captured[FRAME_LEN];
  bool active;
  KeyboardKeycode kb;
  uint16_t consumer;
};

Result controls[] = {
// name        |LIN packet                                    |pressed|keyboard    | consumer
  {"d0_up",     {0x55,0xC1,0x00,0x7C,0x01,0xC0,0x00,0x00,0x00}, false, KEY_UP_ARROW, 0},
  {"d0_down",   {0x55,0xC1,0x00,0x7C,0x10,0xC0,0x00,0x00,0xF0}, false, KEY_DOWN_ARROW, 0},
  {"d0_left",   {0x55,0xC1,0x00,0x7C,0x40,0xC0,0x00,0x00,0xC0}, false, KEY_LEFT_ARROW, 0},
  {"d0_right",  {0x55,0xC1,0x00,0x7C,0x04,0xC0,0x00,0x00,0xFC}, false, KEY_RIGHT_ARROW, 0},
  {"d0_press",  {0x55,0xC1,0x00,0x7C,0x00,0xC4,0x00,0x00,0xFC}, false, KEY_RETURN, 0},
  {"d0_r_left", {0x55,0xC1,0x00,0x7C,0x00,0xC2,0x00,0x01,0xFD}, false, KEY_HOME, 0},
  {"d0_r_right",{0x55,0xC1,0x00,0x7C,0x00,0xC0,0x00,0x01,0x00}, false, KEY_END, 0},
  {"d1_press",  {0x55,0xC1,0x00,0x7C,0x00,0xE0,0x00,0x00,0xE0}, false, 0, MEDIA_VOL_MUTE},
  {"d1_r_left", {0x55,0xC1,0x00,0x7C,0x00,0xD0,0x01,0x00,0xEF}, false, 0, MEDIA_VOL_DOWN},
  {"d1_r_right",{0x55,0xC1,0x00,0x7C,0x00,0xC0,0x01,0x00,0x00}, false, 0, MEDIA_VOL_UP},
  {"music",     {0x55,0xC1,0x20,0x7C,0x00,0xC0,0x00,0x00,0xE0}, false, KEY_N, 0},
  {"home",      {0x55,0xC1,0x02,0x7C,0x00,0xC0,0x00,0x00,0xFE}, false, KEY_SPACE, 0},
  {"nav",       {0x55,0xC1,0x08,0x7C,0x00,0xC0,0x00,0x00,0xF8}, false, KEY_M, 0},
  {"back",      {0x55,0xC1,0x00,0x7D,0x00,0xC0,0x00,0x00,0x00}, false, KEY_ESC, 0},
  {"star",      {0x55,0xC1,0x80,0x7C,0x00,0xC0,0x00,0x00,0x80}, false, KEY_TAB, 0}
};

const int CONTROL_COUNT = sizeof(controls)/sizeof(Result);
bool HID_enabled = true;

void sendBreak() {
  Serial1.end();
  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);
  delayMicroseconds(700);
  digitalWrite(1, HIGH);
  delayMicroseconds(100);
  Serial1.begin(LIN_BAUD);
}

bool pollFrame() {
  sendBreak();
  Serial1.write(0x55);
  Serial1.write(LIN_ID);
  
  int idx = 0;
  unsigned long t = millis();
  while(idx < FRAME_LEN && millis() - t < 12) {
    if(Serial1.available()) frame[idx++] = Serial1.read();
  }
  return idx == FRAME_LEN;
}

bool framesEqual(uint8_t *a, uint8_t *b) {
  for(int i=0; i<FRAME_LEN; i++) if(a[i] != b[i]) return false;
  return true;
}

void handleControl(Result &c) {
  if (HID_enabled) {
    if (c.consumer > 0) {
      Consumer.write(c.consumer);
    } else if (c.kb != 0) {
      Keyboard.press(c.kb);
    }
  }
  if (DEBUG_MODE) { Serial.println(c.name); }
}

void releaseControl(Result &c) {
  if (HID_enabled && c.kb != 0) {
    Keyboard.release(c.kb);
  }
}

void setup() {
  delay(15000);
  pinMode(DISABLE_PIN, INPUT_PULLUP);
  HID_enabled = (digitalRead(DISABLE_PIN) == LOW); 

  if (DEBUG_MODE) {
    Serial.begin(115200);
    while(!Serial); 
  }

  Serial1.begin(LIN_BAUD);

  if (HID_enabled) {
    Keyboard.begin();
    Consumer.begin();
  }
}

void loop() {
  if (!pollFrame()) return;

  bool isIdle = framesEqual(frame, idleFrame);

  for (int i = 0; i < CONTROL_COUNT; i++) {
    bool activeNow = !isIdle && framesEqual(frame, controls[i].captured);

    if (activeNow && !controls[i].active) {
      handleControl(controls[i]);
      controls[i].active = true;
    } 
    else if (!activeNow && controls[i].active) {
      releaseControl(controls[i]);
      controls[i].active = false;
    }
  }
}