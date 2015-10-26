// physical to logical map of GPIOs
int pin_map[8] = { 22, 23, 24, 25, 26, 27, 28, 29};
// Map analog read pins for ADC
int adc_pins[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

//number of electrodes
int max = 8;

// MUX input codes 
// 0: Positive Current input (i+)
int current_pos = 0;
// 1: Negative Current input (i-)
int current_neg = 1;
// 2: Positive Voltage input (v+)
int voltage_pos = 2;
// 3: Negative Voltage input (v-)
int voltage_neg = 3;

// enables pins for i+ and i- MUXs
int enable_0 = 52;
int enable_1 = 53;

// Array of Voltage Values
float voltage_readings[8][5];

// Initialize output GPpins and Serial port for debug
void setup()
{

  Serial.begin(9600);
  pinMode(pin_map[0], OUTPUT);
  pinMode(pin_map[1], OUTPUT);
  pinMode(pin_map[2], OUTPUT);
  pinMode(pin_map[3], OUTPUT);
  pinMode(pin_map[4], OUTPUT);
  pinMode(pin_map[5], OUTPUT);
  pinMode(pin_map[6], OUTPUT);
  pinMode(pin_map[7], OUTPUT);

  // disable all MUXs
  digitalWrite(enable_0, HIGH);
  digitalWrite(enable_1, HIGH);
  
  // set intial positions of all 4 multiplexers
  set_muxes(current_neg, current_pos);

  // enable all MUXs
  digitalWrite(enable_0, LOW);
  digitalWrite(enable_1, LOW);
  
}

// Sample mux_selecting
void loop()
{
  fet = Serial.parseInt()
  gnd = fet + 1;
  if(gnd == 8)
      grd = 0;
  set_muxes(fet, gnd);
}


// Activate a mux's lane
// lane - number 0-7
// mux - number 0-1, for a 2 mux system
void mux_select(int lane, int mux)
{
  //function for choosing select line on the MUX
  //int input - lane is converted to binary
  //binary values control digital outputs
  int x;
  int bit_value;
  for(x = 0; x < 4; x++) {
    bit_value = bitRead(lane,x);
    //Serial.println(bit_value);
    if(bit_value == 1)
      digitalWrite(pin_map[mux * 4 + x],HIGH);
    else 
      digitalWrite(pin_map[mux * 4 + x],LOW);
  }
  delay(5);
}

// Select 2 lanes
void set_muxes(int i_minus, int i_plus)
{
	//sets MUXs in order -> i- , i+
    mux_select(i_minus, 1);
    mux_select(i_plus,  0);
}

float voltageRead(int read_pin)
{
  //Read input analog pin
  delay(5);
  int sensorvalue = analogRead(read_pin);
  //convert to floating point voltage value
  float voltage = sensorvalue * (5.0/1023.0);
  //return this value
  return voltage;
}

float voltage_diff(float value1, float value2)
{
  //complete voltage values across resistor
  float voltage_value = abs(value1 - value2);
  //return this value
  return voltage_value;
}
