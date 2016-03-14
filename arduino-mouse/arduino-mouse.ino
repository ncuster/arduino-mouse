/*
    arduino-mouse

 */

#include <Button.h>
#include <Mouse.h>

const int onOffButton = 7;     // switch to turn on an-d off mouse control (this is our right-mouse click for now)
const int leftMouseButton = 8; // switch to do a leftmouse-click
Button button = Button(onOffButton, BUTTON_PULLUP_INTERNAL, true, 50);

const int xAxisLeft  = 12;
const int xAxisRight = 10;
const int yAxisUp    = 11;
const int yAxisDown  = 9;
const int ledPin = 13;          // our built in LED 

// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value


boolean mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

void setup() {
    // a little hack so we can set output in setup()
    Serial.begin(9600);
    while (!Serial) ;
    
//    pinMode(onOffButton, INPUT_PULLUP);     // the switch pin
    pinMode(ledPin, OUTPUT);                // the LED pin
    // take control of the mouse:
    Mouse.begin();
}

void loop() {
    // Check whether we wanted to turn on/off the mouse capability
    if( button.uniquePress() ) {
        if ( mouseIsActive ) {
            digitalWrite(ledPin, LOW);
        } else {
            digitalWrite(ledPin, HIGH);          
        }

        mouseIsActive = !mouseIsActive;
        Serial.print("Button Pressed mouseActive: ");  Serial.println(mouseIsActive);
    }

    
    // if the mouse control state is active, move the mouse:
    if ( mouseIsActive ) {
        //Mouse.move(xReading, yReading, 0);
        Serial.println("moving to 0,0,0");
        Mouse.move(1,6,0);
    }

}

/*
void loop() {
    // read the switch:
    int switchState = digitalRead(onOffButton);
    Serial.print(switchState); Serial.print(" "); Serial.println(lastSwitchState);
    while ( true) ;

    // if it's changed and it's high, toggle the mouse state:
    if ( switchState != lastSwitchState ) {
        if ( switchState == HIGH ) {
            mouseIsActive = !mouseIsActive;
            digitalWrite(ledPin, mouseIsActive);
//            Serial.println("Mouse is active");
        }

    }
    
    lastSwitchState = switchState;


    // save switch statswitchState;
    // read and scale the two axes:
    int xReading = readAxis(A0);
    int yReading = readAxis(A1);
  
    // if the mouse control state is active, move the mouse:
    if (mouseIsActive) {
        Mouse.move(xReading, yReading, 0);
    }

    // read the mouse button and click or not click:
    // if the mouse button is pressed:
    if (digitalRead(leftMouseButton) == HIGH) {
        // if the mouse is not pressed, press it:
        if (!Mouse.isPressed(MOUSE_LEFT)) {
            Mouse.press(MOUSE_LEFT);
        }
    }
    // else the mouse button is not pressed:
    else {
        // if the mouse is pressed, release it:
        if (Mouse.isPressed(MOUSE_LEFT)) {
            Mouse.release(MOUSE_LEFT);
        }
  
    }
   delay(responseDelay);
 }
*/
/*
  reads an axis (0 or 1 for x or y) and scales the
 analog input range to a range from 0 to <range>
 */

int readAxis(int thisAxis) {
  // read the analog input:
  int reading = digitalRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the
  // rest position threshold,  use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

