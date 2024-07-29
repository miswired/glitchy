/*
This file is licensed under the Creative Commons Attribution-Share Alike 3.0 Unported license.
https://creativecommons.org/licenses/by-sa/3.0/deed.en

Attribution: Miswired

    You are free:
        to share – to copy, distribute and transmit the work
        to remix – to adapt the work
    Under the following conditions:
        attribution – You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
        share alike – If you remix, transform, or build upon the material, you must distribute your contributions under the same or compatible license as the original. https://creativecommons.org/share-your-work/licensing-considerations/compatible-licenses
*/

        
// Defining Pins here
#define ENTER_KEY_PIN 2   
#define LOCKED_LED_PIN 13 
#define UNLOCKED_LED_PIN 12     

//Globals
bool g_button_state = false;         // variable for reading the pushbutton status
volatile bool g_unlocked = false;   //volatile so that the optimizer doesn't remove the code since it is always false

void setup() {
  // initialize pin registers
  pinMode(LOCKED_LED_PIN, OUTPUT);
  pinMode(UNLOCKED_LED_PIN, OUTPUT);
  pinMode(ENTER_KEY_PIN, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  g_button_state = digitalRead(ENTER_KEY_PIN);

  //Check to see if button is pressed and we should evaluate the unlocked variable.
  if (g_button_state == HIGH) {
    if(g_unlocked)
    {
      //unlocked is always false, should never go here unless we succesfully glitch
      unlock_system();
    }
  } else {
    // Show locked
    digitalWrite(LOCKED_LED_PIN, HIGH);
    digitalWrite(UNLOCKED_LED_PIN, LOW);
  }
}

//The function that should never get called
void unlock_system()
{
  digitalWrite(LOCKED_LED_PIN, LOW);
  digitalWrite(UNLOCKED_LED_PIN, HIGH);

  //Stay here forever
  while(true);
}
