// Source                       ::     DaMaze.ino
// Description                  ::     autonomous maze solving vehicle
//                              ::     This program will grow in functionality through a series of project phases.
// Original Program Template    ::     adapted from J.E.Marriott / March 2017


// Phase 0        
// Modified by      ::   R.Barriault / April 2018
//                        6.a. Testing drive motor control
// Phase 1
// Modified by     ::     Your Name / Date                              <----------------- IMPORTANT... Complete these with each phase! -----------------
//                        6.b. Testing FWD facing sensors  
// Phase 2
// Modified by     ::     Your Name / Date
//                        6.c. Testing "non-drive" motors
// Phase 3
// Modified by     ::     Your Name / Date
//                        6.d. Testing bottom facing sensors
// Phase 4
// Modified by     ::     Your Name / Date
//                        6.e. Testing switch
// Phase 5
// Modified by     ::     Your Name / Date
//                        8. Maze code; fwd sensors only
// Phase 6
// Modified by     ::     Your Name / Date
//                        9. Maze code; fwd and bottom sensors, return to start
// Phase 7
// Modified by     ::     Your Name / Date
//                        10. ; 11. Seek and grasp object at end of maze and return

//*******************************************************************************************
// It is considered a "best practice" to map out all your pins at the top of the program,
// even the ones that are currently not in use.  Takes the guesswork out of "what is what"!
//
// Using "defines" instead of "declaring variables" reduces memory usage.
// https://www.arduino.cc/en/reference/define
// GooglE "define vs constant variables in Arduino". 
//
// References created with "#define" are called CONSTANTS. It is common to signify CONSTANTS as ALL UPPER CASE.
//*******************************************************************************************

// Digital Pin Assignments
#define DO_NOT_USE_0            0
#define DO_NOT_USE_1            1
#define NOT_USED_2              2
#define USEDBYSERVO             3   // myServo attached to pin 3 , mount Front IR Sensor on this servo  
#define R_MOTOR_DIREC1          4   // IN1 digitalWrite LOW forward, HIGH reverse
#define R_MOTOR_DIREC2          5   // IN2 digitalWrite HIGH forward, LOW reverse
#define R_MOTOR_SPEED           6   // ENA analogWrite from 0 thru 255
#define L_MOTOR_DIREC1          7   // IN3 digitalWrite LOW forward, HIGH reverse
#define L_MOTOR_DIREC2          8   // IN4 digitalWrite HIGH forward, LOW reverse
#define NOT_USED_9              9   // PWM disabled for pins 9 and 10 due to Servo library
#define NOT_USED_10             10
#define L_MOTOR_SPEED           11  // ENB analogWrite from 0 thru 255
#define MOTOR_REV_SW            12  // reverse Motors when HI 
#define MOTOR_STOP_SW           13  // Brake Motors when HI  

// Analog Pin Assignments
// Did you know that, if necessary, you can use Analog Pins as Digital?
// The numbers would just continue from above, so A0 is 14 , A1 is 15 , etc.
#define POT                     A0  // pot to control motor speed
#define IR_FRONT                A1  //Front IR Sensor analog input
#define NOT_USED_A2             A2
#define NOT_USED_A3             A3
#define NOT_USED_A4             A4
#define NOT_USED_A5             A5

// CONSTANTS for motor controller commands makes actions easier to remember later on
#define STOP      0
#define MAXSPEED  255

//*******************************************************************************************
// Libraries
// https://www.arduino.cc/en/guide/libraries
// How to install from zip... https://www.arduino.cc/en/guide/libraries#toc4

#include<Servo.h>
Servo myServo;

//*******************************************************************************************
// Global Variables are available to every function in the program.
// Frowned on by traditionally trained programmers, they are widely accepted in the Arduino
// community thanks to their simplicity.  This program uses them for that reason. 
// https://www.arduino.cc/en/reference/scope
//
//*******************************************************************************************

// Set initial speed to 60% of maximum.
// Note how the use of the constant MAXSPEED allows the programmer to focus on the
// concept rather than trying to remember some obscure number.
// https://www.arduino.cc/en/reference/int
  int reducedSpeed = MAXSPEED * .60;
  
// Set initial vehicle direction
// https://www.arduino.cc/reference/en/language/variables/data-types/bool/
 bool Forward   = true;
 bool leftWall = false;
 bool rightWall = false;
 bool frontWall = false;
 int irFrontVal;  // Global Variable: Contains reading from IR sensor
 int pwmOutput;   // Global Variable: sets drive motor speed

//*******************************************************************************************
// The setup() function is called when a sketch starts. 
// Use it to initialize variables, pin modes, start communications, etc. 
// The setup function will only run once, after each powerup or reset of the Arduino board.
//*******************************************************************************************

