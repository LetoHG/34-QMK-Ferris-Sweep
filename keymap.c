
#include QMK_KEYBOARD_H

extern bool g_suspend_state;
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;

typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

static tap dance_state = {
    .is_press_action = true,
    .step = 0
};

#define ADD_ACTION_TAP_DANCE(Name) ACTION_TAP_DANCE_FN_ADVANCED(on_##Name, Name##_finished, Name##_reset)

#define DEFINE_TAP_DANCE_SIMPLE(Name, TapCode, HoldCode) DEFINE_TAP_DANCE(Name, TapCode, HoldCode, TapCode, TapCode)
#define DEFINE_TAP_DANCE(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)      \
    void on_##Name(qk_tap_dance_state_t * state, void *user_data);                         \
    uint8_t Name##_dance_step(qk_tap_dance_state_t *state);                                \
    void Name##_finished(qk_tap_dance_state_t *state, void *user_data);                    \
    void Name##_reset(qk_tap_dance_state_t *state, void *user_data);                       \
    DEFINE_TAP_DANCE_ON(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)       \
    DEFINE_TAP_DANCE_STOP(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)     \
    DEFINE_TAP_DANCE_FINISHED(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode) \
    DEFINE_TAP_DANCE_RESET(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)

#define DEFINE_TAP_DANCE_ON(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode) \
    void on_##Name(qk_tap_dance_state_t * state, void *user_data) {                      \
        if (state->count == 3) {                                                         \
            tap_code16(TapCode);                                                         \
            tap_code16(TapCode);                                                         \
            tap_code16(TapCode);                                                         \
        }                                                                                \
        if (state->count > 3) {                                                          \
            tap_code16(TapCode);                                                         \
        }                                                                                \
    }

#define DEFINE_TAP_DANCE_STOP(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)    \
uint8_t Name##_dance_step(qk_tap_dance_state_t *state) {                                      \
    if (state->count == 1) {                                                                  \
        if (state->interrupted || !state->pressed) return SINGLE_TAP;                         \
        else return SINGLE_HOLD;                                                              \
    } else if (state->count == 2) {                                                           \
        if (state->interrupted) return DOUBLE_SINGLE_TAP;                                     \
        else if (state->pressed) return DOUBLE_HOLD;                                          \
        else return DOUBLE_TAP;                                                               \
    }                                                                                         \
    return MORE_TAPS;                                                                         \
}

#define DEFINE_TAP_DANCE_FINISHED(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)                  \
void Name##_finished(qk_tap_dance_state_t *state, void *user_data) {                                            \
    dance_state.step = Name##_dance_step(state);                                                                \
    switch (dance_state.step) {                                                                                 \
        case SINGLE_TAP: register_code16(TapCode); break;                                                       \
        case SINGLE_HOLD: register_code16(HoldCode); break;                                                     \
        case DOUBLE_TAP: register_code16(DoubleTapCode); register_code16(DoubleTapCode); break;                 \
        case DOUBLE_SINGLE_TAP: tap_code16(DoubleSingleTapCode); register_code16(DoubleSingleTapCode); break;   \
    }                                                                                                           \
}

#define DEFINE_TAP_DANCE_RESET(Name, TapCode, HoldCode, DoubleTapCode, DoubleSingleTapCode)         \
                    void Name##_reset(qk_tap_dance_state_t *state, void *user_data) {               \
                        wait_ms(10);                                                                \
                        switch (dance_state.step) {                                                 \
                            case SINGLE_TAP:unregister_code16(TapCode); break;                      \
                            case SINGLE_HOLD: unregister_code16(HoldCode); break;                   \
                            case DOUBLE_TAP: unregister_code16(DoubleTapCode); break;               \
                            case DOUBLE_SINGLE_TAP: unregister_code16(DoubleSingleTapCode); break;  \
                        }                                                                           \
                        dance_state.step = 0;                                                       \
                    }

enum tap_dance_codes {
  DOT_EXLM,
  EXLM_QUES,
  COMMA_MINUS,
  SLASH_UNDS,
  TAB_WINTAB,
  LEFT__CTRL_LEFT,
  RIGHT__CTRL_RIGHT,
  SL_BKSL,
  DQUOTE_QUOTE,
  HASH_AT,
  DLR_PERC,
  HEX_NUMPAD_1_A,
  HEX_NUMPAD_2_B,
  HEX_NUMPAD_3_C,
  HEX_NUMPAD_4_D,
  HEX_NUMPAD_5_E,
  HEX_NUMPAD_6_F,
  HEX_NUMPAD_0_X
};

