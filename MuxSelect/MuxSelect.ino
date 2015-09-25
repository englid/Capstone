// Initialize output GPIOs and Serial port for debug
void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
}

// Sample mux_selecting
void loop()
{
  mux_select(5, 0);
  delay(5000);
  mux_select(7, 0);
  delay(5000);
}

// Activate a mux's lane
// lane - number 0-15
// mux - number 0-3, for a 4 mux system
void mux_select(int lane, int mux)
{
  int s[4];
  for(int i = 0; i < 4; i++){
    s[i] =  pow(2, i) & lane;
    if (s[i] != 0){
      digitalWrite(mux * 4 + i,HIGH);
    } else {
      digitalWrite(mux * 4 + i,LOW);
    }
  }
}

// Select 4 lanes
void set_muxes(int first, int second, int third, int forth)
{
    mux_select(first, 0);
    mux_select(second, 1);
    mux_select(third, 2);
    mux_select(forth, 3);
}