void setup() {
   // Open serial port for USB communications
  // https://www.arduino.cc/en/Serial/Begin
  // Comment out this line when using 1Sheeld   <----------------- IMPORTANT -----------------
  Serial.begin(9600);

  // Set pinModes for all active pins    <----------------- IMPORTANT -----------------
  // https://www.arduino.cc/en/Reference/pinMode
  pinMode ( R_MOTOR_DIREC1 , OUTPUT );
  pinMode ( R_MOTOR_DIREC2 , OUTPUT );
  pinMode ( R_MOTOR_SPEED , OUTPUT );
  pinMode ( L_MOTOR_DIREC1 , OUTPUT );
  pinMode ( L_MOTOR_DIREC2 , OUTPUT );
  pinMode ( L_MOTOR_SPEED , OUTPUT );
  pinMode ( MOTOR_REV_SW, INPUT );
  pinMode ( IR_FRONT    ,  INPUT  );

  myServo.attach(3);
  myServo.write(90);
  delay(300);
 }
//*******************************************************************************************
// Ideally, loop() should contain a minimum, or be completely devoid of, "raw code".  
// At a glance, it should convey the essence of the core algorithm of the sketch.
// https://www.arduino.cc/en/reference/loop
//*******************************************************************************************

void loop ( void ) {     pwmOutput = reducedSpeed;
     leftWall = false;
     rightWall = false;
     forward();
  getSensorReadings ( );        // look ahead
  Serial.print("front: ");
  Serial.println(irFrontVal);
  if (irFrontVal > 400) {                         // wall too close ahead?
    allStop();
     delay(500); 
                                // look left
    myServo.write(10);
    delay(500); 
    getSensorReadings ( );
     delay(500); 
    int irLeft = irFrontVal;
    Serial.print("---left: ");
    Serial.print(irFrontVal);
    if (irFrontVal > 400) {                       // wall too close on left?
    leftWall = true;
    }
                                // look right
    myServo.write(170);
    delay(500); 
    getSensorReadings ( );
     delay(500); 
    int irRight = irFrontVal;
    Serial.print("  ----right: ");
    Serial.println(irFrontVal);
    if (irFrontVal > 400) {                       // wall too close on right?
    rightWall = true;
    }
    if (leftWall== true && rightWall== true) {    // stuck in a corner, so backup
      reverse();
    }
    else if (leftWall == true && rightWall == false){                  // wall on left only so go right?
     right(); 
    }
   else if (leftWall == false && rightWall == true){                 // wall on right only so go left?
     left();                                   
    }
   else if (leftWall == false && rightWall == false){
     if (irLeft >= irRight) {
      right();
     }
      else {
        left();
      }
     }
    delay(500);
   }
  myServo.write(90);             // look ahead
  delay(300);

  // To adhere to the intended algorithm, NOTHING ELSE should be added to loop() 
  // without discussing with the course instructor first!

  updateTimers ( );

}


//*******************************************************************************************
//
// Main Functions
// https://www.arduino.cc/en/Reference/FunctionDeclaration
//
//*******************************************************************************************

void getSensorReadings ( void ) {
  irFrontVal = analogRead ( IR_FRONT );
  delay(200);


}

void updateTimers ( void ) {
// Responsibilities :: Update all active timers

   // 4U2Do ... if you have any blinker (or other) timers running, this is a nice central location to update them! 


}


void allStop ( void ) { 
// Responsibilities :: Motors All Stop 
  digitalWrite(R_MOTOR_DIREC1, LOW); 
  digitalWrite(R_MOTOR_DIREC2, LOW);
  digitalWrite(L_MOTOR_DIREC1, LOW); 
  digitalWrite(L_MOTOR_DIREC2, LOW);
  delay(200);  
}


void forward ( void ) {
// Responsibilities :: Motors Forward
  digitalWrite(R_MOTOR_DIREC1, HIGH); 
  digitalWrite(R_MOTOR_DIREC2, LOW);
  digitalWrite(L_MOTOR_DIREC1, HIGH); 
  digitalWrite(L_MOTOR_DIREC2, LOW);
  analogWrite(R_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Right Motor 
  analogWrite(L_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Left Motor 
}


void reverse ( void ) {
// Responsibilities :: Motors Reverse
  digitalWrite(R_MOTOR_DIREC1, LOW); 
  digitalWrite(R_MOTOR_DIREC2, HIGH); 
  digitalWrite(L_MOTOR_DIREC1, LOW); 
  digitalWrite(L_MOTOR_DIREC2, HIGH);
  analogWrite(R_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Right Motor 
  analogWrite(L_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Left Motor 
  delay(200);
}


void left ( void ) {
// Responsibilities :: Motors Left pivot at (leftSpeed, 0)
  digitalWrite(R_MOTOR_DIREC1, HIGH); 
  digitalWrite(R_MOTOR_DIREC2, LOW);
  digitalWrite(L_MOTOR_DIREC1, LOW); 
  digitalWrite(L_MOTOR_DIREC2, HIGH); 
  analogWrite(R_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Right Motor 
  analogWrite(L_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Left Motor  
}

void right ( void ) {
  digitalWrite(R_MOTOR_DIREC1, LOW); 
  digitalWrite(R_MOTOR_DIREC2, HIGH); 
  digitalWrite(L_MOTOR_DIREC1, HIGH); 
  digitalWrite(L_MOTOR_DIREC2, LOW);
  analogWrite(R_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Right Motor 
  analogWrite(L_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Left Motor    
}



//*******************************************************************************************
//
//  Misc Functions
//  Anything else should go in this section
//
//*******************************************************************************************


