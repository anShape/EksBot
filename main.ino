#define s1 A0
#define s2 A1
#define s3 A2
#define s4 A3
#define s5 A4
#define s6 A5

#define irr 13
#define irl 12 

#define pin_ML1 3
#define pin_ML2 11

#define pin_MR1 9
#define pin_MR2 10

#define echoPin 4 // attach pin D4 Arduino to pin Echo of HC-SR04
#define trigPin 7 //attach pin D7 Arduino to pin Trig of HC-SR04

#define batas_ir 600

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

int sir[6];

bool pertigaan=0, perempatan=0;

int flag=0; //0 tengah, 1 kanan, 2 kiri

int c3=0, c4=0; //Count pertigaan, perempatan

bool center=0, left=0, right=0;

int kec_tinggi = 55;
int kec_med = 25;
int kec_rendah = 10;
int kec_def = 20;

int set_speed = 50;
int err=0;
int steering = err*4;
int speed_L = set_speed + steering;
int speed_R = set_speed - steering;

int start=1;
int troli=0;

void setup() {
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);
  pinMode(s6, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(pin_ML1, OUTPUT);
  pinMode(pin_ML2, OUTPUT);
  pinMode(pin_MR1, OUTPUT);
  pinMode(pin_MR2, OUTPUT);
  pinMode(irl, INPUT);
  pinMode(irr, INPUT);
  Serial.begin(9600);

  for(int i=0; i<6; i++){
    sir[i]=0;
  }
}

void irread(){
  sir[0] = analogRead(s1);
  sir[1] = analogRead(s2);
  sir[2] = analogRead(s3);
  sir[3] = analogRead(s4);
  sir[4] = analogRead(s5);
  sir[5] = analogRead(s6);
  
  right = digitalRead(irr);
  left = digitalRead(irl);
  
  for(int i = 0; i<6; i++){
    if(sir[i]<batas_ir){//Jika sensor mendeteksi putih
      sir[i] = 0;
    }
    else{
      sir[i] = 1;//Sensor mendeteksi hitam
    }
  }
}

void motor(int speedL, int speedR)
{
  if(speedL > 0)
  {
    analogWrite(pin_ML1, speedL);
    digitalWrite(pin_ML2, LOW);
  }
  else if (speedL < 0)
  {
    analogWrite(pin_ML1, -speedL);
    digitalWrite(pin_ML2, LOW);
  }
  else
  {
    digitalWrite(pin_ML1, LOW);
    digitalWrite(pin_ML2, LOW);
  }

  if(speedR > 0)
  {
    analogWrite(pin_MR1, speedR);
    digitalWrite(pin_MR2, LOW);
  }
  else if (speedR < 0)
  {
    analogWrite(pin_MR1, -speedR);
    digitalWrite(pin_MR2, LOW);
  }
  else
  {
    digitalWrite(pin_MR1, LOW);
    digitalWrite(pin_MR2, LOW);
  }
}

int hcread(){
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
}

void readln(){
  if (sir[2] || sir[3]) err=0;
  else if(sir[1] || sir[4]){
    if(sir[1]) err=-1;
    else err=1;
  }
  else if(sir[0] || sir[5]){
    if(sir[0]) err=-2;
    else err=2;
  }
  else if(left || right){
    if (left) err=-4;
    else err=4;
  }
  steering = err*4;
  speed_L = set_speed + steering;
  speed_R = set_speed - steering;
}

void obs(){
  if (hcread() <= 30){
    motor(0,0);
    delay(1000);
  }
}

void Case0(){
  while(start){
    obs();
    irread();
    if(sir[0]||sir[1]||sir[2]||sir[3]||sir[4]||sir[5]||right||left) {
      start=0;
      break;
    }
    readln();
    motor(speed_L,speed_R);
    delay(10);
  }
}

void Right(){
  motor(50,0);
  delay(500);
}

void Left(){
  motor(0,50);
  delay(500);
}

void Emaju(){
  int flag=1;
  while(flag){
    obs();
    irread();
    readln();
    motor(speed_L,speed_R);
    if(!right && !left) flag=0;
  }
}

void Maju(){
  int flag=1;
  while(flag){
    obs();
    irread();
    readln();
    motor(speed_L,speed_R);
    if(((right&&sir[3]) || (left&&sir[2]))) flag=0;
  }
}

void Mundur(){
  int flag=1;
  while(flag){
    obs();
    irread();
    readln();
    motor(-speed_L,-speed_R);
    if(((right&&sir[3]) || (left&&sir[2]))){
      flag=0;
      delay(1000);
    }
  }
}

void Tmaju(){
  int flag=1;
  while(flag){
    obs();
    irread();
    readln();
    motor(speed_L,speed_R);
    if(!right || !left) flag=0;
  }
}

void loop() {
  Case0();
  Emaju();
  Maju();
  Right();
  Maju();
  for(int i=0; i<5; i++){
    Emaju();
    Maju();  
  }
  Left();
  Mundur();
  Case0();
  Emaju();
  Maju();
  while(1){
    for(int i=0; i<4; i++){
      Tmaju();
      Maju();
    }
    Left();
    Maju();
    for(int i=0; i<5; i++){
      Emaju();
      Maju();
    }
    Left();
  }
}