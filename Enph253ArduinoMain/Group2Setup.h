#pragma once

#define DERIVATIVE_GAIN 20
#define PROPORTIONAL_GAIN 20

//DIGITAL INPUTS
#define TAPE_FOLLOWING_QRD_RIGHT  13
#define TAPE_FOLLOWING_QRD_LEFT 12
#define INTERSECTION_QRD_RIGHT 10
#define INTERSECTION_QRD_LEFT 11
#define FRONT_RIGHT_GROUND_SWITCH 7
#define FRONT_LEFT_GROUND_SWITCH 6
#define FRONT_RIGHT_BUMPER_SWITCH 9
#define FRONT_LEFT_BUMPER_SWITCH 8
#define BACK_RIGHT_BUMPER_SWITCH 5
#define BACK_LEFT_BUMPER_SWITCH 4
#define CLAW_SWITCH 14
#define DOLL_SWITCH 15

#define ON 1
#define OFF 0

#define PRESS_NO 1
#define PRESS_YES 0

//ANALOG INPUTS
#define SIDE_QSD_RIGHT 2
#define SIDE_QSD_LEFT 0
// from the left
#define FRONT_QSD1 1
#define FRONT_QSD2 3
#define FRONT_QSD3 4
#define FRONT_QSD4 6
#define FRONT_QSD5 5
#define TOP_QSD 7

//FRONT QSD ANGLE
#define ONE_ANGLE 30
#define TWO_ANGLE 60
#define THREE_ANGLE 90
#define FOUR_ANGLE 120
#define FIVE_ANGLE 150

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
#define QSD_FRONT_THRESHOLD 150

// Navigation constants
#define TAPE_FOLLOWING_TRESHOLD QRD_GROUND_THRESHOLD
#define TAPE_FOLLOWING_CORRECTION 3
#define TAPE_FOLLOWING_DEFAULT_SPEED 120
#define HARD_STOP_WAIT_TIME 60 //ms
#define INTERSECTION_TURN_SPEED 150
#define UTURN_SPEED 100
#define PULLOVER_SPEED 100
#define CLAW_SPEED 100
#define CLAW_WAIT_TICK 500
#define CONVEYOR_SPEED 100
#define CONVEYOR_TIME_TICK 2000
#define INTERSECTION_TURN_WAIT_TICK 5
#define INTERSECTION_TURN_IGNORE_TIME 500
#define INTERSECTION_TURN_TIME 500
#define MOTOR_WRITE_WAIT_TICK 5
#define ARM_POSITION_TICK 500
#define ARM_WAIT_TICK 500
#define BACKUP_WAIT_TICK 1000
#define SERVO_WAIT_TICK 500

#define LOST_TICK 1000

#define THRESHOLDANGLE1 70
#define THRESHOLDANGLE2 110

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
armPosition_t rightPickUp = {0, 50, 50};
armPosition_t leftPickUp = {180, 50, 50};
armPosition_t rightDropOff = {80, 110, 80};
armPosition_t leftDropOff = {100, 110, 80};

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

int clawClampTick = 0;
void setClawClampTick(int newClawClampTick)
{
  clawClampTick = newClawClampTick;
}

