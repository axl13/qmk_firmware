#!/usr/bin/env bash
# Render the my_skeletyl keymap diagram from keymaps/default/keymap.c.
#
# Outputs (all committed):
#   assets/my_skeletyl_keymap.yaml   keymap-drawer input (generated)
#   assets/my_skeletyl_keymap.svg    shown in the readme
#   assets/my_skeletyl_keymap.png    2x raster
#   assets/my_skeletyl_keymap.pdf    printable, A4 portrait, one page
#
# Requirements: python3; Google Chrome or Chromium for PNG/PDF.
# keymap-drawer is installed into ./.venv on first run.
set -euo pipefail

KB="$(cd "$(dirname "$0")/.." && pwd)"
cd "$KB"

KEYMAP=keymaps/default/keymap.c
CFG=assets/keymap_drawer_config.yaml
OUT=assets/my_skeletyl_keymap

if [ ! -x .venv/bin/keymap ]; then
  echo "Installing keymap-drawer into .venv ..."
  python3 -m venv .venv
  .venv/bin/pip install -q --disable-pip-version-check keymap-drawer
fi

.venv/bin/python scripts/keymap_c_to_yaml.py "$KEYMAP" "$OUT.yaml"
.venv/bin/keymap -c "$CFG" draw "$OUT.yaml" -o "$OUT.svg" 2> >(grep -v WARNING >&2)

CHROME=""
for c in \
  "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome" \
  "/Applications/Chromium.app/Contents/MacOS/Chromium" \
  google-chrome google-chrome-stable chromium chromium-browser; do
  if [ -x "$c" ] || command -v "$c" >/dev/null 2>&1; then CHROME="$c"; break; fi
done
if [ -z "$CHROME" ]; then
  echo "WARNING: Chrome/Chromium not found; only the SVG was regenerated." >&2
  exit 0
fi

read -r W H < <(python3 -c "import re;s=open('$OUT.svg').read(500);m=re.search(r'width=\"(\d+)\" height=\"(\d+)\"',s);print(m[1],m[2])")
"$CHROME" --headless=new --disable-gpu --hide-scrollbars --force-device-scale-factor=2 \
  --window-size="$W,$H" --screenshot="$KB/$OUT.png" "file://$KB/$OUT.svg" 2>/dev/null

DATE="$(git log -1 --format=%cs -- "$KEYMAP" 2>/dev/null || date +%F)"
SHA="$(git log -1 --format=%h -- "$KEYMAP" 2>/dev/null || echo local)"
PRINT_HTML="$(mktemp -t my_skeletyl_print).html"
cat > "$PRINT_HTML" <<EOF
<!doctype html><html><head><meta charset="utf-8"><style>
  @page { size: A4 portrait; margin: 10mm; }
  html, body { margin: 0; background: #fff; }
  body { font: 9pt -apple-system, Helvetica, Arial, sans-serif; color: #444; }
  img { display: block; width: 100%; max-height: 265mm; object-fit: contain; }
  p { margin: 2mm 0 0; text-align: right; }
</style></head><body>
  <img src="file://$KB/$OUT.svg">
  <p>my_skeletyl (QMK) · keymap $SHA · $DATE</p>
</body></html>
EOF
"$CHROME" --headless=new --disable-gpu --no-pdf-header-footer \
  --print-to-pdf="$KB/$OUT.pdf" "file://$PRINT_HTML" 2>/dev/null
rm -f "$PRINT_HTML"

echo "Rendered: $OUT.{yaml,svg,png,pdf}"
