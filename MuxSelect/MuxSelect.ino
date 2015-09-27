// physical to logical map of GPIOs
int pin_map[16] = 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 ,11 , 14, 13, 14, 15, 16;

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
  pinMode(pin_map[8], OUTPUT);
  pinMode(pin_map[9], OUTPUT);
  pinMode(pin_map[10], OUTPUT);
  pinMode(pin_map[11], OUTPUT);
  pinMode(pin_map[12], OUTPUT);
  pinMode(pin_map[13], OUTPUT);
  pinMode(pin_map[14], OUTPUT);
  pinMode(pin_map[15], OUTPUT);

  int enable_0 = 52;
  int enable_1 = 51;
  int enable_2 = 50;
  int enable_3 = 49;
}

// Sample mux_selecting
void loop()
{
  //loop for setting MUXs and reading voltages
  for (int i = 0; i < 8; i++) {
  	  //temp voltage positpinn values to save original positions
      int temp_volt_pos = voltage_pos;
      int temp_volt_neg = voltage_neg;
      
      for (int j = 0; j < 5; j++) {
      //take voltage readings
      float voltage1 = voltageRead(voltage_read_1);
      float voltage2 = voltageRead(voltage_read_2);
      //store voltage readings
      voltage_readings[i][j] = voltage_diff(voltage1, voltage2);
      //increment voltage measurement positpinn
      temp_volt_neg += 1;
      //check if it reached full circle
      if (temp_volt_neg == 8)
        temp_volt_neg = 0;
      temp_volt_pos += 1;
      if (temp_volt_pos == 8)
        temp_volt_pos = 0;
      //change select line for voltage measurement positpinn
      mux_select(temp_volt_neg, 3);
      mux_select(temp_volt_pos, 2);     
    }
    voltage_neg +=1;
    voltage_pos +=1;
    current_neg +=1;
    current_pos +=1;
    set_muxes(current_pos, current_neg, voltage_pos, voltage_neg);
    delay(3000);
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
      digitalWrite(pin_map[mux * 4 + i],HIGH);
    } else {
      digitalWrite(pin_map[mux * 4 + i],LOW);
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

float voltageRead(int read_pin)
{
  //Read input analog pin
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
