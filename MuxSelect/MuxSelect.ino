void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  mux_select(5);
  delay(5000);
  mux_select(7);
  delay(5000);
}

void mux_select(int lane)
{
  int s[4];
  s[0] = 1 & lane;
  Serial.print("s0: ");
  Serial.println(s[0]);
  if (s[0] != 0){
    digitalWrite(13,HIGH);
  } else { 
    digitalWrite(13,LOW);
  }

  s[1] = 2 & lane;
  Serial.print("s1: ");
  Serial.println(s[1]);
  if (s[1] != 0){
    digitalWrite(12,HIGH);
  } else { 
    digitalWrite(12,LOW);
  }

  s[2] = 4 & lane;
  Serial.print("s2: ");
  Serial.println(s[2]);
  if (s[2] != 0){
    digitalWrite(11,HIGH);
  } else { 
    digitalWrite(11,LOW);
  }

  s[3] = 8 & lane;
  Serial.print("s3: ");
  Serial.println(s[3]);
  if (s[3] != 0){
    digitalWrite(10,HIGH);
  } else { 
    digitalWrite(10,LOW);
  }
}
