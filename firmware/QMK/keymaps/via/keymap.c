// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _L1,
    _L2,
    _L3,
};

enum custom_keycodes {
    ENC_PRESS = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_SPC, KC_D, KC_F,
        KC_J, KC_K, ENC_PRESS
    ),

    [_L1] = LAYOUT(
        KC_Z, KC_SPC, KC_X,
        KC_SPC, KC_SPC, ENC_PRESS
    ),

    [_L2] = LAYOUT(
        KC_F1, KC_F2, KC_F3,
        KC_F4, KC_F5, ENC_PRESS
    ),

    [_L3] = LAYOUT(
        KC_1, KC_2, KC_3,
        KC_4, KC_5, ENC_PRESS
    )
};

const uint16_t PROGMEM encoder_map[][1][2] = {
    [0] = { ENCODER_CCW_CW(KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN) },
    [1] = { ENCODER_CCW_CW(KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN) },
    [2] = { ENCODER_CCW_CW(KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN) },
    [3] = { ENCODER_CCW_CW(KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN) },
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == ENC_PRESS && record->event.pressed) {
        uint8_t layer = biton32(layer_state);
        switch (layer) {
            case _BASE:
                layer_move(_L1);
                break;
            case _L1:
                layer_move(_L2);
                break;
            case _L2:
                layer_move(_L3);
                break;
            case _L3:
                layer_move(_BASE);
                break;
            default:
                layer_move(_BASE);
                break;
        }
        return false;
    }
    return true;
}

static void set_layer_rgb(uint8_t layer) {
    switch (layer) {
        case _BASE:
            // Base: off
            rgblight_setrgb(0x00, 0x00, 0x00);
            break;
        case _L1:
            // Layer 1: red
            rgblight_setrgb(0xFF, 0x00, 0x00);
            break;
        case _L2:
            // Layer 2: blue
            rgblight_setrgb(0x00, 0x00, 0xFF);
            break;
        case _L3:
            // Layer 3: green
            rgblight_setrgb(0x00, 0xFF, 0x00);
            break;
        default:
            rgblight_setrgb(0x00, 0x00, 0x00);
            break;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    set_layer_rgb(layer);
    return state;
}

void keyboard_post_init_user(void) {
    rgblight_enable();
    set_layer_rgb(get_highest_layer(layer_state));
}

// #ifdef OLED_ENABLE
// bool oled_task_user(void) {
//     // Host Keyboard Layer Status
//     oled_write_P(PSTR("Layer: "), false);

//     switch (get_highest_layer(layer_state)) {
//         case _BASE:
//             oled_write_P(PSTR("Default\n"), false);
//             break;
//         default:
//             // Or use the write_ln shortcut over adding '\n' to the end of your string
//             oled_write_ln_P(PSTR("Undefined"), false);
//     }

//     // Host Keyboard LED Status
//     led_t led_state = host_keyboard_led_state();
//     oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
//     oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
//     oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    
//     return false;
// }
// #endif