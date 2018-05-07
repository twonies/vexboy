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
#define NOT_USED_3              3
#define R_MOTOR_DIREC1          4   // IN1 digitalWrite LOW forward, HIGH reverse
#define R_MOTOR_DIREC2          5   // IN2 digitalWrite HIGH forward, LOW reverse
#define R_MOTOR_SPEED           6   // ENA analogWrite from 0 thru 255
#define L_MOTOR_DIREC1          7   // IN3 digitalWrite LOW forward, HIGH reverse
#define L_MOTOR_DIREC2          8   // IN4 digitalWrite HIGH forward, LOW reverse
#define L_MOTOR_SPEED           9   // ENB analogWrite from 0 thru 255
#define NOT_USED_8              8
#define NOT_USED_9              9
#define NOT_USED_10             10
#define NOT_USED_11             11
#define MOTOR_Left_SW            12  // Go Left when HI 
#define MOTOR_Right_SW           13  // Go Right when HI

// Analog Pin Assignments
// Did you know that, if necessary, you can use Analog Pins as Digital?
// The numbers would just continue from above, so A0 is 14 , A1 is 15 , etc.
#define POT                     A0  // pot to control motor speed
#define NOT_USED_A1             A1
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

//  4U2Do ... put any required libraries here  <----------------- IMPORTANT -----------------


//*******************************************************************************************
// Global Variables are available to every function in the program.
// Frowned on by traditionally trained programmers, they are widely accepted in the Arduino
// community thanks to their simplicity.  This program uses them for that reason. 
// https://www.arduino.cc/en/reference/scope
//
//*******************************************************************************************

// Set turning speed to 50% of maximum.

int leftSpeed    = MAXSPEED * .50;
int rightSpeed   = MAXSPEED * .50;
int  pwmOutput;  
// https://www.arduino.cc/reference/en/language/variables/data-types/bool/
bool Forward   = true;

//*******************************************************************************************
// The setup() function is called when a sketch starts. 
// Use it to initialize variables, pin modes, start communications, etc. 
// The setup function will only run once, after each powerup or reset of the Arduino board.
// https://www.arduino.cc/en/reference/setup
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
  pinMode ( MOTOR_Left_SW, INPUT );
  pinMode ( MOTOR_Right_SW, INPUT);

 }
//*******************************************************************************************
// Ideally, loop() should contain a minimum, or be completely devoid of, "raw code".  
// At a glance, it should convey the essence of the core algorithm of the sketch.
// https://www.arduino.cc/en/reference/loop
//*******************************************************************************************

void loop ( void ) {
  pwmOutput = MAXSPEED;
  forward();
  Forward  = true;
  if (digitalRead(MOTOR_Left_SW) == true) {   // go left?
    left();
    pwmOutput = leftSpeed; 
    }
  if (digitalRead(MOTOR_Right_SW) == true) {   // go right?
    right();
    pwmOutput = rightSpeed;      
    }
  
  analogWrite(R_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Right Motor 
  analogWrite(L_MOTOR_SPEED, pwmOutput);            // Send PWM signal to Left Motor 
 
  Serial.print(Forward);
  Serial.print("---");

  // To adhere to the intended algorithm, NOTHING ELSE should be added to loop() 
  // without discussing with the course instructor first!

  getSensorReadings ( );

  updateTimers ( );

}


//*******************************************************************************************
//
// Main Functions
// https://www.arduino.cc/en/Reference/FunctionDeclaration
//
//*******************************************************************************************

void getSensorReadings ( void ) {
// Responsibilities :: Read sensors, possibly override pilot command, or issue additional/new command for vehicleAction( ). Used in Phase 3 and above.
//                     Ultimately, this should not be a big ball of raw code...
//                     Different sensors should have their own functions called from here!

// IMPORTANT!!! This function should only be getting INPUT from the sensors.
//                            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// THERE SHOULD BE NO INTERACTION with pilot or vehicle components such as the motor controller, lights, etc! 
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 

  // 4U2Do ... add the correct code


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
    
}


void forward ( void ) {
// Responsibilities :: Motors Forward
  digitalWrite(R_MOTOR_DIREC1, HIGH); 
  digitalWrite(R_MOTOR_DIREC2, LOW);
  digitalWrite(L_MOTOR_DIREC1, HIGH); 
  digitalWrite(L_MOTOR_DIREC2, LOW);
}


void reverse ( void ) {
// Responsibilities :: Motors Reverse
  digitalWrite(R_MOTOR_DIREC1, LOW); 
  digitalWrite(R_MOTOR_DIREC2, HIGH); 
  digitalWrite(L_MOTOR_DIREC1, LOW); 
  digitalWrite(L_MOTOR_DIREC2, HIGH);
  
}


void left ( void ) {
// Responsibilities :: Motors Left pivot at (leftSpeed, 0)
  digitalWrite(R_MOTOR_DIREC1, HIGH); 
  digitalWrite(R_MOTOR_DIREC2, LOW);
  digitalWrite(L_MOTOR_DIREC1, LOW); 
  digitalWrite(L_MOTOR_DIREC2, HIGH);  
}

void right ( void ) {
  digitalWrite(R_MOTOR_DIREC1, LOW); 
  digitalWrite(R_MOTOR_DIREC2, HIGH); 
  digitalWrite(L_MOTOR_DIREC1, HIGH); 
  digitalWrite(L_MOTOR_DIREC2, LOW);
    
}



//*******************************************************************************************
//
//  Misc Functions
//  Anything else should go in this section
//
//*******************************************************************************************


