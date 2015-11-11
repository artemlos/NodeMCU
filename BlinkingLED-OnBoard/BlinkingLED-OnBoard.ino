
// NOTE:  each pin will receive approx 3.3 V (by experiment).

// this will make the LED close to RST turn on. same behaviour as for D4.
#define D0 16
#define D1 5
// the LED close to AI label.
// when AI label is HIGH, the output from pin 4 will be low and vice versa.
#define D4 2

int pin = D1;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);
  delay(1000);
}
