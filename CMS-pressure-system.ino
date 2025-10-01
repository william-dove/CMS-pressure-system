void setup() {
  // put your setup code here, to run once:
  int pressurize= 7;
  int release = 4;
  int vacuum = 12;
  int button = 2;
  pinMode(pressurize, OUTPUT);
  pinMode(release, OUTPUT);
  pinMode(vacuum, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int pressurize= 7;
  int release = 4;
  int vacuum = 12;
  int button = 2;
  int t = 300000;                         // number of time at each lower stage (in ms) (5 mins)
  int x = 5;                              //number of stages before max
  int a = 0;
  while(digitalRead(button)==LOW){}       // press button to initialize test once pipe is attached
  digitalWrite(vacuum,HIGH);              //leak check inital pipe attachment
  delay(500);
  while(digitalRead(button)==LOW){}       //press button to close leak checker
  digitalWrite(vacuum,LOW);               //close leak checker
  delay(500);
  while(digitalRead(button)==LOW){}       //press button to pressurize pipe
  digitalWrite(pressurize,HIGH);          //pressurize pipe for the first time @ 472 PSI
  delay(5000);
  digitalWrite(pressurize,LOW);
  delay(600000);                          //hold at pressure for 10 minutes
  while(digitalRead(button)==LOW){}       //press button to start the looping for building up pressure
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
  while(digitalRead(button)==LOW){}       //wait for button input to begin next stage (manually adjust pressure regulator before pressing)
  digitalWrite(vacuum,HIGH);              //These three lines are the leak checker code, move them to the loop if we need to
  while(digitalRead(button)==LOW){}
  delay(500);
  digitalWrite(vacuum,LOW);
}