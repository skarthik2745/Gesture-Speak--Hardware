
#include <LiquidCrystal_I2C.h>

// Try address 0x27 first. If not working, change to 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Flex sensor pins
int thumbPin = A0;
int indexPin = A1;
int middlePin = A2;
int ringPin = A3;

// Thresholds based on your actual readings
int STRAIGHT_TH = 200;  // Above → Straight
int BENT_TH     = 120;  // Below → Bent

// Sensor readings
int t, i, m, r;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gesture Glove");
  delay(1200);
  lcd.clear();
}

void loop() {

  // Read flex values
  t = analogRead(thumbPin);
  i = analogRead(indexPin);
  m = analogRead(middlePin);
  r = analogRead(ringPin);

  // Determine finger states
  bool thumbStraight  = t > STRAIGHT_TH;
  bool indexStraight  = i > STRAIGHT_TH;
  bool middleStraight = m > STRAIGHT_TH;
  bool ringStraight   = r > STRAIGHT_TH;

  bool thumbBent  = t < BENT_TH;
  bool indexBent  = i < BENT_TH;
  bool middleBent = m < BENT_TH;
  bool ringBent   = r < BENT_TH;

  // Count number of straight fingers
  int straightCount = thumbStraight + indexStraight + middleStraight + ringStraight;

  // Display number ALWAYS on top
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number:");
  lcd.print(straightCount);

  // Gesture message for 2nd row
  String gesture = "";

  // ---------------------------------------------------
  //                 GESTURE CONDITIONS
  // ---------------------------------------------------

  // YES / OK
  if (thumbStraight && indexBent && middleBent && ringBent)
    gesture = "YES / OK";

  // NO
  else if (thumbBent && indexStraight && middleStraight && ringStraight)
    gesture = "NO";

  // SUPER (half-bend)
  else if (!thumbBent && !thumbStraight &&
           !indexBent && !indexStraight &&
           middleStraight && ringStraight)
    gesture = "SUPER";

  // SELECT
  else if (indexStraight && thumbBent && middleBent && ringBent)
    gesture = "SELECT";

  // STOP (all straight)
  else if (thumbStraight && indexStraight && middleStraight && ringStraight)
    gesture = "STOP";

  // START 
  else if (thumbBent && indexBent && middleBent && ringBent)
    gesture = "START";

  // LOVE
  else if (thumbStraight && middleBent && ringBent)
    gesture = "LOVE";

  // ASK
  else if (thumbStraight && ringStraight && indexBent && middleBent)
    gesture = "ASK";


  // ---------------------------------------------------

  lcd.setCursor(0,1);
  lcd.print(gesture);

  delay(250);
}
/**************************************
Here is your full finger chart.
(S = Straight, B = Bent)

Legend:

T = Thumb

I = Index

M = Middle

R = Ring

L = Little (ignored)

🟩 1. YES / OK

T	I	M	R

S	B	B	B


👉 Just thumbs-up style.


🟥 2. NO

T	I	M	R

B	S	S	S


👉 Only thumb is bent.


🟧 3. SMALL

T	I	M	R

Middle	Middle	S	S


👉 Thumb + Index slight bend only.

🟦 4. SELECT

T	I	M	R

B	S	B	B


👉 Only index straight.



🟪 6. STOP

T	I	M	R

S	S	S	S


👉 All straight.

🟫 7. START (FIST)

T	I	M	R

B	B	B	B


👉 Make a full fist.

💜 8. LOVE

T	I	M	R

S	Any	B	B


👉 Thumb up + last two bent.

💚 9. ASK

T	I	M	R

S	B	B	S


👉 Index + Middle bent.

 *************/