const int LeftMotorForward = 2; // good utilization of variable names can help overall in the building and reading of code
const int LeftMotorBackward = 3;
const int RightMotorForward  = 4;
const int RightMotorBackward = 5;

const int ultraSonicTrig = A3;     //Ultra sonic sensor pins I/O
const int ultraSonicSense = A2;
long duration;                    // Used for the ultra sonic sensor. its the time it take for a pulse of sound to travel from one eye 

double distance; //Aribtrary variable used for sensing and logic

bool goingForward = false; // delcare this so that code works

double minDist; // This is an abritrary variable dipendent on value declared for specific aplication
double degreeMs = 10; // Some value calculated for right now its 1, units:  ((x) ms) /  (1 degree)

int degreeInc = 0; // this will change when trying to find where a object is
bool object = true; // determines if the object is infront of the sensor

void setup() {
  Serial.begin(9600);

  pinMode(RightMotorForward, OUTPUT); // All output becuase we are just interacting with a logi gate
  pinMode(LeftMotorForward,  OUTPUT); // So they either are outputing power ("1") or they arent ("0")
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward,  OUTPUT);

  pinMode(ultraSonicTrig, OUTPUT); // This sends the signal
  pinMode(ultraSonicSense, INPUT); // This recieves the signal

  servo1.attach(servo1Pin); 
  servo2.attach(servo2Pin);

}

void loop() {
}

void objectFollow(){
  minDist = 4; // 4 inches

  while(object){ // if object is in sight then move forward until you are at the min dist or closer
    if(Sense() > 20){
      Stop();
      object = false;
      Serial.print("lost object");
      break;
    }

    if(Sense() <= minDist){
      Serial.print("stopped at object");
      Stop();
      delay(50);
    } else {
      Serial.print("moving toward object");
      moveForward();
      delay(100);
    }

  }

  while(!object){ // if object isnt in sight then look back and fourth increasing the angle incriments until object is found
    degreeInc+= 5;

    Serial.print("left");
    turnLeft();
    delay(degreeInc * degreeMs);
    Stop();
    if(Sense() <= 20){
      Stop();
      delay(50);
      degreeInc = 0;
      object = true;
      Serial.print("object found");
      break;
    }
    delay(50);

    Serial.print("left");
    turnRight();
    delay((degreeInc * 2) * degreeMs);
    Stop();
    if(Sense() <= 20){
      Stop();
      delay(50);
      degreeInc = 0;
      object = true;
      Serial.print("object found");
      break;
    }
    delay(50);
  }
}




void objectAvoid(){
  minDist = 7;
  if(Sense() <= minDist){
    Stop();
    turnLeft();
    delay(50 * degreeMs);
    Stop();
    if(Sense() <= minDist){
      Stop();
      turnRight();
      delay(100 * degreeMs);
      Stop();
    }
  } else {
    moveForward();
    delay(50);
  }
}


double Sense() {
  digitalWrite(ultraSonicTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSonicTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSonicTrig, LOW);

  duration = pulseIn(ultraSonicSense, HIGH);

  if(duration == 0){
    digitalWrite(ultraSonicTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(ultraSonicTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraSonicTrig, LOW);

    duration = pulseIn(ultraSonicSense, HIGH);
  }

  distance = ((duration*.0343)/2) * 0.39;
  Serial.print("Distance: ");
  Serial.println(distance);

  return(distance);
}


// all functions under this coment are movement functions
// notice how when preforming a motion you must ddigital write the HIGH's before the LOW's
// also notice how we must know if we are moving forward and not backward in order to exicute
// the task. This is because the H-Bridge wants it that way, and dont ask me any further questions.
void moveBackward() {
    // Code to drive motors backward
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(LeftMotorBackward, HIGH);
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(RightMotorBackward, HIGH);
}

void moveForward() {
    // Code to drive motors forward
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(RightMotorBackward, LOW);
}

void Stop() {
    // Code to stop motors
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(RightMotorBackward, LOW);
}

void turnLeft() {
    // Code to turn left
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(LeftMotorBackward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(RightMotorBackward, LOW);
}

void turnRight() {
    // Code to turn right
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(RightMotorBackward, HIGH);
}