DEFINE_TAP_DANCE_SIMPLE(COMMA_MINUS, KC_COMMA, KC_MINUS)
DEFINE_TAP_DANCE_SIMPLE(DOT_EXLM, KC_DOT, KC_EXLM)
DEFINE_TAP_DANCE_SIMPLE(EXLM_QUES, KC_EXLM, KC_QUES)
DEFINE_TAP_DANCE_SIMPLE(SLASH_UNDS, KC_SLASH, KC_UNDS)
DEFINE_TAP_DANCE_SIMPLE(TAB_WINTAB, KC_TAB, LGUI(KC_TAB))
DEFINE_TAP_DANCE_SIMPLE(LEFT__CTRL_LEFT, KC_LEFT, LCTL(KC_LEFT))
DEFINE_TAP_DANCE_SIMPLE(RIGHT__CTRL_RIGHT, KC_RIGHT, LCTL(KC_RIGHT))
DEFINE_TAP_DANCE_SIMPLE(SL_BKSL, KC_SLASH, KC_BSLS)
DEFINE_TAP_DANCE_SIMPLE(DQUOTE_QUOTE, KC_DQUO , KC_QUOT)
DEFINE_TAP_DANCE_SIMPLE(HASH_AT, KC_HASH, KC_AT)
DEFINE_TAP_DANCE_SIMPLE(DLR_PERC, KC_DLR, KC_PERC)

DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_1_A, KC_1, KC_A)
DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_2_B, KC_2, KC_B)
DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_3_C, KC_3, KC_C)
DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_4_D, KC_4, KC_D)
DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_5_E, KC_5, KC_E)
DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_6_F, KC_6, KC_F)
DEFINE_TAP_DANCE_SIMPLE(HEX_NUMPAD_0_X, KC_0, KC_X)

qk_tap_dance_action_t tap_dance_actions[] = {
        [DOT_EXLM] = ADD_ACTION_TAP_DANCE(DOT_EXLM),
        [EXLM_QUES] = ADD_ACTION_TAP_DANCE(EXLM_QUES),
        [COMMA_MINUS] = ADD_ACTION_TAP_DANCE(COMMA_MINUS),
        [SLASH_UNDS] = ADD_ACTION_TAP_DANCE(SLASH_UNDS),
        [TAB_WINTAB] = ADD_ACTION_TAP_DANCE(TAB_WINTAB),
        [LEFT__CTRL_LEFT] = ADD_ACTION_TAP_DANCE(LEFT__CTRL_LEFT),
        [RIGHT__CTRL_RIGHT] = ADD_ACTION_TAP_DANCE(RIGHT__CTRL_RIGHT),
        [SL_BKSL] = ADD_ACTION_TAP_DANCE(SL_BKSL),
        [DQUOTE_QUOTE] = ADD_ACTION_TAP_DANCE(DQUOTE_QUOTE),
        [HASH_AT] = ADD_ACTION_TAP_DANCE(HASH_AT),
        [DLR_PERC] = ADD_ACTION_TAP_DANCE(DLR_PERC),
        [HEX_NUMPAD_1_A] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_1_A),
        [HEX_NUMPAD_2_B] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_2_B),
        [HEX_NUMPAD_3_C] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_3_C),
        [HEX_NUMPAD_4_D] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_4_D),
        [HEX_NUMPAD_5_E] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_5_E),
        [HEX_NUMPAD_6_F] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_6_F),
        [HEX_NUMPAD_0_X] = ADD_ACTION_TAP_DANCE(HEX_NUMPAD_0_X)
};

#define DE_ADIA     UC(0x00E4)
#define DE_SS       UC(0x00DF)
#define DE_ODIA     UC(0x00F6)
#define DE_UDIA     UC(0x00FC)

