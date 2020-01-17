#include <Arduino.h>
#include "SoftwareSerial.h"
#include <JC_Button.h>
#include "../lib/SoundPlayer/SoundPlayer.h"
#include "../lib/FSM/FSM.h"
#include "../lib/PotMonitor/PotMonitor.h"
#include "DFRobotDFPlayerMini.h"

#define LAUNCH_BUTTON_PIN 7
#define COUNTDOWN_BUTTON_PIN 5

#define GO_TO_COUNTDOWN_STATE 1
#define GO_TO_LAUNCH_STATE 2
// #define POT_PIN A5
Button launchButton(LAUNCH_BUTTON_PIN);

Button countdownButton(COUNTDOWN_BUTTON_PIN);
SoundPlayer soundPlayer(18, 15);

State StateDoNothing(NULL, NULL, NULL);
Fsm fsm(&StateDoNothing);
PotMonitor volumePot(A5, 12);

bool _soundIsPlaying = false;

long map2(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void OnCountDownStateEnter()
{
}

void OnCountDownStateUpdate()
{

  if (!_soundIsPlaying)
  {
    if (launchButton.wasReleased()) // if the button was released, change the LED state
    {
      Serial.println("Do Nothing Cant press button");
      if (soundPlayer.isPlaying() == false)
        soundPlayer.PlaySound(3);
    }

    if (countdownButton.wasReleased()) // if the button was released, change the LED state
    {
      Serial.println("countdown button pressed");
      _soundIsPlaying = true;
      soundPlayer.PlaySound(1);
    }
  }
  else if (soundPlayer.isPlaying() == false)
  {
    /// switcht o other state
    _soundIsPlaying = false;
    fsm.trigger(GO_TO_LAUNCH_STATE);
  }
}

void OnLaunchStateEnter()
{
}

void OnLaunchStateUpdate()
{
  if (!_soundIsPlaying)
  {
    if (launchButton.wasReleased()) // if the button was released, change the LED state
    {
      Serial.println("launch");
      _soundIsPlaying = true;
      soundPlayer.PlaySound(2);
    }
    if (countdownButton.wasReleased()) // if the button was released, change the LED state
    {
      Serial.println("countdown");
      if (soundPlayer.isPlaying() == false)
        soundPlayer.PlaySound(3);
    }
  }
  else if (soundPlayer.isPlaying() == false)
  {
    /// switcht o other state
    _soundIsPlaying = false;
    fsm.trigger(GO_TO_COUNTDOWN_STATE);
  }
}

State CountDownState(&OnCountDownStateEnter, &OnCountDownStateUpdate, NULL);
State LaunchState(&OnLaunchStateEnter, &OnLaunchStateUpdate, NULL);

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  delay(1000);
  launchButton.begin();
  countdownButton.begin();
  soundPlayer.initialize();
  //soundPlayer.PlaySound(1);

  fsm.add_transition(&CountDownState, &LaunchState,
                     GO_TO_LAUNCH_STATE, NULL);
  fsm.add_transition(&LaunchState, &CountDownState,
                     GO_TO_COUNTDOWN_STATE, NULL);

  fsm.goToState(&CountDownState);
}

void loop()
{

  if (volumePot.hasUpdated())
  {
    long volume = map2(volumePot.getValue(), 0, 1023, 0, 30);
    Serial.println(volume);
    soundPlayer.volume(volume);
  }

  launchButton.read();
  countdownButton.read();
  soundPlayer.update();
  fsm.run_machine();
  delay(100);
}
