/*
 * input.c
 *
 *  Created on: 14. okt. 2019
 *      Author: Eilif
 */

#include "input.h"
#include "gpio.h"
#include "usbhost.h"
#include "defines.h"
#include "fpga.h"

static bool last_button_state[GPIO_BTN_COUNT] = {0};
static MIDI_packet keydown_to_midi[16] = {
		{{0x90, 0x30, 0x7f}},
		{{0x90, 0x31, 0x7f}},
		{{0x90, 0x32, 0x7f}},
		{{0x90, 0x33, 0x7f}},
		{{0x90, 0x34, 0x7f}},
		{{0x90, 0x35, 0x7f}},
		{{0x90, 0x36, 0x7f}},
		{{0x90, 0x37, 0x7f}},
		{{0x90, 0x38, 0x7f}},
		{{0x90, 0x39, 0x7f}},
		{{0x90, 0x3a, 0x7f}},
		{{0x90, 0x3b, 0x7f}},
		{{0x90, 0x3c, 0x7f}},
		{{0x90, 0x3d, 0x7f}},
		{{0x90, 0x3e, 0x7f}},
		{{0x90, 0x3f, 0x7f}}
};
static MIDI_packet keyup_to_midi[16] = {
		{{0x80, 0x30, 0x00}},
		{{0x80, 0x31, 0x00}},
		{{0x80, 0x32, 0x00}},
		{{0x80, 0x33, 0x00}},
		{{0x80, 0x34, 0x00}},
		{{0x80, 0x35, 0x00}},
		{{0x80, 0x36, 0x00}},
		{{0x80, 0x37, 0x00}},
		{{0x80, 0x38, 0x00}},
		{{0x80, 0x39, 0x00}},
		{{0x80, 0x3a, 0x00}},
		{{0x80, 0x3b, 0x00}},
		{{0x80, 0x3c, 0x00}},
		{{0x80, 0x3d, 0x00}},
		{{0x80, 0x3e, 0x00}},
		{{0x80, 0x3f, 0x00}}
};

MIDI_packet waitForInput(){
	USB_output usb_out = USBWaitForData();
	MIDI_packet midi_out;
	midi_out.data[0] = usb_out.data[1];
	midi_out.data[1] = usb_out.data[2];
	midi_out.data[2] = usb_out.data[3];
	return midi_out;
}

void handleMultipleButtonPresses(MicrocontrollerGeneratorState** generator_states){
	for(int i = 0; i < GPIO_BTN_COUNT; i++){
		if(last_button_state[i] != isButtonDown(i)){
			last_button_state[i] = isButtonDown(i);
			if(isButtonDown(i))
				handleMIDIEvent(&keydown_to_midi[i], generator_states);
			else
				handleMIDIEvent(&keyup_to_midi[i], generator_states);
		}
	}
}