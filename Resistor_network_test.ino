// physical to logical map of GPIOs
int pin_map[8] = { 22, 23, 24, 25, 26, 27, 28, 29};
// Map analog read pins for ADC
int adc_pins[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

//number of electrodes
int max = 8;

// MUX input codes 
// 0: Positive Current input (i+)
int current_pos = 1;
// 1: Negative Current input (i-)
int current_neg = 0;
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
  pinMode(enable_1,   OUTPUT);

  // disable all MUXs
  digitalWrite(enable_0, HIGH);
  digitalWrite(enable_1, HIGH);
  
  // set intial positions of all 4 multiplexers
  set_muxes(current_neg, current_pos);

  digitalWrite(enable_1, LOW);
  //function call for polarizing water
  for (int i = 0; i < max; i++) {
    current_pos +=1;
    if (current_neg == max)
        current_neg = 0;
    if (current_pos == max)
        current_pos = 0;
    set_muxes(current_neg, current_pos);
    delay(10);
  }
  digitalWrite(enable_1, HIGH);

  
}

// Sample mux_selecting
void loop()
{
  if (Serial.read() == 'y') {
  
  // enable all MUXs
  digitalWrite(enable_1, LOW);
  delay(50);

  //function call for polarizing water
  for (int i = 0; i < max; i++) {
    current_pos +=1;
    if (current_neg == max)
        current_neg = 0;
    if (current_pos == max)
        current_pos = 0;
    set_muxes(current_neg, current_pos);
    delay(10);
  }
  
  //loop for setting MUXs and reading voltages
  for (int i = 0; i < max; i++) {
    int temp_volt_pos = voltage_pos;
    int temp_volt_neg = voltage_neg;
    
    for (int j = 0; j < 5; j++) {
      //take voltage readings
      float voltage1 = voltageRead(adc_pins[temp_volt_pos]);
      float voltage2 = voltageRead(adc_pins[temp_volt_neg]);
      //Serial.print("Voltage1: ");
      //Serial.println(voltage1);
      //Serial.print("Voltage2: ");
      //Serial.println(voltage2);
      //store voltage readings
      voltage_readings[i][j] = voltage_diff(voltage1, voltage2);
      //Serial.print(temp_volt_pos);
      //Serial.println(temp_volt_neg);
      //increment voltage measurement positpinn
      temp_volt_neg += 1;
      //check if it reached full circle
      if (temp_volt_neg == max)
        temp_volt_neg = 0;
      temp_volt_pos += 1;
      if (temp_volt_pos == max)
        temp_volt_pos = 0;  
    }

    voltage_neg +=1;
    voltage_pos +=1;
    current_neg +=1;
    current_pos +=1;

    //check if any have reached top of circle
    if (voltage_neg == max)
        voltage_neg = 0;
    if (voltage_pos == max)
        voltage_pos = 0;
    if (current_neg == max)
        current_neg = 0;
    if (current_pos == max)
        current_pos = 0;
  //increment shift current lines to next 2 nodes 
    set_muxes(current_neg, current_pos);
  }
  // Print voltage array
  for (int k = 0; k < 8; k++) {
    for (int l = 0; l < 5; l++) {
      Serial.println(voltage_readings[k][l],7);
    }
  }
  Serial.println("----------------------------------");
  Serial.flush();
 
  //reset to original state
   digitalWrite(enable_1, HIGH);
   delay(50);
  }

  
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
  delay(75);
  float sum = 0;
  for (int i=0; i<10; i++) {
    int throwaway = analogRead(read_pin);
    int sensorvalue = analogRead(read_pin);
    //convert to floating point voltage value
    sum = sum + sensorvalue * (5.0/1023.0);
  }
  float voltage = sum/10;
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
