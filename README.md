# Mazda Commander LIN to HID keyboard

## Required hardware

### Components

1. SparkFun Pro Micro 16MHz
2. eletechsup LINTTL3
3. Mini DC-DC Step-Up converter 12V/0.3A - 22 x 11 x 3 mm
4. JRC-B008 USB-C 2.0 socket
5. USB Micro B plug (at least 4 pin)

## Assembly

### Circuit

![Assembly circuit diagram](/assets/images/circuit.png)

### About connections

`PIN 9` is used as a killswitch. Connecting it to `GND` will enable HID functions on Pro Micro, otherwise the device will be running in Debug Mode.

Remember to connect `LIN` and `INH` on the LINTTL3 module! Otherwise the Commander Knob unit won't wake up.

I've skipped the LED connections. If you want the unit to light up check out the pinout below.

### Commander Knob LIN Plug pinout

Looking from the top of the socket!

```
| GND      | LIN bus  | N/C      | +12V     |
|----------|----------|----------|----------|
| LED GND  | N/C      | N/C      | LED +12V |
```

## Code

Does it looks like a vibecoded nonsesne? Yes.

Is it a vibecoded nonsense? Yes...

Does it work? Yes, kinda.

### Default key bindings

![Commander Knob Tybe B](/assets/images/CommanderKnobTypeB.webp)
Image source: https://www.mazda.ca/en/digital-owners-manual/2023/navigation-manuals-6.5g/contents/32020200.html

| Input | Name in code | Output [Keyboard] | Output [Consumer] |
| --- | --- | --- | --- |
| 1 | `music` |  `KEY_N` | - |
| 2 | `home` | `KEY_SPACE` | - |
| 3 | `nav` | `KEY_M` | - |
| 5 | `back` | `KEY_ESC` | - |
| 8 | `star` | `KEY_TAB` | - |

Main knob (inputs 6 & 7)
| Input | Name in code | Output [Keyboard] | Output [Consumer] |
| --- | --- | --- | --- |
| tilt up | `d0_up` |  `KEY_UP_ARROW` | - |
| tilt down | `d0_down` |  `KEY_DOWN_ARROW` | - |
| tilt left | `d0_left` |  `KEY_LEFT_ARROW` | - |
| tilt right | `d0_right` |  `KEY_RIGHT_ARROW` | - |
| rotate left | `d0_r_left` |  `KEY_HOME` | - |
| rotate right | `d0_r_right` |  `KEY_END` | - |
| press | `d0_press` |  `KEY_RETURN` | - |

Volume knob (input 4)
| Input | Name in code | Output [Keyboard] | Output [Consumer] |
| --- | --- | --- | --- |
| rotate left | `d1_r_left` | - | `MEDIA_VOL_DOWN` |
| rotate right | `d1_r_right` | - | `MEDIA_VOL_UP` |
| press | `d1_press` | - | `MEDIA_VOL_MUTE` |

### Changing key bindings

For media controls use `ConsumerKeycode` from [ConsumerAPI.h](https://github.com/NicoHood/HID/blob/master/src/HID-APIs/ConsumerAPI.h#L30C6-L30C22)
For general keys use `KeyboardKeycode` from [ImprovedKeylayouts.h](https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61C6-L61C22)

## 3D printed bottom cover

Grab from Thingiverse: https://www.thingiverse.com/thing:7314745
