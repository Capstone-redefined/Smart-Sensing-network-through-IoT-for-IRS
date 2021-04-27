const int DOOR_SENSOR_PIN =20; // teensy pin connected to door sensor's pin
const int RELAY_PIN       =15;  // teensy pin connected to the IN pin of relay
const int RPI_SIGNAL_PIN  =17 ;  //teensy pin that recieve the signal from arduino
int rpiState ;
int doorState;

void setup() 
{
  Serial.begin(9600);                     // initialize serial
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set teensy pin to input pull-up mode
  pinMode(RELAY_PIN, OUTPUT);             // set teensy pin to output mode
  pinMode(RPI_SIGNAL_PIN, INPUT_PULLUP);  //set teensy pin to in put pull-up mode
}

void loop()
{
  doorState = digitalRead(DOOR_SENSOR_PIN); // read state
  while(doorState == HIGH)
  {
    Serial.println("The door is open");
    delay(2000);
    digitalWrite(RELAY_PIN, HIGH);
    rpiState = digitalRead(RPI_SIGNAL_PIN);
    while(rpiState == LOW)
    {
      delay(3000);
    }
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("switching off the light");
    delay(2000);
     }
}
    
  
