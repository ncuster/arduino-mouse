/*
    arduino-mouse

 */

#include <Button.h>
#include <Mouse.h>

const int rightMouseButton = 7;     // switch to turn on an-d off mouse control (this is our right-mouse click for now)
const int leftMouseButton = 8; // switch to do a leftmouse-click
const int xAxisLeft  = 10;
const int xAxisRight = 12;
const int yAxisUp    = 11;
const int yAxisDown  = 9;
const int ledPin     = 13;        // our built in LED 
const int mouseSpeed = 5;
const int responseDelay = 10;     // response delay of the mouse, in ms

Button rightButton = Button(rightMouseButton, BUTTON_PULLUP_INTERNAL, true, 50);
Button leftButton = Button(leftMouseButton, BUTTON_PULLUP_INTERNAL, true, 50);

boolean mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

void setup() {
    // a little hack so we can set output in setup()
    Serial.begin(9600);
    while (!Serial) ;
    
    pinMode(ledPin, OUTPUT);                // the LED pin
    pinMode(xAxisLeft, INPUT_PULLUP);
    pinMode(yAxisUp, INPUT_PULLUP);
    pinMode(xAxisRight, INPUT_PULLUP);
    pinMode(yAxisDown, INPUT_PULLUP);
    // take control of the mouse:
    Mouse.begin();
}

void loop() {
    // Check whether we wanted to turn on/off the mouse capability
    if( rightButton.uniquePress() ) {
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
        processMouseMovement();
    }
  
    // a delay so the mouse doesn't move too fast:
    delay(responseDelay);
}

// This assumes the mouse is active, that the arduino is actively
// sending mouse events (and not your computer's).
void processMouseMovement() {
    int leftState = digitalRead(xAxisLeft);
    int upState = digitalRead(yAxisUp);
    int rightState = digitalRead(xAxisRight);
    int downState = digitalRead(yAxisDown);
    //int clickState = digitalRead(leftButton);
  
    // calculate the movement distance based on the button states:
    int  xDistance = (leftState - rightState) * mouseSpeed;
    int  yDistance = (upState - downState) * mouseSpeed;
  
    // if X or Y is non-zero, move:
    if ( (xDistance != 0) || (yDistance != 0) ) {
        Mouse.move(xDistance, yDistance, 0);
    }
  
    // if the mouse button is pressed:
    if ( leftButton.uniquePress() ) {
    //if (clickState == HIGH) {
        // if the mouse is not pressed, press it:
        if ( !Mouse.isPressed(MOUSE_LEFT) ) {
            Mouse.press(MOUSE_LEFT);
        }
    } else {
        // if the mouse is pressed, release it:
        if ( Mouse.isPressed(MOUSE_LEFT) ) {
            Mouse.release(MOUSE_LEFT);
        }
    }
}
