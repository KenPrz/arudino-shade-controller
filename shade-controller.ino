#define stepPin 2
#define dirPin 5

 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  const int TEMP_THRESHOLD = NULL;
  const int HUM_THRESHOLD = NULL;
}
void loop() {
  
  delay(15000);
}

void pullShade(){
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction (change on testing)
  for(int x = 0; x < (10*200); x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1200);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1200); 
  }
}

void retractShade(){
    digitalWrite(dirPin,LOW); //Changes the rotations direction (change on testing)
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < (10*200); x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(1500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(1500);
  }
  delay(1000);
}