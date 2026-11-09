# Agent instructions (axl13 fork of qmk_firmware)

This is a fork of upstream QMK. The **only** directory that belongs to the
owner is `keyboards/axl13/my_skeletyl/` (plus `.github/workflows/build.yml`).
Do not edit upstream QMK files.

The board is a **hand-wired Skeletyl** (ATmega32U4, Caterina, soft-serial on
D2, SH1106 OLED). It runs the same layout as the ZMK Skeletyl in
<https://github.com/axl13/zmk-config-bkb>; keep the two in sync, and follow
that repo's AGENTS.md design rules (Kinesis Advantage muscle memory,
opposite-hand Shift, layers on thumbs, same-row combos).

## Never change the hardware definition

These describe the physical wiring and OLED. Do not modify them unless the
owner explicitly asks:

- `keyboards/axl13/my_skeletyl/keyboard.json` (matrix pins, diode direction, split, OLED driver)
- `keyboards/axl13/my_skeletyl/rules.mk` (MCU, bootloader, debounce)
- in `keymaps/default/config.h`, the block marked "Hand-wired split + OLED"
- the OLED code in `keymaps/default/keymap.c` (histogram, average tap time,
  idle wave, `RESET_STATS`)

## Rule: every layout change ships with a fresh printable diagram

Whenever `keymaps/default/keymap.c` (layers, combos, tap-hold) changes, in the
same commit:

1. Run `keyboards/axl13/my_skeletyl/scripts/draw_keymap.sh`. It regenerates
   `keyboards/axl13/my_skeletyl/assets/my_skeletyl_keymap.{yaml,svg,png,pdf}`
   (the PDF is the printable one-page A4 version).
2. Open the PNG and check it: all layers, combos between the right keys, no
   raw QMK names where a symbol is expected. If one shows, add it to `LABELS`
   in `scripts/keymap_c_to_yaml.py` and re-run.
3. Update `keyboards/axl13/my_skeletyl/readme.md` if combos or layers changed.
4. Commit and push, then confirm the **Build Skeletyl Firmware** workflow
   succeeds. It builds against the latest QMK release tag. Check the firmware
   size in the job summary; the ATmega32U4 has 28 KB with Caterina.

Never hand-edit the generated `assets/my_skeletyl_keymap.*` files.

## Build notes

- CI checks out `qmk/qmk_firmware` at the newest release tag, overlays
  `keyboards/axl13/`, and adds `getreuer/qmk-modules` (sentence case) pinned
  by `GETREUER_MODULES_SHA` in the workflow.
- Local build: in a QMK checkout at a release tag, copy `keyboards/axl13`,
  clone getreuer/qmk-modules to `modules/getreuer`, then
  `qmk compile -kb axl13/my_skeletyl -km default`.
- Rollback: the pre-v3 firmware that was verified on the board (flash
  read-back, 2026-11-09) is in history at commit `9004294c12`, path
  `keyboards/axl13/my_skeletyl/keymaps/default/keyboards/axl13/my_skeletyl/`
  (sm_td keymap). It is intentionally not kept in the tree.
- Flash the **same hex to both halves**; the split protocol changes between
  QMK versions and a half left on an old build stops responding.
