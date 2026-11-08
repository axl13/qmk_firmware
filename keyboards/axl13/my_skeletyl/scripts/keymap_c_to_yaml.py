#!/usr/bin/env python3
"""Convert keymaps/default/keymap.c into a keymap-drawer YAML file.

Reads LAYOUT_split_3x5_3(...) layer blocks, #define aliases and COMBO()
definitions; writes YAML for `keymap draw`. No QMK CLI needed.

usage: keymap_c_to_yaml.py <keymap.c> <out.yaml>
"""
import re
import sys

import yaml

LAYOUT_MACRO = "LAYOUT_split_3x5_3"
N_KEYS = 36

LABELS = {
    # letters & digits handled generically; punctuation and specials below
    "KC_COMM": ",", "KC_DOT": ".", "KC_SLSH": "/", "KC_SCLN": ";", "KC_QUOT": "'",
    "KC_GRV": "`", "KC_EQL": "=", "KC_MINS": "-", "KC_LBRC": "[", "KC_RBRC": "]",
    "KC_BSLS": "\\", "KC_EXLM": "!", "KC_AT": "@", "KC_HASH": "#", "KC_DLR": "$",
    "KC_PERC": "%", "KC_CIRC": "^", "KC_AMPR": "&", "KC_ASTR": "*", "KC_LPRN": "(",
    "KC_RPRN": ")", "KC_UNDS": "_", "KC_PLUS": "+", "KC_COLN": ":", "KC_DQUO": '"',
    "KC_TILD": "~", "KC_PIPE": "|", "KC_LCBR": "{", "KC_RCBR": "}",
    "KC_LEFT": "←", "KC_RGHT": "→", "KC_UP": "↑", "KC_DOWN": "↓",
    "KC_BSPC": "Bspc", "KC_DEL": "Del", "KC_ENT": "Enter", "KC_SPC": "Space",
    "KC_TAB": "Tab", "KC_ESC": "Esc", "KC_INS": "Ins", "KC_HOME": "Home",
    "KC_END": "End", "KC_PGUP": "PgUp", "KC_PGDN": "PgDn",
    "KC_LGUI": "Cmd", "KC_RGUI": "Cmd", "KC_LCTL": "Ctrl", "KC_RCTL": "Ctrl",
    "KC_LALT": "Alt", "KC_RALT": "Alt", "KC_LSFT": "Shift", "KC_RSFT": "Shift",
    "KC_MUTE": "Mute", "KC_VOLD": "Vol −", "KC_VOLU": "Vol +", "KC_MPLY": "Play",
    "KC_BRID": "Brt −", "KC_BRIU": "Brt +",
    "LALT(LSFT(KC_2))": "€",
    "RESET_STATS": "Reset\nstats",
    "SENTENCE_CASE_TOGGLE": "Sent.\ncase",
    "QK_BOOT": "Boot",
}
MODS = {
    "LSFT_T": "Shift", "RSFT_T": "Shift", "LCTL_T": "Ctrl", "RCTL_T": "Ctrl",
    "LGUI_T": "Cmd", "RGUI_T": "Cmd", "LALT_T": "Alt", "RALT_T": "Alt",
}


def strip_comments(src):
    src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
    return re.sub(r"//[^\n]*", "", src)


def split_args(s):
    """Split a comma list at paren depth 0."""
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def block_after(src, start):
    """Return text inside the parenthesis that opens at/after `start`."""
    i = src.index("(", start)
    depth = 0
    for j in range(i, len(src)):
        if src[j] == "(":
            depth += 1
        elif src[j] == ")":
            depth -= 1
            if depth == 0:
                return src[i + 1 : j]
    raise ValueError("unbalanced parentheses")


def main(path, out):
    src = strip_comments(open(path).read())

    defines = dict(re.findall(r"^\s*#define\s+(\w+)\s+(.+?)\s*$", src, flags=re.M))

    def resolve(tok):
        seen = 0
        while tok in defines and seen < 10:
            tok, seen = defines[tok], seen + 1
        return tok.strip()

    layer_enum = re.search(r"enum\s+layers\s*{([^}]*)}", src).group(1)
    layer_ids = [x.strip().split("=")[0].strip() for x in layer_enum.split(",") if x.strip()]
    pretty = {lid: lid.lstrip("_").title() for lid in layer_ids}

    km_start = src.index("keymaps[][MATRIX_ROWS][MATRIX_COLS]")
    layers = {}
    for m in re.finditer(r"\[(\w+)\]\s*=\s*" + LAYOUT_MACRO, src[km_start:]):
        body = block_after(src, km_start + m.start())
        keys = [resolve(k) for k in split_args(body)]
        assert len(keys) == N_KEYS, f"{m.group(1)}: {len(keys)} keys"
        layers[m.group(1)] = keys
        if len(layers) == len(layer_ids):
            break

    base = layers[layer_ids[0]]

    # positions that activate each layer from the base layer
    activators = {lid: [] for lid in layer_ids}
    for pos, k in enumerate(base):
        m = re.fullmatch(r"LT\((\w+),\s*(\w+)\)", k)
        if m:
            activators[m.group(1)].append(pos)

    def label(k, lid, pos):
        if pos in activators.get(lid, []):
            return {"t": "", "type": "held"}
        if k in ("_______", "KC_TRNS"):
            return {"t": "▽", "type": "trans"}
        if k in ("XXXXXXX", "KC_NO"):
            return ""
        m = re.fullmatch(r"(\w+_T)\((\w+)\)", k)
        if m and m.group(1) in MODS:
            return {"t": label(m.group(2), None, -1), "h": MODS[m.group(1)]}
        m = re.fullmatch(r"LT\((\w+),\s*(\w+)\)", k)
        if m:
            return {"t": label(m.group(2), None, -1), "h": pretty.get(m.group(1), m.group(1))}
        if k in LABELS:
            return LABELS[k]
        m = re.fullmatch(r"KC_([A-Z])", k)
        if m:
            return m.group(1)
        m = re.fullmatch(r"KC_(\d)", k)
        if m:
            return m.group(1)
        m = re.fullmatch(r"KC_(F\d+)", k)
        if m:
            return m.group(1)
        return k.replace("KC_", "")

    yaml_layers = {
        pretty[lid]: [label(k, lid, pos) for pos, k in enumerate(layers[lid])]
        for lid in layer_ids
    }

    # combos: const uint16_t PROGMEM name[] = {A, B, COMBO_END};  COMBO(name, KC)
    arrays = {
        name: [resolve(x) for x in split_args(body) if x.strip() != "COMBO_END"]
        for name, body in re.findall(r"PROGMEM\s+(\w+)\[\]\s*=\s*{([^}]*)}", src)
    }
    combos = []
    for name, kc in re.findall(r"COMBO\(\s*(\w+)\s*,\s*([^)]+?)\s*\)", src):
        if name not in arrays:
            continue
        try:
            positions = [base.index(k) for k in arrays[name]]
        except ValueError:
            continue
        combos.append({"p": positions, "k": label(resolve(kc), None, -1), "l": [pretty[layer_ids[0]]]})

    doc = {
        "layout": {"qmk_keyboard": "bastardkb/skeletyl", "layout_name": "LAYOUT_split_3x5_3"},
        "layers": yaml_layers,
        "combos": combos,
    }
    with open(out, "w") as f:
        yaml.safe_dump(doc, f, allow_unicode=True, sort_keys=False, width=200)


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
