int temperaturePin = A0; //Set the temperature pin input float temperature; //Set temperature variable

boolean debug = true; //For serial communication set debug to true, for faster code set debug to false

long duration, cm; //Set time and cm for distance sensing

int trig = 10, echo = 9; //Set pins for trig and echo

void setup() {
  if (debug) {
    Serial.begin(9600);
  }
}

void loop() {
  temperature = (getVoltage(temperaturePin) - 0.5) * 100;

  if (debug) {
    Serial.println(temperature);
  }

  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  cm = microsecondsToCentimeters(duration, temperature); if (debug) {
    Serial.println(cm);
    Serial.println("cm");
  }
}

float getVoltage(int pin) {
  return (analogRead(pin) * .004882814); //Converting from 0 to 1024 to 0 to 5v }

  long microsecondsToCentimeters(long microseconds, long temp) {
    return (microseconds * (331.3 + 0.606 * temp)) / 2; //Multiplying the speed of sound through a certain temperature of air by the //length of time it takes to reach the object and back, divided by two }
