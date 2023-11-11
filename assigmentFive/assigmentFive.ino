/* *
 * Student name : Filip Błaszczyk
 * Student number : 5575958
 *
*/
#define HB_A1A PB3
#define HB_A1B PA8
#define HB_B1A PB15
#define HB_B2A PB14
#define TRIG PA10
#define ECHO PB9
#define SENSOR_LEFT PB12
#define SENSOR_RIGHT PB13

#define SPEED_ENCODER_RIGHT PA0
#define SPEED_ENCODER_LEFT PA1

#define SLOTS_PER_CM 0.952380952

#define THRESHOLD 400.0f
#define STATE_ONE 1
#define STATE_TWO 2
#define STATE_THREE 3

double distance = 150;
double pulses = 9 * distance;

long leftPings = 0;
long rightPings = 0;

bool isOpen;
int state;


void set_velocity(int left_velocity_forward, int right_velocity_forward,int left_velocity_backward, int right_velocity_backward){
  analogWrite(HB_A1B,left_velocity_forward);
  analogWrite(HB_B2A, right_velocity_forward);
  analogWrite(HB_A1A,left_velocity_backward);
  analogWrite(HB_B1A,right_velocity_backward);
}
void set_velocity_forward(int velocity){
  set_velocity(velocity, velocity,0,0);
}
void turn_left_slow(int velocity){
  set_velocity(velocity,0,0,velocity);
}
void turn_right_slow(int velocity){
  set_velocity(0,velocity,velocity,0);
}
long getPings(){
  return (leftPings+rightPings)/2;
}
void ISR_speed_right(){
  rightPings++;
}
void ISR_speed_left(){
  leftPings++;
}
void ISR_sensor_right(){
  if(digitalRead(SENSOR_LEFT)==LOW){
    state= STATE_THREE;
  }else{
    state = STATE_TWO;
  }
}
void ISR_sensor_left(){
  if(digitalRead(SENSOR_RIGHT)==LOW){
    state= STATE_THREE;
  }else{
    state = STATE_ONE;
  }
}
void setup() {
  pinMode(HB_A1A, OUTPUT);
  pinMode(HB_A1B, OUTPUT);
  pinMode(HB_B1A, OUTPUT);
  pinMode(HB_B2A, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(SENSOR_LEFT,INPUT);
  pinMode(SENSOR_RIGHT,INPUT);
  pinMode(SPEED_ENCODER_LEFT, INPUT);
  pinMode(SPEED_ENCODER_RIGHT, INPUT);

  attachInterrupt(digitalPinToInterrupt(SPEED_ENCODER_LEFT),ISR_speed_right,CHANGE);
  attachInterrupt(digitalPinToInterrupt(SPEED_ENCODER_RIGHT),ISR_speed_left,CHANGE);
  attachInterrupt(digitalPinToInterrupt(SENSOR_LEFT),ISR_sensor_right,CHANGE);
  attachInterrupt(digitalPinToInterrupt(SENSOR_RIGHT),ISR_sensor_left,CHANGE);

  isOpen = false;
  state = STATE_THREE;
}

void loop() {

  if (leftPings >= pulses && rightPings>=pulses) {
      set_velocity_forward(0);
  }else{
    if(state == STATE_TWO){
      turn_right_slow(200);
    }else if(state==STATE_ONE){
      turn_left_slow(200);
    }else{
      set_velocity_forward(200);
    }
  }
}