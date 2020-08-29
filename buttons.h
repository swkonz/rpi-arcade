#ifndef BUTTONS_H
#define BUTTONS_H

/**
 * interrupts_setup: Setup all interrupt pins for the buttons. Does not setup 
 *      handlers for any of the interrupts. By default, all pins use a default 
 *      handler
 * 
 *  !Note: Uses pins defined in global constants file
 */
void interrupts_setup ();

#endif