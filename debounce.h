#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "stdbool.h"

/**
 * debounce_init: Initialize this pin to be debounced. Uses time as a debounce mechanism
 * 
 * !Warning: Assumes only GPIO Pins AKA 0 - 53
 * 
 *  @param pin: The pin number to be debounced
 */
void debounce_init (unsigned pin);

/**
 * debounce: Check if this pin is in a valid state to receive new interrupts
 *  @param pin: The pin number to be debounced
 */
bool debounce (unsigned pin);

#endif