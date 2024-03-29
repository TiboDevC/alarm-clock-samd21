#include "alarm_clock_fsm.h"

#include "button/button.h"
#include "debug.h"
#include "drv/screen/DEV_Config.h"
#include "drv/timer/timer.h"
#include "flash_storage/alarm_flash_storage.h"
#include "rtc/rtc_tool.h"
#include "ui/screen.h"
#include "wifi/wifi_tool.h"

enum fsm_handler_rc state_init(struct fsm *fsm, struct fsm_event const *event)
{
	enum alarm_clock_fsm_event_type event_alarm_clock = event->type & ALARM_CLOCK_FSM_EVT_MSK;

	switch (event_alarm_clock) {
	case FSM_EVENT_INIT:
		debug("[state_init] event_init\n");
		init_alarm_flash_storage();
		setup_gclk();
		init_buttons();

		DEV_Module_Init();
		init_screen();
		display_hello_msg();

		init_rtc();

		rtc_start_interrupt();

		debug("[state_init] event_init ends\n");

		fsm->state = (fsm_event_handler_t) &state_clock_display;
		return FSM_RC_TRANSITION;
	default:
		break;
	}
	return FSM_IGNORED();
}