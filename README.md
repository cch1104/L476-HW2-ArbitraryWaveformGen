# STM32 DAC Waveform Generator with DMA

## Overview

This project implements an Arbitrary Waveform Generator (AWG) using an STM32 microcontroller.  
The system generates multiple analog waveforms through the built-in DAC and outputs them continuously using DMA + Timer Trigger for efficient hardware-based waveform synthesis.

Waveforms can be switched dynamically using an external push button, while the current waveform type is displayed on an LCD.

---

# Features

- DAC analog waveform output
- DMA-based continuous waveform transfer
- Timer-triggered DAC updates
- Multiple waveform generation
- External interrupt button control
- LCD waveform display
- Lookup Table (LUT) waveform synthesis

---

# Implemented Waveforms

The generator currently supports:

| Waveform | Description |
|---|---|
| Sawtooth | Linear rising ramp |
| Square | 50% duty cycle square wave |
| Sine | Smooth sinusoidal waveform |
| Trapezoidal | Ramp + flat-top waveform |

---

# Hardware Requirements

- STM32 development board
- LCD module (parallel interface)
- Push button (PC13)
- Oscilloscope (recommended)
- Jumper wires

---

# STM32 Peripheral Usage

| Peripheral | Purpose |
|---|---|
| DAC1 | Analog waveform output |
| TIM2 | DAC trigger timing |
| DMA1 | Automatic waveform transfer |
| GPIO EXTI | Push button interrupt |
| GPIO | LCD control |

---

# Project Architecture

## DMA-Based Waveform Generation

The waveform data is stored inside lookup tables:

```c
uint16_t sawTable[TABLE_SIZE];
uint16_t squareTable[TABLE_SIZE];
uint16_t sineTable[TABLE_SIZE];
uint16_t trapezoidTable[TABLE_SIZE];

DMA continuously transfers waveform samples from memory to DAC:

HAL_DAC_Start_DMA(
    &hdac1,
    DAC_CHANNEL_1,
    (uint32_t*)currentTable,
    TABLE_SIZE,
    DAC_ALIGN_12B_R
);

TIM2 generates periodic trigger events:

sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;

Current DMA Method

Now the architecture is:

    TIM2 Trigger → DMA → DAC

Advantages:

Lower CPU load
Higher waveform frequency
More stable waveform output
Better timing precision
Hardware-level signal generation
Scalable to larger waveform tables

Waveform Switching

Waveforms are switched using the external push button on PC13.

The interrupt callback changes the waveform table pointer:

    currentTable = sineTable;

DMA is restarted automatically:

    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);

    HAL_DAC_Start_DMA(
        &hdac1,
        DAC_CHANNEL_1,
        (uint32_t*)currentTable,
        TABLE_SIZE,
        DAC_ALIGN_12B_R
    );

LCD Display

The LCD displays the currently selected waveform:

    Waveform style:
    Sawtooth

Supported display labels:

    Sawtooth
    Squarewave
    Sinewave
    Trapezoidal

Main Program Flow

    Initialize HAL
        ↓
    Configure GPIO / DAC / DMA / TIM2
        ↓
    Generate waveform lookup tables
        ↓
    Start TIM2
        ↓
    Start DAC DMA transfer
        ↓
    Display waveform on LCD
        ↓
    Wait for button interrupt
        ↓
    Switch waveform table
        ↓
    Restart DMA

Example Signal Flow

    Waveform Table
      ↓
     DMA
      ↓
     DAC1
      ↓
    Analog Output

Timer synchronization:

    TIM2 → TRGO → DAC Trigger

## Video Result

https://youtu.be/-jWcd2QrvF0?si=p4T3UnnXI17r-rIE