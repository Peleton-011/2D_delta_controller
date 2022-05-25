
#include <math.h>

using namespace std;

//Declare initial variables and parametrs

//Step
const int STEPS[2] = {7, 8};

//Dir
const int DIRS[2] = {6, 9};

//Tool control
const int TOOL = 10;

//Steps per revolution
const int  steps_per_rev = 200;

//Radians per motor step
const int radians_per_step = 0;

//Homing pins
const int homing_pins[2] = {2, 4};

//Other parameters
const int motor_frequency = 50;

const int motors = 2;

//Motor angle at homing
const double default_angles[2] = {3.3, 4.8};

//Default spinning directions. Should be determined through testing
const bool default_direction_cc[2] = {true, true};

//Range of action
int range[2] = {0, 0};

//Current position in steps
int angle[2] = {0, 0};

//Setup motor period to enforce motor frequency
int motor_period = 1000/motor_frequency;

//Setup for serial comms
enum state_codes {    wait, get_sof1, check_get_sof2, check_get_dest, check_get_src, get_size, setup_count, get_byte_dec, get_parity, get_eof, check_parity, write_data, discard_data } current_state, next_state;

enum event_code { no_event, byte_received, error, timeout } current_event;

//Sets the current event
void event_set(enum event_code new_event){
    current_event=new_event;
}

//Clears current event
void event_clear(){
    current_event=no_event;
}

//Returns current event
enum event_code event_get(){
    return current_event;
}

//Checks for an incoming byte_recieved
void waitFunc()
{
    switch(event_get())
    {
        case byte_received:
            next_state=get_sof1;
            break;

        case no_event:
            next_state=wait;
            break;

        case error:
            next_state=wait;
            break;

        case timeout:
            next_state=wait;
            break;
    }
}

//Handles states and their behaviour
void stateMachine()
{
    switch(current_state)
    {
    case wait:
        waitFunc();
        break;

    case get_sof1:

        break;
    case check_get_sof2:

        break;
    case check_get_dest:

        break;
    case check_get_src:

        break;
    case get_size:

        break;
    case setup_count:

        break;
    case get_byte_dec:

        break;
    case get_parity:

        break;
    case get_eof:

        break;
    case check_parity:

        break;
    case write_data:

        break;
    case discard_data:

        break;
    default:
        break;
    }
}


void setup()
{
    //Start serial connection of 115,200 bauds
    Serial.begin(115200);
    
    //Setup all output pins
    for (int i = 0; i < motors; i++)
    {
        pinMode(DIRS[i], OUTPUT);
        pinMode(STEPS[i], OUTPUT);
    }
    
    pinMode(TOOL, OUTPUT);

    //Setup all input pins
    for (int i = 0; i < (motors * 2); i++)
    {
        pinMode(homing_pins[i], INPUT);
    }


    //Homing and initial position
    for (int i = 0; i < motors; i++)
    {
        homing(i);
    }
}

//Set turning direction, if opposite is false it will be the default direction for that motor
//otherwise it will be the other direction
void setDirection(int motor, bool opposite)
{
    if (!opposite)
    {
        if (default_direction_cc[motor])
        {
            digitalWrite(DIRS[motor], LOW);
            return;
        }

        digitalWrite(DIRS[motor], HIGH);
        return;
    }
    
    if (!default_direction_cc[motor])
    {
        digitalWrite(DIRS[motor], LOW);        
        return;
    }
    digitalWrite(DIRS[motor], HIGH);
    return;
}

//Homing function
void homing(int motor)
{
    int opposite_motor;

    if (!!motor)
    {
        opposite_motor = 0;
    }
    else
    {
        opposite_motor = 1;
    }

    setDirection(opposite_motor, false);
    setDirection(motor, true);

    while (digitalRead(homing_pins[motor]) == 0)
    {
        digitalWrite(STEPS[motor], HIGH);
        delayMicroseconds(motor_period);
        digitalWrite(STEPS[motor], LOW);
        delayMicroseconds(motor_period);

        digitalWrite(STEPS[opposite_motor], HIGH);
        delayMicroseconds(motor_period);
        digitalWrite(STEPS[opposite_motor], LOW);
        delayMicroseconds(motor_period);
    }
    
    angle[motor] = default_angles[motor];
    range[motor] = default_angles[motor];

    setDirection(opposite_motor, true);
    setDirection(motor, false);

    while (digitalRead(homing_pins[opposite_motor]) == 0)
    {
        digitalWrite(STEPS[motor], HIGH);
        delayMicroseconds(motor_period);
        digitalWrite(STEPS[motor], LOW);
        delayMicroseconds(motor_period);

        digitalWrite(STEPS[opposite_motor], HIGH);
        delayMicroseconds(motor_period);
        digitalWrite(STEPS[opposite_motor], LOW);
        delayMicroseconds(motor_period);

        angle[motor] -= radians_per_step;

    }

    angle[opposite_motor] = default_angles[opposite_motor];
    range[opposite_motor] = default_angles[opposite_motor];

    return;
} 


//Generic function to move to a specific position
void move(int coord, int motor)
{
    int distance = angle[motor] - coord;
    //Return if the distance is 0
    if (!distance)
    {
        return;
    }
    
    setDirection(motor, (distance > 0));

    while (!!distance)
    {
        //Move a step
        digitalWrite(STEPS[motor], HIGH);
        delayMicroseconds(motor_period);
        digitalWrite(STEPS[motor], LOW);
        delayMicroseconds(motor_period);

        //Update position
        if (distance > 0)
        {
            angle[motor]++;
        }
        else if (distance < 0)
        {
            angle[motor]--;
        }
        else //Return if the position is reached
        {
            return;
        }

        //Update distance
        distance = angle[motor] - coord;
    }
}

//Function that uses tool whichever the current coordinates
void useTool()
{
    digitalWrite(TOOL, HIGH);
    move(range[3], 3);
    delayMicroseconds(actuation_length);
    move(0, 3);
    digitalWrite(TOOL, LOW);
}