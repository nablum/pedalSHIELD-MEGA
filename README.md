<p align="center">
    <img src="https://github.com/user-attachments/assets/f7b5cbec-0567-4627-9ddc-53d40b98cde8" 
    alt="logo" 
    width="250"/>
</p>

[![License: CC BY-NC](https://img.shields.io/badge/License-CC%20BY--NC-8e44ad.svg)](https://creativecommons.org/licenses/by-nc/4.0/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform: Arduino](https://img.shields.io/badge/Platform-Arduino-lightblue.svg)](https://www.arduino.cc/)
[![KiCad](https://img.shields.io/badge/PCB-KiCad-blue.svg)](https://kicad.org/)
[![OpenSCAD](https://img.shields.io/badge/CAD-OpenSCAD-orange.svg)](https://www.openscad.org/)

A programmable guitar effects shield for Arduino MEGA, featuring OLED display, true-bypass footswitch, and DSP-ready input/output stages. Based on pedalSHIELD MEGA by [ElectroSmash](https://www.electrosmash.com/pedalshield-mega)  

<p align="center">
    <img src="https://github.com/user-attachments/assets/0dfe1d75-aef2-4f0b-8fa8-783ec8c1eae4" 
    alt="device" 
    width="500"/>
</p>

## 📦 What’s Included

- ✅ **KiCad project**: schematic & PCB layout files for the hardware shield
- ✅ **Arduino examples**: boost, fuzz, delay, and other DSP sketches
- ✅ **Bill of Materials**: CSV file with component list and Mouser links
- ✅ **Documentation**: assembly guide, schematic diagrams, signal flow

## 🛠️ Assembly & Programming

1. **Clone the repository**:

    ```bash
    git clone https://github.com/nablum/pedalSHIELD-MEGA.git
    cd pedalSHIELD-MEGA
    ```

2. **Build the PCB** with KiCad or order the board from a fab using the provided Gerbers.

3. **Solder the components** according to the [Assembly Guide](https://www.electrosmash.com/media/kunena/attachments/631/How-to-Build-pedalSHIELD-MEGA-V1.1.pdf).

4. **Upload an effect sketch** to your Arduino MEGA using Arduino IDE.
5. **Plug and play**:
    - Guitar IN → Input Jack
    - Output → Amp or Audio Interface
    - Power → USB or DC JACK.

## 🎛️ Example Effects

Each audio effect is implemented in a separate folder under [`firmware/effects/`](./firmware/effects/):

| Effect Name       | Description                                               |
|-------------------|-----------------------------------------------------------|
| BitCrusher        | Reduces resolution and sample rate for lo-fi textures     |
| Chorus            | Modulates pitch with delay to create a thickening effect  |
| ChorusOrVibrato   | Blendable chorus and vibrato with adjustable depth        |
| Clean             | Transparent clean boost (reference circuit)               |
| Distortion        | Hard-clipping distortion effect                           |
| Echo              | Simple digital delay with feedback loop                   |
| Fuzz              | Saturated high-gain fuzz effect                           |
| MultiEffects      | Combination of multiple effects in a single patch         |
| OctaveCrusher     | Bit reduction with added octave-up/down harmonics         |
| Octaver           | Mixes original with octave-down processed signal          |
| Reverb            | Simulates ambient reverb using delay-based algorithm      |
| Tremolo           | Volume modulation using a low-frequency oscillator        |
| Vibrato           | Pitch modulation effect for expressive shimmer            |

## ⚙️ Hardware Specs

- Compatible with **Arduino MEGA 2560**
- 128×64 I²C OLED display (SSD1306)
- True-bypass footswitch with relay
- Stereo audio jacks with analog preamp/output
- Powered via **12 V DC (center-negative)**

## ❓ Troubleshooting

- **OLED not lighting up** → Double-check I²C wiring and 4.7k pull-up resistors.
- **No audio output** → Ensure signal chain is complete and shield is powered.
- **Arduino upload fails** → Check USB port, board selection, and serial permissions.

## 📚 Resources

- 🔬 [pedalSHIELD-MEGA Documentation – ElectroSmash](https://www.electrosmash.com/pedalshield-mega)  
  Official guide, schematics, theory of operation, and signal path details.

- 🧰 [Assembly Guide – PDF](https://www.electrosmash.com/media/kunena/attachments/631/How-to-Build-pedalSHIELD-MEGA-V1.1.pdf)  
  Step-by-step instructions with images for building the pedalSHIELD‑MEGA hardware.

- 📦 [u8glib – OLED Display Library](https://github.com/olikraus/u8glib)  
  Arduino graphics library used to drive the OLED screen (SSD1306, I²C).

- 💻 [Arduino IDE Downloads](https://www.arduino.cc/en/software/)  
  Install the Arduino IDE to compile and upload code to the MEGA 2560.

## 🤝 Contributing

Contributions are welcome! You can:

- Submit new audio effects or improvements to existing sketches.
- Suggest hardware upgrades (e.g. better op-amps, analog filtering).
- Improve documentation or add new troubleshooting tips.

Please open a pull request against the `main` branch and follow the coding style of existing files.

## 📄 License

- **Hardware (PCB files, schematics, BOM)**:  
  Licensed under [Creative Commons Attribution-NonCommercial 3.0 (CC BY-NC 3.0)](https://creativecommons.org/licenses/by-nc/3.0/)

- **Software (Arduino code, examples)**:  
  Licensed under the [MIT License](LICENSE)

## 📬 Contact

Created by [@nablum](https://github.com/nablum).  
For questions or collaboration ideas, feel free to open an issue or reach out directly.
