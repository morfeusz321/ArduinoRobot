/* *
 * Student name : Filip Błaszczyk
 * Student number : 5575958
*/
#define HB_A1A PB3
#define HB_A1B PA8
#define HB_B1A PB15
#define HB_B2A PB14
#define TRIG_FRONT PA9
#define ECHO_FRONT PB8
#define STATE_ONE 1
#define STATE_TWO 2
volatile unsigned long pulseInTimeBegin = micros();
volatile unsigned long pulseInTimeEnd = micros();
volatile bool newPulseDurationAvailable = false;
int cm;
int duration;
int state;
bool is_first_time;

void set_velocity(int left_velocity_forward, int right_velocity_forward,int left_velocity_backward, int right_velocity_backward){
  analogWrite(HB_A1B,left_velocity_forward);
  analogWrite(HB_B2A, right_velocity_forward);
  analogWrite(HB_A1A,left_velocity_backward);
  analogWrite(HB_B1A,right_velocity_backward);
}
void set_velocity_forward(int velocity){
  set_velocity(velocity, velocity,0,0);
}

int get_distance(int triger, int echo){
  if(is_first_time){
    delay(500);
    is_first_time = false;
  }
  digitalWrite(triger, LOW);
  delayMicroseconds(5);
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;  
  return cm;
}
int get_distance_front(){
  return get_distance(TRIG_FRONT,ECHO_FRONT);
}
void start_timer(){
  if(digitalRead(ECHO_FRONT) == HIGH){
    pulseInTimeBegin = micros();
  }
  else{
    newPulseDurationAvailable = true;
    pulseInTimeEnd = micros();
  }
}
void stop_timer(){
  
}
void setup() {
  pinMode(HB_A1A, OUTPUT);
  pinMode(HB_A1B, OUTPUT);
  pinMode(HB_B1A, OUTPUT);
  pinMode(HB_B2A, OUTPUT);
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_FRONT),
                  start_timer,
                  CHANGE);
}

void loop() {
  digitalWrite(TRIG_FRONT, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_FRONT, LOW);
  pinMode(ECHO_FRONT, INPUT);

  if(newPulseDurationAvailable){
    newPulseDurationAvailable = true;
    unsigned long pulseDuration = pulseInTimeEnd - pulseInTimeBegin;
    int cm = (pulseDuration/2) / 29.1;
    if(cm<15){
      set_velocity_forward(0);
    }  

  }else{
    set_velocity_forward(200);
  }
  // long front_distance = get_distance_front();
  // if(front_distance>10){
  //   set_velocity_forward(200);
  // }else{
  //   set_velocity_forward(0);
  // }
}

