# Hand-wired split with OLED
SPLIT_KEYBOARD = yes
OLED_ENABLE = yes
LTO_ENABLE = yes
DEFERRED_EXEC_ENABLE = no

# Layout & typing features
COMBO_ENABLE = yes
EXTRAKEY_ENABLE = yes       # volume, play, brightness on the Function layer
AUTOCORRECT_ENABLE = yes    # dictionary: autocorrect_dict.txt
CAPS_WORD_ENABLE = yes      # hold both home-row Shifts (A + ;)
RAW_ENABLE = yes            # agent status lights: the Mac sets them over Raw HID
OS_DETECTION_ENABLE = yes   # auto Mac/Linux logo + Ctrl<->GUI swap

# Not used; saves AVR flash
TAP_DANCE_ENABLE = no
MOUSEKEY_ENABLE = no
NKRO_ENABLE = no
WPM_ENABLE = no
CONSOLE_ENABLE = no
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no
