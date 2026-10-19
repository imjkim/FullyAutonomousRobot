/*
 * File:   bot.h
 * Author: sirajesh
 *
 * Created on May 11, 2022, 1:32 PM
 */


#ifndef BOT_H
#define BOT_H

#include <BOARD.h>
/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

#define ROACH_MAX_SPEED 100

// BOT_INIT() 
// Initialize every sensor/motor 

void Bot_Init(void);

char Bot_LeftMtrSpeed(char newSpeed);

char Bot_RightMtrSpeed(char newSpeed);
unsigned char Bot_MidMtrSpeed(char newSpeed);
unsigned char Bot_ReadFrontTapeSensor(void);

unsigned char Bot_ReadRightTapeSensor(void);
unsigned char Bot_ReadLeftTapeSensor(void);
unsigned char Bot_ReadMidRightTapeSensor(void);
unsigned char Bot_ReadMidLeftTapeSensor(void);
unsigned char Bot_ReadBackTapeSensor(void);
unsigned char Bot_PuncherMtrSpeed(char newSpeed);
unsigned char Bot_ReadSideTapeSensors(void);
unsigned char Bot_ReadTapeSensors(void);

unsigned char Bot_ReadBumpers(void);

unsigned int Bot_BeaconReading(void);

unsigned int Bot_TrackWireReading(void);
unsigned int Bot_TrackWire2Reading(void);

#endif