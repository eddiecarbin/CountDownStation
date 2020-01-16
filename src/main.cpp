#include <Arduino.h>
#include "SoftwareSerial.h"
#include <JC_Button.h>
//#include "../lib/SoundPlayer/SoundPlayer.h"
#include "DFRobotDFPlayerMini.h"

#define LAUNCH_BUTTON_PIN 7
#define COUNTDOWN_BUTTON_PIN 5

Button launchButton(LAUNCH_BUTTON_PIN);

Button countdownButton(COUNTDOWN_BUTTON_PIN);
//SoundPlayer soundPlayer(18, 15);
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  delay(1000);

  launchButton.begin();
  countdownButton.begin();
  mySoftwareSerial.begin(9600);

  // soundPlayer.initialize();
  if (!myDFPlayer.begin(mySoftwareSerial))
  { //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  return;
  //Serial.println(F("DFPlayer Mini online."));

  //myDFPlayer.volume(30); //Set volume value. From 0 to 30

  //int fileCount = myDFPlayer.readFileCounts();
  // myDFPlayer.play(1); //Play the first mp3

  //Serial.print("file count = ");
  // Serial.println(fileCount);
}

void loop()
{

  launchButton.read();
  countdownButton.read();
  //soundPlayer.update();

  if (launchButton.wasReleased()) // if the button was released, change the LED state
  {
    Serial.println("launche button pressed");
    //soundPlayer.PlaySound(2);
  }
  if (countdownButton.wasReleased()) // if the button was released, change the LED state
  {
    Serial.println("countdown button pressed");

    //soundPlayer.PlaySound(1);
  }

  Serial.println("test");
  // put your main code here, to run repeatedly:
}