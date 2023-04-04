// Copyright 2016-2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <stdlib.h>

#include "doomdef.h"
#include "doomtype.h"
#include "m_argv.h"
#include "d_event.h"
#include "g_game.h"
#include "d_main.h"
#include "gamepad.h"
#include "lprintf.h"

#include "psxcontroller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


//The gamepad uses keyboard emulation, but for compilation, these variables need to be placed
//somewhere. THis is as good a place as any.
int usejoystick=0;
int joyleft, joyright, joyup, joydown;


//atomic, for communication between joy thread and main game thread
volatile int joyVal=0;

typedef struct {
	int ps2mask;
	int *key;
} JsKeyMap;

//Mappings from PS2 buttons to keys
static const JsKeyMap keymap[]={
	{0x10, &key_up},
	{0x40, &key_down},
	{0x80, &key_left},
	{0x20, &key_right},
	
	{0x4000, &key_use},				//cross
	{0x2000, &key_fire},			//circle
	{0x2000, &key_menu_enter},		//circle
	{0x8000, &key_pause},			//square
	{0x1000, &key_weapontoggle},	//triangle

	{0x8, &key_escape},				//start
	{0x1, &key_map},				//select
	
	{0x400, &key_strafeleft},		//L1
	{0x100, &key_speed},			//L2
	{0x800, &key_straferight},		//R1
	{0x200, &key_strafe},			//R2

	{0, NULL},
};


void gamepadPoll(void)
{
	static int oldPollJsVal=0xffff;
	int newJoyVal=joyVal;
	event_t ev;

	for (int i=0; keymap[i].key!=NULL; i++) {
		if ((oldPollJsVal^newJoyVal)&keymap[i].ps2mask) {
			ev.type=(newJoyVal&keymap[i].ps2mask)?ev_keyup:ev_keydown;
			ev.data1=*keymap[i].key;
			D_PostEvent(&ev);
		}
	}

	oldPollJsVal=newJoyVal;
}


void jsTask(void *arg) {
	int oldJoyVal=0xFFFF;
	printf("Joystick task starting.\n");
	while(1) {
		vTaskDelay(20/portTICK_PERIOD_MS);
		joyVal=psxReadInput();
//		if (joyVal!=oldJoyVal) printf("Joy: %x\n", joyVal^0xffff);
		oldJoyVal=joyVal;
	}
}

void gamepadInit(void)
{
	lprintf(LO_INFO, "gamepadInit: Initializing game pad.\n");
}

void jsInit() {
	//Starts the js task
	psxcontrollerInit();
	xTaskCreatePinnedToCore(&jsTask, "js", 5000, NULL, 7, NULL, 0);
}