enum {
    _ALPHA,
    _SPECIAL,
    _NUMBERS,
    _NAVIGATION,
    _FUNCTION,
    _MOUSE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_ALPHA] = LAYOUT(
  //----------------------------------------------------------------------------       ---------------------------------------------------------------------------------------
      KC_Y,     KC_C,     KC_L,                 KC_M,                 KC_K,                KC_Z,       KC_F,                KC_U,               TD(COMMA_MINUS),     KC_BSPC,
  //----------------------------------------------------------------------------       ---------------------------------------------------------------------------------------
      KC_I,     KC_S,     MT(MOD_LALT,KC_R),    MT(MOD_LGUI,KC_T),    KC_G,                KC_P,       MT(MOD_RGUI,KC_N),   MT(MOD_RALT,KC_E),  KC_A,               KC_O,
  //----------------------------------------------------------------------------       ---------------------------------------------------------------------------------------
      KC_Q,     KC_V,     KC_W,                 KC_D,                 KC_J,                KC_B,       KC_H,                TD(SLASH_UNDS),     TD(DOT_EXLM),       KC_X,
  //----------------------------------------------------------------------------       ---------------------------------------------------------------------------------------
                                                TO(_SPECIAL),      OSM(MOD_LCTL),          OSM(MOD_LSFT), KC_SPACE
  //----------------------------------------------------------------------------       ---------------------------------------------------------------------------------------
  ),

  [_SPECIAL] = LAYOUT(
  //---------------------------------------------------------------------------------------      ---------------------------------------------------------------------------------
    KC_ESCAPE,      KC_UNDS,          KC_LBRC,      KC_RBRC,               KC_CIRC,                  TD(EXLM_QUES),    KC_LABK,        KC_RABK,           KC_EQUAL,       KC_BSPC,
  //---------------------------------------------------------------------------------------      ---------------------------------------------------------------------------------
    KC_TAB,         TD(SL_BKSL),      KC_LCBR,      KC_RCBR,               KC_ASTR,                  KC_MINUS,         KC_LPRN,        KC_RPRN,           KC_SCOLON,      KC_ENTER,
  //---------------------------------------------------------------------------------------      ---------------------------------------------------------------------------------
    TD(HASH_AT),    TD(DLR_PERC),     KC_PIPE,      KC_TILD,               KC_GRV,                   KC_PLUS,          KC_AMPR,        TD(DQUOTE_QUOTE),  KC_COLON,       TO(_NAVIGATION),
  //---------------------------------------------------------------------------------------      ---------------------------------------------------------------------------------
                                                    TO(_NUMBERS),          LALT(KC_LCTL),            OSM(MOD_LSFT),    TO(_ALPHA)
  //---------------------------------------------------------------------------------------      ---------------------------------------------------------------------------------
  ),

  [_NUMBERS] = LAYOUT(
  //-----------------------------------------------------------------------------------------------------------------       ----------------------------------------------------
    KC_ESCAPE,              KC_MEDIA_PLAY_PAUSE,    KC_MEDIA_PREV_TRACK,    KC_MEDIA_NEXT_TRACK,    KC_SLASH,                 KC_MINS,            KC_7,               KC_8,               KC_9,                 KC_TRANSPARENT,
  //-----------------------------------------------------------------------------------------------------------------       ----------------------------------------------------
    KC_TAB,                 KC_LALT,                KC_LSFT,                KC_LCTL,                KC_ASTR,                  KC_PLUS,            TD(HEX_NUMPAD_4_D), TD(HEX_NUMPAD_5_E), TD(HEX_NUMPAD_6_F),   KC_ENTER,
  //-----------------------------------------------------------------------------------------------------------------       ----------------------------------------------------
    KC_BRIGHTNESS_DOWN,     KC_BRIGHTNESS_UP,       KC_AUDIO_VOL_DOWN,      KC_AUDIO_VOL_UP,        KC_KP_DOT,                TD(HEX_NUMPAD_0_X), TD(HEX_NUMPAD_1_A), TD(HEX_NUMPAD_2_B), TD(HEX_NUMPAD_3_C),   TO(_FUNCTION),
  //-----------------------------------------------------------------------------------------------------------------       ----------------------------------------------------
                                                                            TO(_NUMBERS),           KC_LGUI,                  OSM(MOD_LSFT),      TO(_ALPHA)
  //-----------------------------------------------------------------------------------------------------------------       ----------------------------------------------------
  ),

  [_NAVIGATION] = LAYOUT(
  //-----------------------------------------------------------------------       ----------------------------------------------------------------------------------------------
    KC_ESC,           KC_NO,       KC_WBAK,       KC_WFWD,      KC_NO,                 KC_PGUP,     KC_HOME,                KC_END,     KC_PGDOWN,              KC_TRANSPARENT,
  //-----------------------------------------------------------------------       ----------------------------------------------------------------------------------------------
    KC_TAB,           KC_LALT,     KC_LSFT,       KC_LCTL,      LCTL(KC_S),            KC_LEFT,     KC_DOWN,                KC_UP,      KC_RIGHT,               KC_ENTER,
  //-----------------------------------------------------------------------       ----------------------------------------------------------------------------------------------
    LCTL(KC_Z),       LCTL(KC_X),  LCTL(KC_C),    LCTL(KC_V),   LCTL(KC_Y),            KC_NO,       TO(_MOUSE),             KC_NO,      KC_NO,                  KC_DELETE,
  //-----------------------------------------------------------------------       ----------------------------------------------------------------------------------------------
                                                  TO(_SPECIAL), KC_LGUI,               KC_TRANSPARENT,  TO(_ALPHA)
  //-----------------------------------------------------------------------       ----------------------------------------------------------------------------------------------
  ),

  [_FUNCTION] = LAYOUT(
  //--------------------------------------------------------       ----------------------------------------------------------
    KC_ESC,   KC_NO,     KC_NO,     KC_NO,        KC_NO,               KC_NO,      KC_F1,     KC_F2,     KC_F3,     KC_F4,
  //--------------------------------------------------------       ----------------------------------------------------------
    KC_NO,    KC_LALT,   KC_LSFT,   KC_LCTL,      KC_NO,               KC_NO,      KC_F5,     KC_F6,     KC_F7,     KC_F8,
  //--------------------------------------------------------       ----------------------------------------------------------
    KC_NO,    KC_NO,     KC_NO,     KC_NO,        KC_NO,               KC_NO,      KC_F9,      KC_F10,    KC_F11,    KC_F12,
  //--------------------------------------------------------       ----------------------------------------------------------
                                    TO(_SPECIAL), KC_LALT,             KC_TRANSPARENT, TO(_ALPHA)
  //--------------------------------------------------------       ----------------------------------------------------------
  ),

  [_MOUSE] = LAYOUT(
  //---------------------------------------------------------------       ----------------------------------------------------------
    KC_ESC,   KC_NO,        KC_MS_UP,      KC_NO,          KC_NO,                KC_MS_WH_UP,      KC_LCTL,       KC_LSFT,        KC_LALT,       KC_NO,
  //---------------------------------------------------------------       ----------------------------------------------------------
    KC_NO,    KC_MS_LEFT,   KC_MS_DOWN,    KC_MS_RIGHT,    KC_NO,                KC_MS_WH_DOWN,    KC_MS_BTN1,    KC_MS_BTN2,     KC_NO,         KC_NO,
  //---------------------------------------------------------------       ----------------------------------------------------------
    KC_NO,    KC_MS_BTN3,   KC_MS_WH_LEFT, KC_MS_WH_RIGHT, KC_NO,                KC_NO,            KC_MS_ACCEL0,  KC_MS_ACCEL1,  KC_MS_ACCEL2,   KC_NO,
  //---------------------------------------------------------------       ----------------------------------------------------------
                                           TO(_SPECIAL),   KC_LALT,              KC_TRANSPARENT,   TO(_ALPHA)
  //---------------------------------------------------------------       ----------------------------------------------------------
  )

