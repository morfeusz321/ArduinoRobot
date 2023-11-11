/* *
 * Student name : Filip Błaszczyk
 * Student number : 5575958
 *
 */
#define HB_A1A PB3
#define HB_A1B PA8
#define HB_B1A PB15
#define HB_B2A PB14
#define TRIG_FRONT PA9
#define ECHO_FRONT PB8
#define TRIG_SIDE PA10
#define ECHO_SIDE PB9
#define SENSOR_LEFT PB12
#define SENSOR_RIGHT PB13
#define STATE_ONE 1
#define STATE_TWO 2
#define STATE_THREE 3
long duration;
int cm;
int state;
bool is_first_time;
int irs_counter;
bool entered_loop;
int started_state_two;
bool realined;


void set_velocity(int left_velocity_forward, int right_velocity_forward,int left_velocity_backward, int right_velocity_backward){
  analogWrite(HB_A1B,left_velocity_forward);
  analogWrite(HB_B2A, right_velocity_forward);
  analogWrite(HB_A1A,left_velocity_backward);
  analogWrite(HB_B1A,right_velocity_backward);
}
void set_velocity_forward(int velocity){
  set_velocity(velocity, velocity,0,0);
}

void set_velocity_backward(int velocity){
  set_velocity(0,0,velocity, velocity);
}

void turn_right(int velocity,int delayTime){
  set_velocity(velocity, 0,0,0);
  delay(delayTime);
  // set_velocity(0,0,velocity,0);
  set_velocity_forward(0);  
}

void turn_left(int velocity,int delayTime){
  set_velocity(0,velocity,0,0);
  delay( delayTime);
  set_velocity_forward(0);
}

void turn_right_slow(int velocity){
  set_velocity(velocity, 0,0,0);
  delay(50);
  // set_velocity(0,0,velocity,0);
  set_velocity_forward(0);  
}

void turn_left_slow(int velocity){
  set_velocity(0,velocity,0,0);
  delay(50);
  set_velocity_forward(0);
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

int get_distance_side(){
  return get_distance(TRIG_SIDE,ECHO_SIDE);
}
void ISR_sensor(){
  
  if(state == STATE_TWO && started_state_two>20){
    state = STATE_THREE;
  
  }
  if(state == STATE_TWO){
    started_state_two++;
  }
  // if(state == STATE_THREE || !entered_loop){
  //   set_velocity_forward(0);
  //   started_state_two=0;
  // }
  
}
void reverse(){
  set_velocity_backward(200);
  delay(400);
  turn_left(200,1000);
    turn_left(200,1000);
    turn_left(200,1000);
    turn_right(200,1100);
    set_velocity_forward(0);
    delay(2000);
}
void setup() {
  pinMode(HB_A1A, OUTPUT);
  pinMode(HB_A1B, OUTPUT);
  pinMode(HB_B1A, OUTPUT);
  pinMode(HB_B2A, OUTPUT);
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_SIDE, OUTPUT);
  pinMode(ECHO_SIDE, INPUT);
  pinMode(SENSOR_LEFT,INPUT);
  pinMode(SENSOR_RIGHT,INPUT);
  state = STATE_ONE;
  is_first_time = true;
  irs_counter = 0;
  entered_loop = false;
  started_state_two = 0;
  realined = true;
  attachInterrupt(digitalPinToInterrupt(SENSOR_LEFT),ISR_sensor,RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_RIGHT),ISR_sensor,RISING);

}

void loop() {
  int distance_front = get_distance_front();
  int distance_side = get_distance_side();
  if (state == STATE_ONE) {
    if(distance_front<10){
      state = STATE_TWO;
      set_velocity_forward(0);
      
    } else {
      if(digitalRead(SENSOR_RIGHT)==HIGH){
      turn_right_slow(200);
      }else if(digitalRead(SENSOR_LEFT)==HIGH){
      turn_left_slow(200);
      }else{
      set_velocity_forward(200);
     }  
    }
  }
  if (state == STATE_TWO) {
     if(distance_side > 10){
      if(distance_front <10){
        set_velocity_backward(200);
        delay(400);
        turn_right(200,1000);
        delay(200);
      }else{
        turn_left(200,200);
      }  
    }
    else {
      set_velocity_forward(200);
      delay(200);
      set_velocity_forward(0);
    }
  }
  if(state == STATE_THREE){
      realined =  true;
      entered_loop = true;
      distance_front = get_distance_front();
      while(digitalRead(SENSOR_RIGHT)!=HIGH){
        if(distance_front<10){
        realined =  false;
        reverse();
        distance_front = get_distance_front();
      }
        turn_left_slow(150);
        distance_front = get_distance_front();

      }
      while(digitalRead(SENSOR_LEFT)!=HIGH){
        if(distance_front<10){
        realined =  false;
        reverse();
      }
        turn_right_slow(150);
        distance_front = get_distance_front();

      }
      while(digitalRead(SENSOR_RIGHT)!=HIGH){
      if(distance_front<10){
      realined =  false;
      reverse();
    }
        turn_left_slow(150);
        distance_front = get_distance_front();
      }
      if(realined == true){
      state = STATE_ONE;  
      }   
  }
}

