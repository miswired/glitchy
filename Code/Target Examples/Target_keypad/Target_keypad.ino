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
#define ENTER_KEY_PIN       2   
#define KEY_1_PIN           6
#define KEY_2_PIN           5
#define KEY_3_PIN           4

#define LOCKED_LED_PIN      12 
#define UNLOCKED_LED_PIN    11
#define STATUS_LED_PIN      13     

//Globals
bool g_button_state = false;         // variable for reading the pushbutton status
volatile bool g_unlocked = false;   //volatile so that the optimizer doesn't remove the code since it is always false
bool g_was_the_right_key_pressed = false;

void setup() {
  // initialize pin registers
  pinMode(LOCKED_LED_PIN, OUTPUT);
  pinMode(UNLOCKED_LED_PIN, OUTPUT);
  pinMode(ENTER_KEY_PIN, INPUT_PULLUP);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  //Note, this example expects the pins to be pulled high and low, no pullup needed.
  pinMode(KEY_1_PIN, INPUT_PULLUP);
  pinMode(KEY_2_PIN, INPUT_PULLUP);
  pinMode(KEY_3_PIN, INPUT_PULLUP);


  // Show locked
  digitalWrite(LOCKED_LED_PIN, HIGH);
  digitalWrite(UNLOCKED_LED_PIN, LOW);

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  Serial.println("Booted, waiting for key press\n");
}

void loop() {
  if(!digitalRead(KEY_1_PIN))
  {
    g_was_the_right_key_pressed = false;
    Serial.println("Key 1 Pressed\n");
  }

    if(!digitalRead(KEY_2_PIN))
  {
    g_was_the_right_key_pressed = false;
    Serial.println("Key 2 Pressed\n");
  }

  //In this example, we only care if key 3 is pressed, because it's the right key.
  if(!digitalRead(KEY_3_PIN))
  {
    g_was_the_right_key_pressed = true;
    //digitalWrite(STATUS_LED_PIN, HIGH);
    //Do some extra operation to simulate a different execution path, this is NOT realilistic, just for testing
    ///float testing=1928.12*272727/0.5782+5;
    float prime = ({int p=2,n=0,f;for(;n</**/100/**/;p++)for(f=2;(f<p||!++n)&&p%f++;);--p;});
    Serial.println(prime);
    delayMicroseconds(100);
    Serial.println("Key 3 Pressed\n");
    //digitalWrite(STATUS_LED_PIN, LOW);
  }


  

  //Check to see if button is pressed and we should evaluate the password
  if (!digitalRead(ENTER_KEY_PIN)) {
    Serial.println("Enter Key Pressed\n");
    if(g_was_the_right_key_pressed)
    {
      unlock_system();
    }
  } 
}


void unlock_system()
{
  digitalWrite(LOCKED_LED_PIN, LOW);
  digitalWrite(UNLOCKED_LED_PIN, HIGH);
}
