# Rover Magnetic Field Detection System

A simple magnetic field detection system for rover applications using Allegro A1324 Hall sensor and Arduino.

---

## What's in this repository

- `magnet_detect_final_edition.ino` - Arduino firmware for reading the sensor
- `A1324-5-6-Datasheet.pdf` - Hall sensor datasheet
- Circuit schematic (see image: hall_sensor_opamp_amplify.png)

---

## Components Used

| Component | Model |
|-----------|-------|
| Hall Sensor | Allegro A1324 (sensitivity: 5.0 mV/G) |
| Op-Amp | MCP6022 (LT1366 in schematic, MCP6022 used in practice) |
| MCU | Arduino (Uno/Nano/Mega) |

---

## Circuit Overview

The Hall sensor outputs an analog voltage proportional to the magnetic field strength. The signal is amplified by an op-amp before being read by the Arduino's ADC.

### A1324 Pinout

| Pin | Function |
|-----|----------|
| Left (VCC) | 3.3V power |
| Middle (GND) | Ground |
| Right (VOUT) | Analog output |

### Key Parameters (from datasheet)

- Quiescent output: 50% of VCC (≈1.65V at 3.3V)
- Sensitivity: 5.0 mV/G
- Operating voltage: 4.5V – 5.5V (also works at 3.3V)

---

## How the Code Works

1. **Calibration**: On startup, the Arduino takes 100 readings to find the center (no-field) value.
2. **Detection**: The code calculates `strength = abs(hallValue - centerValue)`.
3. **Stability check**: Waits for stable readings before reporting a detection.
4. **Output**: Prints the result via serial monitor.

### Key Thresholds (adjustable)

| Parameter | Default | Description |
|-----------|---------|-------------|
| `detectThreshold` | 12 | Minimum strength to trigger detection |
| `releaseThreshold` | 12 | Reset detection when strength drops below this |
| `stableDelta` | 5 | Max variation to consider the reading "stable" |
| `stableRequired` | 4 | Number of consecutive stable readings needed |

---

## Serial Output

Open Serial Monitor at **115200 baud**.

During normal operation (debug mode ON):

```
Calibrating...
Center = 512
System Ready
532
528
530
545
560
===== RESULT =====
DIRECTION: UP
==================
512
510
508
505
===== RESULT =====
NO STONE
==================
498
492
480
===== RESULT =====
DIRECTION: DOWN
==================
```

Output Explanation:

| Output | Meaning |
|--------|---------|
| `Calibrating...` | System is taking 100 readings to find center value |
| `Center = 512` | No-field baseline value (varies with your sensor) |
| `System Ready` | Calibration complete, ready to detect |
| `[number]` | Raw ADC reading from hallPin (debug output) |
| `===== RESULT =====` | A stable magnetic field has been detected |
| `DIRECTION: UP` | hallValue > centerValue (south pole / increasing field) |
| `DIRECTION: DOWN` | hallValue < centerValue (north pole / decreasing field) |
| `NO STONE` | Field strength below detection threshold |
| `==================` | End of result block |

**Note:** 
- `debugMode = true` (default): Raw ADC values are printed every loop.
- `debugMode = false`: Only detection results are printed.

---

## Threshold Tuning

These values in the code control detection sensitivity:

```cpp
const int detectThreshold = 12;    // Minimum strength to trigger detection
const int releaseThreshold = 12;   // Reset detection when strength drops below this
const int stableDelta = 5;         // Max variation to consider the reading "stable"
const int stableRequired = 4;      // Number of consecutive stable readings required
```

**Adjust for your environment:**
- Higher `detectThreshold` → less sensitive (ignore small magnetic fields)
- Lower `detectThreshold` → more sensitive (detect weaker fields)
- Increase `stableRequired` → more stable but slower response

---

## Quick Setup

1. Connect the circuit as shown in the schematic.
2. Upload the `.ino` file to your Arduino.
3. Open Serial Monitor (115200 baud).
4. Move a magnet near the sensor and observe the output.

---

## Notes for Rover Integration

- Mount the sensor away from motors to avoid interference.
- Add a 0.1µF capacitor near the sensor VCC pin for noise filtering.
- The op-amp circuit (MCP6022) provides additional gain for better sensitivity.

---

## References

- [A1324 Datasheet](./A1324-5-6-Datasheet.pdf)
- Allegro MicroSystems - www.allegromicro.com
