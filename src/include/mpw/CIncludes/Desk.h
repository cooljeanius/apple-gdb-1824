/************************************************************

Created: Sunday, January 6, 1991 at 9:05 PM
    Desk.h
    C Interface to the Macintosh Libraries


        Copyright Apple Computer, Inc.    1985-1989
        All rights reserved

************************************************************/


#ifndef __DESK__
#define __DESK__

#ifndef __TYPES__
# include <Types.h>
#endif /* !__TYPES__ */

#ifndef __QUICKDRAW__
# include <Quickdraw.h>
#endif /* !__QUICKDRAW__ */

#ifndef __EVENTS__
# include <Events.h>
#endif /* !__EVENTS__ */


enum {

    accEvent = 64,
    accRun = 65,
    accCursor = 66,
    accMenu = 67,
    accUndo = 68,
    accCut = 70,
    accCopy = 71,
    accPaste = 72,
    accClear = 73,
    goodbye = -1    /*goodbye message*/
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
pascal short OpenDeskAcc(ConstStr255Param deskAccName)
#ifdef MPW_C
    = 0xA9B6
#endif /* MPW_C */
	;
pascal void CloseDeskAcc(short refNum)
#ifdef MPW_C
    = 0xA9B7
#endif /* MPW_C */
	;
pascal void SystemClick(const EventRecord *theEvent,WindowPtr theWindow)
#ifdef MPW_C
    = 0xA9B3
#endif /* MPW_C */
	;
pascal Boolean SystemEdit(short editCmd)
#ifdef MPW_C
    = 0xA9C2
#endif /* MPW_C */
	;
pascal void SystemTask(void)
#ifdef MPW_C
    = 0xA9B4
#endif /* MPW_C */
	;
pascal Boolean SystemEvent(const EventRecord *theEvent)
#ifdef MPW_C
    = 0xA9B2
#endif /* MPW_C */
	;
pascal void SystemMenu(long menuResult)
#ifdef MPW_C
    = 0xA9B5
#endif /* MPW_C */
	;
short opendeskacc(char *deskAccName);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__DESK__ */

/* EOF */
