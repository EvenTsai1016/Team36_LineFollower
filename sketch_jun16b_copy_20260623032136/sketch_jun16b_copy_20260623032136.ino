// =========================
// 感測器
// =========================
#define S1 32
#define S2 33
#define S3 25
#define S4 16
#define S5 17

// =========================
// PWM
// =========================
#define ENA 5
#define ENB 12

#define PWM_FREQ 1000
#define PWM_RES 8

// =========================
// 按鈕
// =========================
#define BUTTON 4

// =========================
// 馬達
// =========================
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

// =========================
// RGB
// =========================
#define RED   13
#define GREEN 14
#define BLUE  15

bool started = false;

// =========================
// 速度設定
// =========================
int baseSpeed = 140;
int turnSpeed = 255;

// =========================
// RGB
// =========================
void WAIT()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
}

void AUTO()
{
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void SAFE()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

// =========================
// PWM速度
// =========================
void setMotorSpeed(int leftSpeed, int rightSpeed)
{
  ledcWrite(ENA, leftSpeed);
  ledcWrite(ENB, rightSpeed);
}

// =========================
// 馬達控制
// =========================
void stopMotor()
{
  setMotorSpeed(0, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward()
{
  setMotorSpeed(baseSpeed, baseSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// 線偏左 -> 往右修正
void turnRight()
{
  setMotorSpeed(0, turnSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// 線偏右 -> 往左修正
void turnLeft()
{
  setMotorSpeed(turnSpeed, 0);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =========================
// Setup
// =========================
void setup()
{
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(115200);

  // ESP32 Core 3.x PWM
  ledcAttach(ENA, PWM_FREQ, PWM_RES);
  ledcAttach(ENB, PWM_FREQ, PWM_RES);

  stopMotor();
  WAIT();
}

// =========================
// Loop
// =========================
void loop()
{
  if (!started)
  {
    WAIT();
    stopMotor();

    if (digitalRead(BUTTON) == LOW)
    {
      delay(50);

      if (digitalRead(BUTTON) == LOW)
      {
        started = true;

        while (digitalRead(BUTTON) == LOW)
        {
          delay(10);
        }
      }
    }

    return;
  }

  AUTO();

  int s1 = digitalRead(S1);
  int s2 = digitalRead(S2);
  int s3 = digitalRead(S3);
  int s4 = digitalRead(S4);
  int s5 = digitalRead(S5);

  Serial.print(s1);
  Serial.print(s2);
  Serial.print(s3);
  Serial.print(s4);
  Serial.println(s5);

  // =========================
  // 中間 -> 前進
  // =========================
  if (
      (s1==0 && s2==0 && s3==1 && s4==0 && s5==0) || //00100
      (s1==0 && s2==1 && s3==1 && s4==1 && s5==0) || //01110
      (s1==1 && s2==1 && s3==1 && s4==1 && s5==1)    //11111
     )
  {
    forward();
  }

  // =========================
  // 線偏左
  // =========================
  else if (
      (s1==1 && s2==0 && s3==0 && s4==0 && s5==0) ||
      (s1==1 && s2==1 && s3==0 && s4==0 && s5==0) ||
      (s1==0 && s2==1 && s3==0 && s4==0 && s5==0) ||
      (s1==0 && s2==1 && s3==1 && s4==0 && s5==0) ||
      (s1==1 && s2==1 && s3==1 && s4==0 && s5==0)
     )
  {
    turnRight();
  }

  // =========================
  // 線偏右
  // =========================
  else if (
      (s1==0 && s2==0 && s3==0 && s4==0 && s5==1) ||
      (s1==0 && s2==0 && s3==0 && s4==1 && s5==1) ||
      (s1==0 && s2==0 && s3==0 && s4==1 && s5==0) ||
      (s1==0 && s2==0 && s3==1 && s4==1 && s5==0) ||
      (s1==0 && s2==0 && s3==1 && s4==1 && s5==1)
     )
  {
    turnLeft();
  }

  // =========================
  // 全白
  // =========================
  else if (
      s1==0 &&
      s2==0 &&
      s3==0 &&
      s4==0 &&
      s5==0
     )
  {
    stopMotor();
    SAFE();
  }

  else
  {
    forward();
  }

  delay(5);
}