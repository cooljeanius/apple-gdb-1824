/************************************************************

Created: Sunday, January 6, 1991 at 9:34 PM
    OSEvents.h
    C Interface to the Macintosh Libraries


        Copyright Apple Computer, Inc.    1985-1990
        All rights reserved

************************************************************/


#ifndef __OSEVENTS__
#define __OSEVENTS__ 1

#ifndef __TYPES__
# include <Types.h>
#endif /* !__TYPES__ */

#ifndef __EVENTS__
# include <Events.h>
#endif /* !__EVENTS__ */

#ifndef __OSUTILS__
# include <OSUtils.h>
#endif /* !__OSUTILS__ */

#ifndef __EVENTS__
struct EvQEl {
    QElemPtr qLink;
    short qType;
    short evtQWhat;     /*this part is identical to the EventRecord as...*/
    long evtQMessage;   /*defined in ToolIntf*/
    long evtQWhen;
    Point evtQWhere;
    short evtQModifiers;
};

typedef struct EvQEl EvQEl;
typedef EvQEl *EvQElPtr;
#endif /* !__EVENTS__ */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifndef __EVENTS__
# pragma parameter __D0 PostEvent(__A0,__D0)
pascal OSErr PostEvent(short eventNum, long eventMsg)
# ifdef MPW_C
    = 0xA02F
# endif /* MPW_C */
	;
#endif /* !__EVENTS__ */
#ifdef PRAGMA_PARAMETER
# pragma parameter __D0 PPostEvent(__A0,__D0,__A1)
#endif /* PRAGMA_PARAMETER */
pascal OSErr PPostEvent(short eventCode, long eventMsg,EvQElPtr *qEl)
#ifdef MPW_C
    = {0xA12F,0x2288}
#endif /* MPW_C */
	;
#ifdef PRAGMA_PARAMETER
# pragma parameter __D0 OSEventAvail(__D0,__A0)
#endif /* PRAGMA_PARAMETER */
pascal Boolean OSEventAvail(short mask, EventRecord *theEvent)
#ifdef MPW_C
    = {0xA030,0x5240}
#endif /* MPW_C */
	;
#ifdef PRAGMA_PARAMETER
# pragma parameter __D0 GetOSEvent(__D0,__A0)
#endif /* PRAGMA_PARAMETER */
pascal Boolean GetOSEvent(short mask, EventRecord *theEvent)
#ifdef MPW_C
    = {0xA031,0x5240}
#endif /* MPW_C */
	;
#ifndef __EVENTS__
pascal void FlushEvents(short whichMask, short stopMask)
# ifdef MPW_C
    = {0x201F,0xA032}
# endif /* MPW_C */
	;
# endif /* !__EVENTS__ */
#ifndef __EVENTS__
# pragma parameter SetEventMask(__D0)
pascal void SetEventMask(short theMask)
# ifdef MPW_C
    = {0x31C0,0x0144}
# endif /* MPW_C */
	;
#endif /* !__EVENTS__ */
#define GetEvQHdr() ((QHdrPtr)0x014A)
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__OSEVENTS__ */

/* EOF */
