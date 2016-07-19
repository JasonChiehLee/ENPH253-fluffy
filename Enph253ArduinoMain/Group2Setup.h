#pragma once

//DIGITAL INPUTS
#define FRONT_RIGHT_GROUND_SWITCH 0
#define FRONT_LEFT_GROUND_SWITCH 1
#define FRONT_RIGHT_BUMPER_SWITCH 2
#define FRONT_LEFT_BUMPER_SWITCH 3
#define BACK_RIGHT_BUMPER_SWITCH 4
#define BACK_LEFT_BUMPER_SWITCH 5
#define CLAW_SWITCH 6

#define ON 1
#define OFF 0

//ANALOG INPUTS
#define TAPE_FOLLOWING_QRD_RIGHT  0
#define TAPE_FOLLOWING_QRD_LEFT 1
#define INTERSECTION_QRD_RIGHT 2
#define INTERSECTION_QRD_LEFT 3
#define SIDE_QSD_RIGHT 4
#define SIDE_QSD_LEFT 5
#define FRONT_QSD 6
#define TOP_QSD 7

//MOTOR OUTPUTS
#define MOTOR_RIGHT_WHEEL 0
#define MOTOR_LEFT_WHEEL 1
#define MOTOR_CLAW 2
#define MOTOR_CONVEYOR 3

//SERVO OUTPUTS
//RCServo0 BASEPOSITION
//RCServo1 BACKPOSITION
//RCServo2 FRONTPOSITION

// QRD to ground threshold
#define QRD_GROUND_THRESHOLD 150

// SIDE QSD to doll threshold
#define QSD_SIDE_THRESHOLD 750

// Navigation constants
#define TAPE_FOLLOWING_TRESHOLD QRD_GROUND_THRESHOLD
#define TAPE_FOLLOWING_CORRECTION 3
#define TAPE_FOLLOWING_DEFAULT_SPEED 120
#define HARD_STOP_WAIT_TIME 120 //ms
#define INTERSECTION_TURN_SPEED 150
#define UTURN_SPEED 100
#define PULLOVER_SPEED 100
#define CLAW_SPEED 100
#define CONVEYOR_SPEED 100
#define CONVEYOR_TIME_TICK 2000
#define INTERSECTION_TURN_WAIT_TICK 5
#define INTERSECTION_TURN_IGNORE_TIME 500
#define INTERSECTION_TURN_TIME 500
#define MOTOR_WRITE_WAIT_TICK 5
#define ARM_WAIT_TICK 500
#define BACKUP_WAIT_TICK 1000

#define LOST_TICK 1000

enum direction_e {
  RIGHT, LEFT, FORWARD, BACKWARD
};

struct armPosition_t
{
  byte baseRotationAngle;
  byte backPositionAngle;
  byte frontPositionAngle;
};

// Arm position
armPosition_t reset = {90, 10, 150};
armPosition_t rightPickUp = {0, 50, 80};
armPosition_t leftPickUp = {180, 50, 80};
armPosition_t rightDropOff = {80, 90, 80};
armPosition_t leftDropOff = {100, 90, 80};

enum clamp_e {
  CLOSE, OPEN
};

enum rotation_e {
  STARBOARD, PORT
};

#define TRUE 1
#define FALSE 0

bool loadStatus = FALSE;
void setLoadStatus(bool newLoadStatus) {
  loadStatus = newLoadStatus;
};

direction_e previousTurn = RIGHT;
void setPreviousTurn(direction_e newPreviousTurn)
{
  previousTurn = newPreviousTurn;
}

direction_e nextTurn = RIGHT;
void setNextTurn(direction_e newNextTurn)
{
  nextTurn = newNextTurn;
}

bool dropStatus = FALSE;
void setStatus(bool newDropStatus)
{
  dropStatus = newDropStatus;
}

byte travelAngle = 90;
void setTravelAngle(byte newTravelAngle)
{
  travelAngle = newTravelAngle;
}

