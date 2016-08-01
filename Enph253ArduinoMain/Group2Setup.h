#pragma once

#define DERIVATIVE_GAIN 20
#define PROPORTIONAL_GAIN 15

//DIGITAL INPUTS
#define TAPE_FOLLOWING_QRD_RIGHT  12
#define TAPE_FOLLOWING_QRD_LEFT 9
#define INTERSECTION_QRD_RIGHT 11
#define INTERSECTION_QRD_LEFT 10
#define FRONT_RIGHT_GROUND_SWITCH 5
#define FRONT_LEFT_GROUND_SWITCH 6
#define FRONT_RIGHT_BUMPER_SWITCH 7
#define FRONT_LEFT_BUMPER_SWITCH 8
#define BACK_RIGHT_BUMPER_SWITCH 3
#define BACK_LEFT_BUMPER_SWITCH 4
#define CLAW_CLOSE_SWITCH 13
#define CLAW_OPEN_SWITCH 15
#define DOLL_SWITCH 14

#define ON 1
#define OFF 0

#define PRESS_NO HIGH
#define PRESS_YES LOW

//ANALOG INPUTS
#define SIDE_QSD_RIGHT 2
#define SIDE_QSD_LEFT 0
// from the left
#define FRONT_QSD_LEFT 1
#define FRONT_QSD_STRAIGHT 3
#define FRONT_QSD_RIGHT 4
#define TOP_QSD_LEFT 6
#define TOP_QSD_STRAIGHT 5
#define TOP_QSD_RIGHT 7

//FRONT QSD ANGLE
#define LEFT_ANGLE 30
#define TWO_ANGLE 60
#define FORWARD_ANGLE 90
#define FOUR_ANGLE 120
#define RIGHT_ANGLE 150

//MOTOR OUTPUTS
#define MOTOR_RIGHT_WHEEL 1
#define MOTOR_LEFT_WHEEL 0
#define MOTOR_CLAW 3
#define MOTOR_CONVEYOR 2

//SERVO OUTPUTS
//RCServo0 BASEPOSITION
//RCServo1 BACKPOSITION
//RCServo2 FRONTPOSITION

// QRD to ground threshold
#define QRD_GROUND_THRESHOLD 150

// SIDE QSD to doll threshold
#define QSD_SIDE_THRESHOLD 90
#define QSD_FRONT_THRESHOLD 75

// Top QSD to dropoff zone threshold
#define QSD_TOP_THRESHOLD 100
#define AT_DROPOFF_ZONE_THRESHOLD 400

// Navigation constants
#define TAPE_FOLLOWING_TRESHOLD QRD_GROUND_THRESHOLD
#define TAPE_FOLLOWING_CORRECTION 3
#define TAPE_FOLLOWING_DEFAULT_SPEED 120
#define HARD_STOP_WAIT_TIME 40 //ms
#define INTERSECTION_TURN_SPEED 150
#define UTURN_SPEED 120
#define UTURN_TAPE_IGNORE_TIME 250 
#define PULLOVER_SPEED 100
#define CLAW_SPEED 200
#define CLAW_WAIT_TIME 500
#define CLAW_CLAMP_WAIT_TIME 1000
#define CONVEYOR_SPEED 250
#define CONVEYOR_WAIT_TIME 1500
#define INTERSECTION_TURN_WAIT_TIME 5
#define INTERSECTION_TURN_IGNORE_TIME 200
#define INTERSECTION_TURN_TIME 500
#define INTERSECTION_WAIT_TIME 1000
#define INTERSECTION_WIGGLE_TIME 160
#define DOLL_PICKUP_WAIT 500
#define MOTOR_WRITE_WAIT_TIME 5
#define ARM_POSITION_TIME 300
#define ARM_WAIT_TIME 500
#define BACKUP_WAIT_TIME 250
#define SERVO_WAIT_TIME 400
#define FAIL_WAIT_TIME 2000
#define CENTRE_SPEED 60
#define DROPOFF_CONVEYOR_WAIT_TIME 1500
#define DROPOFF_TURN_AROUND_WAIT 2000

#define LOST_TIME 1000

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
armPosition_t reset = {95, 120, 0};
armPosition_t pickUpInit = {0, 30, 160};
armPosition_t pickUpInitLimit1 = {0, 0, 60};
armPosition_t pickUpInitLimit2 = {0, 0, 120};
armPosition_t movingPosition = {95, 120, 0};
armPosition_t rightPickUp = {15, 30, 60};
armPosition_t leftPickUp = {180, 30, 60};
armPosition_t dropOffInit = {45, 150, 65};
armPosition_t dropOff = {90, 150, 65};



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

int clawClampTIME = 0;
void setClawClampTIME(int newClawClampTIME)
{
  clawClampTIME = newClawClampTIME;
}

int dollCount = 0;
