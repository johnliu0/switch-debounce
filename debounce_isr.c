/**
 * Switch debounce with interrupts logic code.
 */

#define POLLING_PERIOD 1
#define DEB_DELAY_CTR_LIMIT 50
#define SWITCH_ADDR 0xABCDEF00

// current debounced switch state
int cur_state = 0;

// volatile as the deb_timer is modified in the ISR
// prevents compiler from optimizing update_switch by
// removing the deb_timer if statement check
volatile int deb_timer = 0;

/* Gets the state of the device memory mapped to the given address. */
int poll_device(const unsigned int mem_addr);

/* ISR to handle the switch event. Start the debounce timer when the interrupt is called. */
void switch_isr_handler() {
    deb_timer = 1;
}

/* If the debounce timer has started due to the ISR, then increment the timer
 * until it reaches the specified delay before toggling the switch state.
 * The interrupt will be called multiple times due to button bouncing,
 * however each call to the ISR will reset the timer. When the ISR
 * has not been called for more than 50ms, the state can finally toggle,
 * and the timer can be stopped and reset. */
int update_switch() {
    if (!deb_timer) {
        deb_timer++;
        if (deb_timer == DEB_DELAY_CTR_LIMIT + 1) {
            deb_timer = 0;
            // take the polled state of the device as the current state
            // the polled state has remained in that state for at least the debounce time
            cur_state = poll_device(SWITCH_ADDR);
        }
    }
}





/* elsewhere, the main loop must still periodically call update_switch();
 * however, the CPU may idle while waiting for the interrupt */
void loop() {
    while(1) {
        // MCU can idle here while waiting for a button press

        // if MCU is awake, then call update_switch every 1ms
        if (update_switch()) {
            // if the switch has finished changing state,
            // let the MCU know that it can go back to idling
        }
    }
}