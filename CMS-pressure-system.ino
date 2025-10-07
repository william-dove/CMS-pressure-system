
// Variables

// Map actions to Arduino pin numbers
const int PRESSURIZE = 7;
const int RELEASE = 4;
const int VACUUM = 12;
const int BUTTON = 2;

//Set up variables 
int serial_test_var;
int t = 300000;                         // number of time at each lower stage (in ms) (5 mins)
int x = 5;                              //number of stages before max
int a = 0;

// Setup
void setup() {
  // put your setup code here, to run once:

  // For serial communication:

  Serial.begin(115200);   // Start serial communication at this baudrate
  Serial.setTimeout(100);   // Set timeout when waiting for serial input to 100 ms

  // Set pins as input or output
  pinMode(PRESSURIZE, OUTPUT);
  pinMode(RELEASE, OUTPUT);
  pinMode(VACUUM, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

}

// Loop
void loop() {
  // put your main code here, to run repeatedly:

  // Wait for serial info
  while (!Serial.available()); 

  // Read input
  String inputString = Serial.readStringUntil('\n');  // read until newline
  inputString.trim();

  if (inputString == "test") {
    while (!Serial.available()); 
    serial_test_var = Serial.readStringUntil('\n').toInt();
    Serial.println(serial_test_var + 1);
  }

  else if (inputString == "open vacuum") {
    digitalWrite(VACUUM, HIGH);   
    Serial.println("Leak checker opened.");
  }
  else if (inputString == "close vacuum") {
    digitalWrite(VACUUM, LOW);
    Serial.println("Leak checker closed.");
  }
  else if (inputString == "open pressure") {
    digitalWrite(PRESSURIZE, HIGH);
    Serial.println("Pressure valve opened.");
  }
  else if (inputString == "close pressure") {
    digitalWrite(PRESSURIZE, LOW);
    Serial.println("Pressure valve closed.");
  }
  else if (inputString == "open release") {
    digitalWrite(RELEASE, HIGH);
    Serial.println("Release valve opened.");
  }
  else if (inputString == "close release") {
    digitalWrite(RELEASE, LOW);
    Serial.println("Release valve closed.");
  }

  else if (inputString == "automated") {
    // Automated arduino run code:
    Serial.println("Starting automated sequence.");
    a = 0; // reset stage counter

    // Wait for an input (button pressed) to begin automated run
    while(digitalRead(BUTTON)==LOW){}       // press button to initialize test once pipe is attached
    //leak check inital pipe attachment
    digitalWrite(VACUUM,HIGH);          
    delay(500);

    // Wait until button press to close leak checker
    while(digitalRead(BUTTON)==LOW){}       
    //close leak checker
    digitalWrite(VACUUM,LOW);               
    delay(500);

    //press button to pressurize pipe
    while(digitalRead(BUTTON)==LOW){}    
    //pressurize pipe for the first time @ 472 PSI
    digitalWrite(PRESSURIZE,HIGH);          
    delay(5000);
    digitalWrite(PRESSURIZE,LOW);
    delay(600000);                          //hold at pressure for 10 minutes

    //press button to start the looping for building up pressure
    while(digitalRead(BUTTON)==LOW){}       
    // Once button is pressed:
    while(a!=x){                            //loop through all the pressure stages before max pressure (pressure adjusted manually) (1175-1410-1645-1880-2115)
      digitalWrite(PRESSURIZE,HIGH);        //pressurize pipe
      delay(5000);
      digitalWrite(PRESSURIZE,LOW);
      delay(t);                             //hold at pressure
      while(digitalRead(BUTTON)==LOW){}     //wait for button input to begin next stage (manually adjust pressure regulator before pressing)
      a=a+1;
    }

    digitalWrite(PRESSURIZE,HIGH);          //pressurize pipe at max pressure (2350)
    delay(5000);
    digitalWrite(PRESSURIZE,LOW);
    delay(1800000);                         //30 minutes
    digitalWrite(PRESSURIZE,HIGH);          //pressurize pipe (2115)
    delay(5000);
    digitalWrite(PRESSURIZE,LOW);
    delay(t);                               //hold at pressure
    digitalWrite(RELEASE,HIGH);             //release
    delay(5000);
    digitalWrite(RELEASE,LOW);

    //wait for button input to begin next stage (manually adjust pressure regulator before pressing)
    while(digitalRead(BUTTON)==LOW){}       
    digitalWrite(VACUUM,HIGH);              //These three lines are the leak checker code, move them to the loop if we need to

    while(digitalRead(BUTTON)==LOW){}
    delay(500);
    digitalWrite(VACUUM,LOW);
  } // End automated run

  else {
    Serial.println("Error: Unknown command");
  }

} // End loop