//   [3] = LAYOUT(
//   //---------------------------------------------------------------------------------------       ---------------------------------------------------------------------------------------------------------------
//     KC_ESC,           KC_MS_WH_LEFT,  KC_MS_UP,       KC_MS_WH_RIGHT,     KC_NO,                    LGUI(KC_LBRACKET),      LCTL(LSFT(KC_TAB)),     RCTL(KC_TAB),           LGUI(KC_RBRACKET),      KC_TRANSPARENT,
//   //---------------------------------------------------------------------------------------       ---------------------------------------------------------------------------------------------------------------
//     TD(TAB_WINTAB),   KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,        MT(MOD_LGUI,KC_DEL),      TD(LEFT__CTRL_LEFT),    MT(MOD_RGUI,KC_DOWN),   MT(MOD_RALT,KC_UP),     TD(RIGHT__CTRL_RIGHT),  KC_ENTER,
//   //---------------------------------------------------------------------------------------       ---------------------------------------------------------------------------------------------------------------
//     KC_NO,            KC_MS_BTN2,     KC_MS_WH_UP,    KC_MS_WH_DOWN,      KC_TRANSPARENT,           KC_MS_BTN1,             KC_MS_ACCEL0,           KC_MS_ACCEL1,           KC_MS_ACCEL2,           KC_NO,
//   //---------------------------------------------------------------------------------------       ---------------------------------------------------------------------------------------------------------------
//                                                       KC_LALT,            TO(_SPECIAL),                    TO(0),                  KC_TRANSPARENT
//   //---------------------------------------------------------------------------------------       ---------------------------------------------------------------------------------------------------------------
//   ),
};
