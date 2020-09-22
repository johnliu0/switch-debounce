/**
 * Switch debounce logic code.
 * It is assumed that the poll_device function exists to read
 * the current state of the hardware device and that the poll_switch is run every 1ms.
 * The debounced output is reflected in cur_state.
 */

#include <stdlib.h>
#include <time.h>

/* Gets the state of the device memory mapped to the given address. */
int poll_device(const unsigned int mem_addr);

// memory mapped location of switch
const unsigned int SWITCH_ADDR = 0xABCDEF00;

// current debounced switch state, assumed LOW to begin with 
int cur_state = 0;

// debounce delay in ms
const unsigned int deb_delay = 50;

// counter to keep track of time since a switch toggle
int delay_ctr;

/* Polls the switch and saves the output state of the button into cur_state.
 * It is assumed this function is called from a loop that executes every 1ms. */
void poll_switch() {
	int polled_state = poll_device(SWITCH_ADDR);

	// check if the switch is potentially being toggled
	if (polled_state != cur_state) {
		// if it is, keep track of the time it is under this new state
		delay_ctr++;
	} else {
		// reset the counter if the reading goes back to the current state
		delay_ctr = 0;
	}

	// if the polled state has been different from the current state
	// for enough time, then the state can be changed
	if (delay_ctr >= deb_delay) {
		cur_state = !cur_state;
		delay_ctr = 0;
	}
}
