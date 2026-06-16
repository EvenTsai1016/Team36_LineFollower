// 感測器
#define S1 32
#define S2 33
#define S3 25
#define S4 26
#define S5 27

// 馬達
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

const int RED = 13;
const int GREEN = 14;
const int BLUE = 15;

void setup()
{
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(4, INPUT_PULLUP);

  Serial.begin(115200);

  WAIT();      // 藍燈
  delay(3000); // 展示待命3秒
}

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void leftTurn()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rightTurn()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopMotor()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void SAFE()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void AUTO()
{
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void WAIT()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
}


void loop()
{

  if(digitalRead(4)==LOW)
  {
    stopMotor();
    SAFE();

    while(1);
  }


  AUTO();

  int s1 = digitalRead(S1);
  int s2 = digitalRead(S2);
  int s3 = digitalRead(S3);
  int s4 = digitalRead(S4);
  int s5 = digitalRead(S5);

  Serial.print(s1);
  Serial.print(" ");
  Serial.print(s2);
  Serial.print(" ");
  Serial.print(s3);
  Serial.print(" ");
  Serial.print(s4);
  Serial.print(" ");
  Serial.println(s5);

  // 黑線在中間
  if(s1==1 && s2==1 && s3==0 && s4==1 && s5==1)
  {
    forward();
  }

  // 黑線偏左
  else if(s5==1 && s4==0)
  {
    leftTurn();
  }

  // 黑線偏更左
  else if(s5==0)
  {
    leftTurn();
  }

  // 黑線偏右
  else if(s2==0 && s1==1)
  {
    rightTurn();
  }

  // 黑線偏更右
  else if(s1==0)
  {
    rightTurn();
  }

  else
  {
    stopMotor();
    SAFE();
  }
}
