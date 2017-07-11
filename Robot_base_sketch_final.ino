//define ints for Motor 1 pins
const int M1In1 = 8; //digital output
const int M1In2 = 11; //digital output
const int M1PWM = 9;//PWM
const int M1QEIA = 3; //interrupt
const int M1QEIB = 7;//digital input

//define ints for Motor 2 pins
int M2In1 = 12;//digital output
int M2In2 = 13;//digital output
int M2PWM = 10;//PWM
int M2QEIA = 2; //interrupt
int M2QEIB = 4;//digital input


//define ints for encoder count data
int M1count = 0;
int M2count = 0;
int M1countold = 0;
int M2countold = 0;

//define floats for wheel rotation data
float M1rotations = 0;
float M2rotations = 0;

//define int for distance per rotation
int rotDistance = 188.495;

//define int for angle turned
int angle = 0;

//define ints for distance travelled
int distance1 = 0;
int distance2 = 0;

//define int for switch case sequence
int sequence = 1;


void setup() {
//define pinModes for Motor 1
pinMode (M1In1, OUTPUT);
pinMode (M1In2, OUTPUT);
pinMode (M1PWM, OUTPUT);
pinMode (M1QEIA, INPUT_PULLUP);
pinMode (M1QEIB, INPUT_PULLUP);
Serial.begin (9600);
attachInterrupt(digitalPinToInterrupt(M1QEIA), encode1, CHANGE);

//define pinModes for Motor 2
pinMode (M2In1, OUTPUT);
pinMode (M2In2, OUTPUT);
pinMode (M2PWM, OUTPUT);
pinMode (M2QEIA, INPUT_PULLUP);
pinMode (M2QEIB, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(M2QEIA), encode2, CHANGE);
}

void encode1 () {
M1count++;  
}

void encode2 () {
M2count++;  
}

void baseReverse (int x, int y){
distance1 = 0;
//distance travelled is wheel rotations x distance per rotation
distance1 = M1rotations*rotDistance;
//if distance travelled is smaller than input distance, engage forward drive
if (distance1 < x){
digitalWrite (M1In1, LOW);
digitalWrite (M1In2, HIGH);
analogWrite (M1PWM, y);
digitalWrite (M2In1, HIGH);
digitalWrite (M2In2, LOW);
analogWrite (M2PWM, y);
}
else {
//if not, brake wheels and stop
digitalWrite (M1In1, LOW);
digitalWrite (M1In2, LOW);
digitalWrite (M1PWM, LOW);
digitalWrite (M2In1, LOW);
digitalWrite (M2In2, LOW);
digitalWrite (M2PWM, LOW);
//increase sequence counter to next switch case
sequence++;
delay(100);
}

//this code reports distance travelled
if (M1countold != M1count)
  {
    M1rotations=M1count/32.00;
    M1rotations*rotDistance;
       M1countold = M1count;
       Serial.println();
       Serial.print ("distance travelled = ");
       Serial.print (distance1);
       Serial.println(" mm");
       delay (100);
} 
}

void baseForward (int x, int y){
distance1 = 0; 
//distance travelled is wheel rotations x distance per rotation
distance1=M1rotations*rotDistance;
if (distance1 < x){
//if distance travelled is smaller than input distance, engage forward drive
digitalWrite (M1In1, HIGH);
digitalWrite (M1In2, LOW);
analogWrite (M1PWM, y);
digitalWrite (M2In1, LOW);
digitalWrite (M2In2, HIGH);
analogWrite (M2PWM, y);
if (M1countold != M1count)
//report distance travelled
  {
    M1rotations=M1count/32.00;
    M1rotations*rotDistance;
       M1countold = M1count;
       Serial.println();
       Serial.print ("M1 distance travelled = ");
       Serial.print (distance1);
       Serial.println(" mm");
       delay (200);
       return;
      }
}
else {
//brake the wheels to a stop
digitalWrite (M1In1, HIGH);
digitalWrite (M1In2, HIGH);
digitalWrite (M1PWM, LOW);
digitalWrite (M2In1, HIGH);
digitalWrite (M2In2, HIGH);
digitalWrite (M2PWM, LOW);
//increase sequence to next switch case
sequence++;
delay(1000);
return;
}


}

void turnRight (int x, int y) {
distance2=0;
angle=0;
distance2=M2rotations*rotDistance;
//angle = wheel distance travelled divided by angle divisions of wheelbase circle
angle = distance2/2.44;
if (angle < x) {
//if angle variable is less than x argument input, engage diff drive turning
digitalWrite (M1In1, HIGH);
digitalWrite (M1In2, LOW);
analogWrite (M1PWM, y);
digitalWrite (M2In1, HIGH);
digitalWrite (M2In2, LOW);
analogWrite (M2PWM, y);
if (M2countold != M2count)
//report angle turned
  {
    M2rotations=M2count/32.00;
    M2rotations*rotDistance;
       M2countold = M2count;
       Serial.println();
       Serial.print ("Angle turned = ");
       Serial.print (angle);
       Serial.println(" degrees");
       delay (10);
       return;
      }

}

else {
//brake wheels and stop
digitalWrite (M1In1, HIGH);
digitalWrite (M1In2, HIGH);
digitalWrite (M1PWM, LOW);
digitalWrite (M2In1, HIGH);
digitalWrite (M2In2, HIGH);
digitalWrite (M2PWM, LOW); 
//increase sequence to next switch case 
sequence++;
}
}

void turnLeft (int x, int y) {
//distance2=0;
//angle=0;
//distance2=M2rotations*rotDistance;
////angle = wheel distance travelled divided by angle divisions of wheelbase circle
//angle = distance2/2.44;
if (angle < x) {
digitalWrite (M1In1, HIGH);
digitalWrite (M1In2, HIGH);
analogWrite (M1PWM, x);
digitalWrite (M2In1, LOW);
digitalWrite (M2In2, HIGH);
analogWrite (M2PWM, x);
}

else {
//brake wheels and stop
digitalWrite (M1In1, HIGH);
digitalWrite (M1In2, HIGH);
digitalWrite (M1PWM, LOW);
digitalWrite (M2In1, HIGH);
digitalWrite (M2In2, HIGH);
digitalWrite (M2PWM, LOW); 
//increase sequence to next switch case 
sequence++;
}

}
void clearValues() {
//this flushes the values used to calculate distances/angles
M1count = 0;
M2count = 0;
M1countold = 0;
M2countold = 0;
M1rotations = 0;
M2rotations = 0;
angle = 0;
distance1 = 0;
distance2 = 0;
Serial.println("clearing values");
sequence++;
return;
}

void loop() {
  // put your main code here, to run repeatedly:
switch (sequence) {
  case 1:
  {baseForward (500, 250);}
  break;
  case 2:
  {clearValues();}
  break;
  case 3:
  turnRight (90, 90);
  break;
  case 4:
  clearValues();
  break;
  case 5:
  sequence = 1;
}

}




