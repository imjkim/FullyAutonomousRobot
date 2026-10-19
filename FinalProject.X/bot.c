/*
 * File:   bot.c
 * Author: sirajesh
 *
 * Created on May 11, 2022, 1:35 PM
 */

#include <bot.h>
#include <BOARD.h>
#include "xc.h"
#include <pwm.h>
#include <serial.h>
#include <AD.h>
#include <IO_Ports.h>

#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define RIGHT_PWM PWM_PORTY10
#define LEFT_PWM PWM_PORTY12
#define PUNCHER_PWM PWM_PORTY04
#define MID_PWM PWM_PORTZ06 

#define LEFT_DIR_TRIS PORTZ03_TRIS
#define LEFT_DIR_INV_TRIS PORTZ04_TRIS
#define RIGHT_DIR_TRIS PORTZ05_TRIS
#define RIGHT_DIR_INV_TRIS PORTZ08_TRIS
#define MID_DIR_TRIS PORTY07_TRIS
#define MID_DIR_INV_TRIS PORTZ09_TRIS
#define PUNCHER_DIR_TRIS PORTY11_TRIS
#define PUNCHER_DIR_INV_TRIS PORTY09_TRIS


#define RIGHT_DIR PORTZ03_LAT
#define RIGHT_DIR_INV PORTZ04_LAT
#define LEFT_DIR PORTZ05_LAT
#define LEFT_DIR_INV PORTZ08_LAT
#define MID_DIR PORTY07_LAT
#define MID_DIR_INV PORTZ09_LAT
#define PUNCHER_DIR PORTY11_LAT
#define PUNCHER_DIR_INV PORTY09_LAT

// Tape Sensors 

#define FRONT_T_TRIS PORTX04_TRIS
#define RIGHT_T_TRIS PORTX03_TRIS
#define LEFT_T_TRIS PORTX08_TRIS
#define BACK_T_TRIS PORTZ07_TRIS
#define RIGHT_MID_T_TRIS PORTX05_TRIS
#define LEFT_MID_T_TRIS PORTX06_TRIS


#define SIDE_T_B_TRIS PORTY03_TRIS
#define SIDE_T_F_TRIS PORTY08_TRIS

#define FRONT_T PORTX04_BIT
#define RIGHT_T PORTX03_BIT
#define LEFT_T PORTX08_BIT
#define BACK_T PORTZ07_BIT
#define RIGHT_MID_T PORTX05_BIT
#define LEFT_MID_T PORTX06_BIT
#define SIDE_T_B PORTY03_BIT
#define SIDE_T_F PORTY08_BIT

// Bumpers
#define FRONT_R_B_TRIS PORTX09_TRIS
#define FRONT_L_B_TRIS PORTX11_TRIS
#define BACK_R_B_TRIS PORTX12_TRIS
#define BACK_L_B_TRIS PORTZ11_TRIS
#define SIDE_B_TRIS PORTY05_TRIS
#define SIDE_F_TRIS PORTY06_TRIS

#define FRONT_R_B PORTX09_BIT
#define FRONT_L_B PORTX11_BIT
#define BACK_R_B PORTX12_BIT
#define BACK_L_B PORTZ11_BIT
#define SIDE_B PORTY05_BIT
#define SIDE_F PORTY06_BIT

// Beacon Detector 
#define BEACON_DETECTOR AD_PORTV3
#define TRACK_WIRE AD_PORTV5
#define TRACK_WIRE2 AD_PORTV6
#define BOT_MAX_SPEED 100 


void Bot_Init(void){
    //PWM for Motors
    BOARD_Init();
    PWM_Init();
    AD_Init();
    PWM_SetFrequency(MAX_PWM_FREQ);
    PWM_AddPins(LEFT_PWM | RIGHT_PWM | MID_PWM | PUNCHER_PWM);
    
    // ADD H BRIDGE 
   
    LEFT_DIR_TRIS = 0;
    LEFT_DIR_INV_TRIS = 0;
    RIGHT_DIR_TRIS = 0;
    RIGHT_DIR_INV_TRIS = 0;
    MID_DIR_TRIS = 0;
    MID_DIR_INV_TRIS = 0;
    PUNCHER_DIR_TRIS = 0;
    PUNCHER_DIR_INV_TRIS = 0;
    
    LEFT_DIR = 0;
    LEFT_DIR_INV = ~LEFT_DIR;
    RIGHT_DIR = 0;
    RIGHT_DIR_INV = ~RIGHT_DIR;
    PUNCHER_DIR = 0;
    PUNCHER_DIR_INV = ~PUNCHER_DIR;
    MID_DIR = 0;
    MID_DIR_INV = ~MID_DIR;
    printf("Init");
    
    // TAPE Sensors
    
    FRONT_T_TRIS = 1;
    RIGHT_T_TRIS = 1;
    LEFT_T_TRIS = 1;
    BACK_T_TRIS = 1;
    RIGHT_MID_T_TRIS = 1;
    LEFT_MID_T_TRIS = 1;
    SIDE_T_F_TRIS = 1;
    SIDE_T_B_TRIS = 1;
    
    // Bumper Sensor
    FRONT_R_B_TRIS  = 1;
    FRONT_L_B_TRIS = 1;
    BACK_R_B_TRIS = 1;
    BACK_L_B_TRIS = 1;
    SIDE_B_TRIS = 1;
    SIDE_F_TRIS = 1;
    
    AD_AddPins(BEACON_DETECTOR | TRACK_WIRE | TRACK_WIRE2);
    
}

