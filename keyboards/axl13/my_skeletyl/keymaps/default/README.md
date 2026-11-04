# Skeletyl ZMK keymap port

This is the QMK port of `config/skeletyl_v2.keymap` from `axl13/zmk-config-bkb`.
The custom hand-wired matrix in the parent keyboard definition is unchanged.

The ZMK Bluetooth profile, external-power toggle, and soft-off bindings were
left unassigned because this keyboard is wired and uses an ATmega32U4. ZMK's
third function layer is QMK layer 2; the old ZMK `&mo 3` reference is mapped to
that same layer so it works with the three-layer keymap.

The previous QMK keymap is preserved under `keymaps/legacy/` for reference.
