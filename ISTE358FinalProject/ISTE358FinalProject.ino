// ISTE 358 Jeffrey Zimmer
#include <SoftwareSerial.h>
#include <Grove_LED_Bar.h>

//LED bar attatched to D6
Grove_LED_Bar bar(7,6,0,LED_BAR_10); 

//attached to pin D2
//awake: hicell
//say command while red led lit
//if led blue, recognized the command
#define SOFTSERIAL_RX_PIN 2
#define SOFTSERIAL_TX_PIN 3

//magnetic switch attatched to D4
int magSwitch = 4;

//button attached to D8
int buttonPin = 8;

SoftwareSerial softSerial(SOFTSERIAL_RX_PIN, SOFTSERIAL_TX_PIN);
//available commands
const char *voiceBuffer[] = {
  "Turn on the light",
  "Turn off the light",
  "Play music",
  "Pause",
  "Next",
  "Previous",
  "Up",
  "Down",
  "Turn on the TV",
  "Turn off the TV",
  "Increase temperature",
  "Decrease temperature",
  "What's the time",
  "Open the door",
  "Close the door",
  "Left",
  "Right",
  "Stop",
  "Start",
  "mode 1",
  "mode 2",
  "Go"
  };
//for this I will be using mainly "mode 1","mode 2", and "go" as variables

bool scruff = false;
bool silver = false;
bool doorOpened = false;
long timerStart,timer = 0;

//how long you want to give yourself to input a command once the door opens
long timerDelay = 5;
  
void setup() {
  Serial.begin(9600);
  Serial.println("hello");
  softSerial.begin(9600);
  softSerial.listen();

  pinMode(magSwitch, INPUT);
  pinMode(buttonPin, INPUT);
  bar.begin();
  Serial.println("Starting...");
}//setup

void loop() {
  char cmd;

  //check to see if the door that the magnetic switch is attached to is opened
  if(digitalRead(magSwitch) == 0) {
    doorOpened = true;
    timerStart += millis();
    Serial.print("mag Activated ");
  }// if magSwitch is used
  
  if(softSerial.available()) {
    Serial.println("voice available...");
    cmd = softSerial.read();
    Serial.println(voiceBuffer[cmd-1]);
    
    //use the speach input to determine what to do
    if(voiceBuffer[cmd-1] == "mode 1") {
        //change the state for silver
        silver = !silver;
        Serial.print("silver is: ");
        Serial.println(silver);
        doorOpened = false;
    } 
    
    if(voiceBuffer[cmd-1] == "mode 2") {
        //change the state for scruff
        scruff = !scruff;
        Serial.print("scruff is: ");
        Serial.println(scruff);
        doorOpened = false;
    } 
    
    if(voiceBuffer[cmd-1] == "Go") {
        //see who is outside
        whoOutside();
        doorOpened = false;
        //digitalWrite(vibePin, LOW);
    }

    timerStart = 0;
  }//if softSerial is available

  //hit the button to know who is outside as well as turning off the notification that the door was opened
  if(digitalRead(buttonPin) == HIGH) {
    Serial.println("button hit...");
    whoOutside();
    doorOpened = false;
    timerStart = 0;
  } //if button state

  //buzzes or vibrates if the door is opened without a command given
  if(doorOpened && checkTime()) {
    Serial.println("Reminder activated...");
    bar.setBits(random(1024));
  }// if doorOpend and checkTime is true
  
} //loop

//will display which cat is outside
void whoOutside() {
  //clear the LED Bar
  Serial.println("LEDs off...");
  for(int j = 0; j <= 10; j++){
    bar.setLed(j,0);
  }
  
  //if silver is light up the left side grey
  Serial.println("whoOutside called...");
  if(silver) {
    Serial.println("Silver Displayed...");
    for(int i = 3; i<=6; i++){
      bar.setLed(i, 1);
    }
  } // if silver
  
  //if scruff is outside light up the right side white
  if(scruff) {
    Serial.println("Scruff Displayed...");
    for(int i = 7; i<=10; i++){
      bar.setLed(i, 1);
    }
  }// if scruff
  
  delay(10000);
  //turn off all LEDs
  Serial.println("LEDs off...");
  for(int j = 0; j <= 10; j++){
    bar.setLed(j,0);
  }
}// whoOutside

//checks to see if the time that has passed is longer than the timer delay
bool checkTime() {
  timer = millis();
  long checker = (timer-timerStart)/1000;
  if(checker >= timerDelay) {
    Serial.println("times up...");
    return true;
  }
  return false;
}//checkTime()
