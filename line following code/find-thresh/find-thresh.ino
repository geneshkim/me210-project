#define LEFT_PIN         A0   
#define MID_PIN         A1   
#define RIGHT_PIN         A2   


void setup() {
  Serial.begin(9600);
  pinMode(LEFT_PIN, INPUT);
  pinMode(MID_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
}

void loop() {
  Serial.print("left:");
  Serial.println(analogRead(LEFT_PIN));
  Serial.print("mid:");
  Serial.println(analogRead(MID_PIN));
  Serial.print("rt:");
  Serial.println(analogRead(RIGHT_PIN));
}
