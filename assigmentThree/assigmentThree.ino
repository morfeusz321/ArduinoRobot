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
#define STATE_ONE 1
#define STATE_TWO 2
long duration;
int cm;
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
void turn_left(int velocity){
  set_velocity(velocity,0,0,0);
}
void turn_right(int velocity){
  set_velocity(0,velocity,0,0);
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
  state = STATE_ONE;
}

void loop() {
  if(state==STATE_ONE){
    if(digitalRead(SENSOR_RIGHT)==HIGH){
      state = STATE_TWO;
    }else{
      turn_right(170);
    }
  }
  if(state==STATE_TWO){
    if(digitalRead(SENSOR_LEFT)==HIGH){
      state=STATE_ONE;
    }else{
      turn_left(170);
    }
  }
  // if(digitalRead(SENSOR_RIGHT)==HIGH){
  //     turn_left(150);
  // }else if(digitalRead(SENSOR_LEFT)==HIGH){
  //     turn_right(150);
  // }else{
  //   set_velocity_forward(150);
  // }
  

}

