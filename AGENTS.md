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
- the OLED code in `keymaps/default/keymap.c` (OS pod, WPM, keystroke trace,
  autocorrect toast, layer view, idle scope)

## Rule: always keep the QMK and ZMK Skeletyl in sync

The owner types on two Skeletyls: this hand-wired QMK one and a wireless ZMK
one (<https://github.com/axl13/zmk-config-bkb>, `config/skeletyl_v2.keymap`).
Any change to layers, combos, tap-hold behaviour or timing made in one repo
**must** be made in the other in the same piece of work, with both diagrams
regenerated. Don't report a layout change as done until both repos have it.
The ZMK repo on GitHub is the tested reference; start from its `master`, not
from an old local clone.

| Feature | QMK (`keymaps/default/keymap.c`, `config.h`) | ZMK (`config/skeletyl_v2.keymap`) |
|---|---|---|
| Positional home-row mods | `CHORDAL_HOLD` + `PERMISSIVE_HOLD`, `TAPPING_TERM 200` | `bhm_*` / `bhs_*`: `balanced`, `hold-trigger-key-positions`, 200 ms |
| Typing window (mod pressed right after a key = tap) | `FLOW_TAP_TERM 150`; 80 for Shift in `get_flow_tap_term()` | `require-prior-idle-ms` 150 on `bhm_*`, 80 on `bhs_*` |
| Caps Word, hold `A` + `;` | `BOTH_SHIFTS_TURNS_ON_CAPS_WORD` | `combo_caps` + `cw_ht` |
| Combo windows | `COMBO_TERM 40`; 25 for `I+O`, `A+S`; `COMBO_PRIOR_IDLE_MS 150` for `O+P`, `.+/` | `timeout-ms`, `require-prior-idle-ms` per combo |
| Layer-tap thumbs | `get_quick_tap_term()` 175 | `&lt` `quick-tap-ms` 175 |
| Sentence case | `getreuer/sentence_case` module | `zmk-sentence-case` module |

Intentional differences (do not "fix" them):

- Function-layer hardware keys. ZMK: Bluetooth, Studio unlock, ext power, soft
  off. QMK: autocorrect toggle (Fn+;), OS mode (Fn+Z), sentence-case toggle
  (Fn+G).
- QMK only: Autocorrect, OS detection with the Ctrl/Cmd swap, the OLED.

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
4. Commit and push, then start the **Build Skeletyl Firmware** workflow by
   hand (Actions tab -> Run workflow, or a `workflow_dispatch` API call) and
   confirm it succeeds. Pushes do not start a build. It builds against the
   latest QMK release tag. Check the firmware size in the job summary; the
   ATmega32U4 has 28 KB with Caterina. Batch changes: one build per set of
   changes, not one per commit.

Never hand-edit the generated `assets/my_skeletyl_keymap.*` files.

## Build notes

- GitHub Actions on this fork: only `build.yml` is enabled and it is
  manual-only. Every workflow inherited from upstream QMK is disabled in the
  repository settings (Actions tab). After syncing with upstream, disable any
  new workflow it brings in. GitHub switched Actions off for the whole fork
  once (2026-11-10), citing compute wasted by workflows inherited from
  upstream.

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
