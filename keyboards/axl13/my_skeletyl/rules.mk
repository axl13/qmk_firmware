# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Set the debounce algorithm
DEBOUNCE_TYPE = asym_eager_defer_pk

# Disable unused features to save space
BOOTMAGIC_ENABLE = no
MOUSEKEY_ENABLE = no
EXTRAKEY_ENABLE = no
NKRO_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
SUSPEND_POWER_DOWN_ENABLE = no
MAGIC_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no

# Disable RGB features
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no

# Default layout
LAYOUTS = split_3x5_3
OLED_ENABLE = yes
LTO_ENABLE = yes
DEFERRED_EXEC_ENABLE = yes
COMBO_ENABLE = yes
