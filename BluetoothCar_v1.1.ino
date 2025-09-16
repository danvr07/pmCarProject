#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <ESP32Servo.h>
#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (SDA=25, SCL=26)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Motoare
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

// Ultrasunete
#define TRIG_PIN 14
#define ECHO_PIN 27
#define MAX_DISTANCE 200

// Servomotor
#define SERVO_PIN 32

// Senzor IR pentru margine/prăpastie
#define IR_SENSOR_PIN 34

// Configurări
#define MAX_MOTOR_SPEED 255
#define TURN_SPEED 150
#define OBSTACLE_DISTANCE 20

Servo myServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

boolean goesForward = false;
boolean obstacleAvoidanceMode = false;
int distance = 100;
int speedSet = 0;

void setUpPinModes()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);
  Serial.println("Servomotor inițiat la 90°");
}

void moveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Înainte");
}

void moveBackward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Înapoi");
}

void turnLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Stânga");
}

void turnRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Dreapta");
}

void stopMotors()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Oprire");
}

int readPing()
{
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0)
  {
    cm = MAX_DISTANCE;
  }
  Serial.print("Distanță: ");
  Serial.print(cm);
  Serial.println(" cm");
  return cm;
}

int lookRight()
{
  myServo.write(50);
  delay(500);
  int distance = readPing();
  myServo.write(90);
  return distance;
}

int lookLeft()
{
  myServo.write(130);
  delay(500);
  int distance = readPing();
  myServo.write(90);
  return distance;
}

void avoidObstacles()
{
  int senzorIR = digitalRead(IR_SENSOR_PIN); // LOW = prăpastie
  int distanceR = 0;
  int distanceL = 0;
  distance = readPing();

  if (senzorIR == LOW)
  {
    stopMotors();
    lcd.setCursor(0, 1);
    lcd.print("Prapastie STOP  ");
    delay(200);

    moveBackward();
    delay(400);
    stopMotors();
    delay(200);

    turnLeft(); // Întoarcere completă
    delay(700);
    stopMotors();
    delay(200);
    return;
  }

  if (distance <= OBSTACLE_DISTANCE)
  {
    stopMotors();
    lcd.setCursor(0, 1);
    lcd.print("Obstacol STOP   ");
    delay(100);
    moveBackward();
    delay(300);
    stopMotors();
    delay(200);

    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL)
    {
      turnRight();
      delay(500);
      stopMotors();
    }
    else
    {
      turnLeft();
      delay(500);
      stopMotors();
    }
  }
  else
  {
    moveForward();
    lcd.setCursor(0, 1);
    lcd.print("Drum liber      ");
  }
}

void setup()
{
  Serial.begin(115200);
  setUpPinModes();
  Dabble.begin("MyBluetoothCar");

  Wire.begin(25, 26); // SDA, SCL
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Conecteaza-te");
  lcd.setCursor(0, 1);
  lcd.print("sefule ");

  Serial.println("Dabble și LCD inițiate, aștept conectarea...");
}

void loop()
{
  Dabble.processInput();

  if (!Dabble.isAppConnected())
  {
    lcd.setCursor(0, 0);
    lcd.print("Conecteaza-te ");
    lcd.setCursor(0, 1);
    lcd.print("sefule        ");
    delay(1000);
    return;
  }

  static bool wasConnected = false;
  if (!wasConnected)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mod: Manual");
    wasConnected = true;
  }

  // Activare mod evitare obstacole (include și prăpastie)
  if (GamePad.isTrianglePressed())
  {
    Serial.println("Mod obstacole+prapastie activat");
    obstacleAvoidanceMode = true;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mod: Obstacole+");
    lcd.setCursor(0, 1);
    lcd.print("Prapastii       ");
  }

  // Dezactivare
  if (GamePad.isCrossPressed())
  {
    Serial.println("Mod automat oprit, revenire la manual");
    obstacleAvoidanceMode = false;
    stopMotors();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mod: Manual");
  }

  if (obstacleAvoidanceMode)
  {
    avoidObstacles();
    return;
  }

  // Control manual
  if (GamePad.isUpPressed())
  {
    moveForward();
  }
  else if (GamePad.isDownPressed())
  {
    moveBackward();
  }
  else if (GamePad.isLeftPressed())
  {
    turnLeft();
  }
  else if (GamePad.isRightPressed())
  {
    turnRight();
  }
  else
  {
    stopMotors();
  }
}

