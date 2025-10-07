
// Variables

// Map actions to Arduino pin numbers
int pressurize= 7;
int release = 4;
int vacuum = 12;
int button = 2;

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
  pinMode(pressurize, OUTPUT);
  pinMode(release, OUTPUT);
  pinMode(vacuum, OUTPUT);
  pinMode(button, INPUT_PULLUP);

}

// Loop
void loop() {
  // put your main code here, to run repeatedly:

  // Serial test

  // Wait for serial info
  while (!Serial.available()); 

  // Read input
  String inputString = Serial.readStringUntil('\n');  // read until newline

  if (inputString == "test") {
    while (!Serial.available()); 
    serial_test_var = Serial.readStringUntil('\n').toInt();
    //serial_test_var = Serial.readString().toInt(); 
    Serial.println(serial_test_var + 1);
  }

  else if (inputString == "automated") {
    // Automated arduino run code:

    // Wait for an input (button pressed) to begin automated run
    while(digitalRead(button)==LOW){}       // press button to initialize test once pipe is attached
    //leak check inital pipe attachment
    digitalWrite(vacuum,HIGH);          
    delay(500);

    // Wait until button press to close leak checker
    while(digitalRead(button)==LOW){}       
    //close leak checker
    digitalWrite(vacuum,LOW);               
    delay(500);

    //press button to pressurize pipe
    while(digitalRead(button)==LOW){}    
    //pressurize pipe for the first time @ 472 PSI
    digitalWrite(pressurize,HIGH);          
    delay(5000);
    digitalWrite(pressurize,LOW);
    delay(600000);                          //hold at pressure for 10 minutes

    //press button to start the looping for building up pressure
    while(digitalRead(button)==LOW){}       
    // Once button is pressed:
    while(a!=x){                            //loop through all the pressure stages before max pressure (pressure adjusted manually) (1175-1410-1645-1880-2115)
      digitalWrite(pressurize,HIGH);        //pressurize pipe
      delay(5000);
      digitalWrite(pressurize,LOW);
      delay(t);                             //hold at pressure
      while(digitalRead(button)==LOW){}     //wait for button input to begin next stage (manually adjust pressure regulator before pressing)
      a=a+1;
    }

    digitalWrite(pressurize,HIGH);          //pressurize pipe at max pressure (2350)
    delay(5000);
    digitalWrite(pressurize,LOW);
    delay(1800000);                         //30 minutes
    digitalWrite(pressurize,HIGH);          //pressurize pipe (2115)
    delay(5000);
    digitalWrite(pressurize,LOW);
    delay(t);                               //hold at pressure
    digitalWrite(release,HIGH);             //release
    delay(5000);
    digitalWrite(release,LOW);

    //wait for button input to begin next stage (manually adjust pressure regulator before pressing)
    while(digitalRead(button)==LOW){}       
    digitalWrite(vacuum,HIGH);              //These three lines are the leak checker code, move them to the loop if we need to

    while(digitalRead(button)==LOW){}
    delay(500);
    digitalWrite(vacuum,LOW);
  } // End automated run

  else {
    Serial.println("Error: Unknown command");
  }

} // End loop




