/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This file includes a test harness that will run the event detectors listed in the
 * ES_Configure file in the project, and will conditionally compile main if the macro
 * EVENTCHECKER_TEST is defined (either in the project or in the file). This will allow
 * you to check you event detectors in their own project, and then leave them untouched
 * for your project unless you need to alter their post functions.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "BeaconEventChecker.h"
#include "ES_Events.h"
#include "serial.h"
#include "bot.h"
#include <stdio.h>
#include "AD.h"
#include "TemplateHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define BEACON_THRESHOLD 675
#define NO_BEACON_THRESHOLD 550

#define TRACKWIRE2_THRESHOLD 700
#define TRACKWIRE2_THRESHOLDB 800
#define NOTRACKWIRE2_THRESHOLD 550

#define TRACKWIRE_THRESHOLD 850 // 700
#define TRACKWIRE_THRESHOLDB 700
#define NOTRACKWIRE_THRESHOLD 475
/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

//#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular event checker */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t CheckBeacon(void){
    static ES_EventTyp_t lastEvent = NOBEACON;
    ES_EventTyp_t curEvent = lastEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    // Read Current BeaconDetector; 
    uint16_t currentBeaconLevel = Bot_BeaconReading(); 
    
    // if dark 
    if (currentBeaconLevel > BEACON_THRESHOLD ){
        curEvent = BEACON; 
    }
    if(currentBeaconLevel < NO_BEACON_THRESHOLD ){
        curEvent = NOBEACON;
    }
    if (curEvent != lastEvent) { // check for change from last time
        thisEvent.EventType = curEvent;
        thisEvent.EventParam = currentBeaconLevel;
        returnVal = TRUE;
        lastEvent = curEvent; // update history
        PostTemplateHSM(thisEvent);  
    }
    return (returnVal);

}
uint8_t CheckTrackWire(void){
    static ES_EventTyp_t lastEvent = NOTWIRE;
    ES_EventTyp_t curEvent = lastEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    // Read Current BeaconDetector; 
    uint16_t currentTrackLevel = Bot_TrackWireReading(); 
    
    // if dark 
   //printf("\r \n %d", currentTrackLevel);

    if (currentTrackLevel > TRACKWIRE_THRESHOLD){
        curEvent = TWIRE; 
    }
//    else if( currentTrackLevel > TRACKWIRE_THRESHOLDB && currentTrackLevel < TRACKWIRE_THRESHOLD ){
//        curEvent = TWIREB;
//    }
    else if (currentTrackLevel < NOTRACKWIRE_THRESHOLD ){
        curEvent = NOTWIRE;
    }
    if (curEvent != lastEvent) { // check for change from last time
        thisEvent.EventType = curEvent;
        thisEvent.EventParam = currentTrackLevel;
        returnVal = TRUE;
        lastEvent = curEvent; // update history
        PostTemplateHSM(thisEvent);  
    }
    return (returnVal);
    
}

uint8_t CheckTrackWire2(void){
static ES_EventTyp_t lastEvent = NOTWIRE2;
ES_EventTyp_t curEvent = lastEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    // Read Current BeaconDetector; 
    uint16_t currentTrackLevel = Bot_TrackWire2Reading(); 
    
    // if dark 
    //printf("\r \n %d", currentTrackLevel);
    if (currentTrackLevel > TRACKWIRE2_THRESHOLD ){
        curEvent = TWIRE2; 
    }
    if(currentTrackLevel < NOTRACKWIRE2_THRESHOLD ){
        curEvent = NOTWIRE2;
    }
    if (curEvent != lastEvent) { // check for change from last time
        thisEvent.EventType = curEvent;
        thisEvent.EventParam = currentTrackLevel;
        returnVal = TRUE;
        lastEvent = curEvent; // update history
        PostTemplateHSM(thisEvent);  
    }
    return (returnVal);
    
}
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */

    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (IsTransmitEmpty()) {
            for (i = 0; i< sizeof (EventList) >> 2; i++) {
                if (EventList[i]() == TRUE) {
                    PrintEvent();
                    break;
                }

            }
        }
    }
}

void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
}
#endif