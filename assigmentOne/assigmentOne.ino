/* *
 * Student name : Filip Błaszczyk
 * Student number : 5575958
 *
*/
//MOTORS

#define HB_A1A PB3
#define HB_A1B PA8
#define HB_B1A PB15
#define HB_B2A PB14

// void drive(i)
void set_velocity(int left_velocity_forward, int right_velocity_forward,int left_velocity_backward, int right_velocity_backward){
  analogWrite(HB_A1B,left_velocity_forward);
  analogWrite(HB_B2A, right_velocity_forward);
  analogWrite(HB_A1A,left_velocity_backward);
  analogWrite(HB_B1A,right_velocity_backward);
}
void set_velocity_forward(int velocity){
  set_velocity(velocity, velocity,0,0);
}
void setup() {
  pinMode(HB_A1A, OUTPUT);
  pinMode(HB_A1B, OUTPUT);
  pinMode(HB_B1A, OUTPUT);
  pinMode(HB_B2A, OUTPUT);
  set_velocity_forward(150);
  delay(4000);
  set_velocity_forward(225);
  delay(4000);
  int velo = 225;
  while(velo>0) {
    delay(100);
    velo = velo - 1;
    set_velocity_forward(velo);
  }
}

void loop() {
  
}

