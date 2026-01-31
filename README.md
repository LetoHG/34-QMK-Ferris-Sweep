# 34-QMK-Ferris-Sweep

My personal Ferris Sweep layout focused on flexible thumb clusters and multiple alpha layers.

> [!NOTE]
> The basic design philosophy of this keymap is influenced by the work and ideas of [**Ben Vallack**](https://github.com/benvallack).

> [!INFO]
> This keymap is developed and tested with **QMK Firmware 0.31.3**.
> Other versions may work but are not guaranteed.

---

## Features

- **Four alpha layers**, switchable at runtime:
  - Graphite (Default)
  - ISRT
  - Colemak-DH
  - QWERTY

- **Flexible Thumb Cluster Swapping**  
  Swap thumb key behavior on one side or both halves simultaneously.

- **Cross-Side Thumb Cluster Swap**  
  Mirror entire thumb cluster layouts between halves.

- **Thumb Combo as Shift**  
  Press both inner thumb keys to trigger Shift — no more thumb pivoting.

## Usage

```
cd <path-to-qmk-firmware>
git clone https://github.com/LetoHG/34-QMK-Ferris-Sweep ./keyboards/ferris/keymaps/34-QMK-Ferris-Sweep
```

## Building

### Elite-C
```
make ferris/sweep:34-QMK-Ferris-Sweep
```


### Sea-Picro

For builds using a [Sea-Picro](https://shop.beekeeb.com/products/sea-picro) compile using:
```
qmk compile -c -kb ferris/sweep -km 34-QMK-Ferris-Sweep -e CONVERT_TO=rp2040_ce
```


After a successful build, the generated `.hex` or `.uf2` file in the `.build` folder can be flashed to the board.
See the official [QMK flashing guide](https://docs.qmk.fm/newbs_flashing) for details.

## Keymap

![](keymap.ortho.svg)