char Bot_LeftMtrSpeed(char newSpeed){
    
    if ((newSpeed < -BOT_MAX_SPEED) || (newSpeed > BOT_MAX_SPEED)) {
        return (ERROR);
    }

    if (newSpeed < 0) {
        LEFT_DIR = 0;
        newSpeed = newSpeed * (-1); // set speed to a positive value
    } else {
        LEFT_DIR = 1;
    }
    LEFT_DIR_INV = ~(LEFT_DIR);
    if (PWM_SetDutyCycle(LEFT_PWM, newSpeed * (MAX_PWM / BOT_MAX_SPEED)) == ERROR) {
        printf("ERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);
}

char Bot_RightMtrSpeed(char newSpeed){
if ((newSpeed < -BOT_MAX_SPEED) || (newSpeed > BOT_MAX_SPEED)) {
        return (ERROR);
    }
newSpeed = -newSpeed;
    if (newSpeed < 0) {
        RIGHT_DIR = 0;
        newSpeed = newSpeed * (-1); // set speed to a positive value
    } else {
        RIGHT_DIR = 1;
    }
    RIGHT_DIR_INV = ~(RIGHT_DIR);
    if (PWM_SetDutyCycle(RIGHT_PWM, newSpeed * (MAX_PWM / BOT_MAX_SPEED)) == ERROR) {
        //puts("\aERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);

}

unsigned char Bot_MidMtrSpeed(char newSpeed){
    if ((newSpeed < -BOT_MAX_SPEED) || (newSpeed > BOT_MAX_SPEED)) {
        return (ERROR);
    }
    if (newSpeed < 0) {
        MID_DIR = 0;
        newSpeed = newSpeed * (-1); // set speed to a positive value
    } else {
        MID_DIR = 1;
    }
    MID_DIR_INV = ~(MID_DIR);
    if (PWM_SetDutyCycle(MID_PWM, newSpeed * (MAX_PWM / BOT_MAX_SPEED)) == ERROR) {
        //puts("\aERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);
}

unsigned char Bot_PuncherMtrSpeed(char newSpeed){
if ((newSpeed < -BOT_MAX_SPEED) || (newSpeed > BOT_MAX_SPEED)) {
        return (ERROR);
    }
    if (newSpeed < 0) {
        PUNCHER_DIR = 0;
        newSpeed = newSpeed * (-1); // set speed to a positive value
    } else {
        PUNCHER_DIR = 1;
    }
    PUNCHER_DIR_INV = ~(PUNCHER_DIR);
    if (PWM_SetDutyCycle(PUNCHER_PWM, newSpeed * (MAX_PWM / BOT_MAX_SPEED)) == ERROR) {
        //puts("\aERROR: setting channel 1 speed!\n");
        return (ERROR);
    }
    return (SUCCESS);

}
 
unsigned char Bot_ReadFrontTapeSensor(void){
    return FRONT_T;
}
unsigned char Bot_ReadRightTapeSensor(void){
    return !RIGHT_T;
}
unsigned char Bot_ReadLeftTapeSensor(void){
    return !LEFT_T;
}
unsigned char Bot_ReadMidRightTapeSensor(void){
    return RIGHT_MID_T;
}
unsigned char Bot_ReadMidLeftTapeSensor(void){
    return LEFT_MID_T;
}
unsigned char Bot_ReadBackTapeSensor(void){
    return BACK_T;
}

unsigned char Bot_ReadSideTapeSensors(void){
    return (SIDE_T_B + ((SIDE_T_F) << 1));
}
unsigned char Bot_ReadTapeSensors(void){
    
    return (RIGHT_MID_T + ((LEFT_MID_T) << 1));
}

unsigned char Bot_ReadBumpers(void){
    return (FRONT_L_B + ((FRONT_R_B) << 1)+((BACK_L_B) << 2)+((BACK_R_B) << 3) + ((SIDE_B) << 4) + ((SIDE_F) << 5));
}
unsigned int Bot_BeaconReading(void){
return AD_ReadADPin(BEACON_DETECTOR);
}

unsigned int Bot_TrackWireReading(void){
    return AD_ReadADPin(TRACK_WIRE);
}
unsigned int Bot_TrackWire2Reading(void){
    return AD_ReadADPin(TRACK_WIRE2);
}
#ifdef BOT_TEST
//#define BOT_TEST
int main(void) {
    
    Bot_Init();
    Bot_LeftMtrSpeed(100);
   Bot_RightMtrSpeed(100);
    while(1){
        printf("%u\n", FRONT_T);
    }
    return 0;
}
#endif