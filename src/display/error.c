#include "display/lvgl.h"
#include "ifi/v5_api.h"
#include "kapi.h"

void display_fatal_error(const char* text) {
	// in fatal error state, cannot rely on integrity of the RTOS
	char s[50];
	strncpy(s, text, 50);
	if (lcd_is_initialized()) {
		vexDisplayForegroundColor(COLOR_RED);
		vexDisplayRectFill(0, 0, 480, 19);
		vexDisplayRectFill(0, 0, 27, 240);
		vexDisplayRectFill(453, 0, 480, 240);
		vexDisplayRectFill(0, 179, 480, 240);
		vexDisplayForegroundColor(0x1A1917);
		vexDisplayRectFill(50, 190, 130, 230);
		vexDisplayRectFill(200, 190, 280, 230);
		vexDisplayRectFill(350, 190, 430, 230);
		vexDisplayCenteredString(0, s);
	} else {
		vexDisplayForegroundColor(COLOR_RED);
		vexDisplayRectFill(0, 0, 480, 240);
		vexDisplayForegroundColor(COLOR_WHITE);
		vexDisplayCenteredString(6, s);
	}
}

static lv_obj_t* _window;
static lv_obj_t* _warning_label;

static lv_res_t _clear_warning(lv_obj_t* btn) {
	lv_obj_set_hidden(lv_win_get_from_btn(btn), true);
	return LV_RES_OK;
}
void display_error(const char* text) {
	// proceed iff there's not another error showing and there's an error message
	// to show
	if (!text[0]) {
		if (_window) {
			lv_obj_set_hidden(_window, true);
		}
		return;
	}
	if (!_window) {
		_window = lv_win_create(lv_scr_act(), NULL);
		lv_obj_set_size(_window, 480, 240);
		lv_win_add_btn(_window, SYMBOL_CLOSE, _clear_warning);
		lv_win_set_title(_window, SYMBOL_WARNING " WARNING " SYMBOL_WARNING);

		_warning_label = lv_label_create(_window, NULL);
	}
	lv_label_set_text(_warning_label, text);
	lv_obj_set_hidden(_window, false);
}