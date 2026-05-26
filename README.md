# L476-HW2-ArbitraryWaveformGen
# STM32 DAC Waveform Generator

## Overview
This project implements a simple Arbitrary Waveform Generator (AWG) using an STM32 microcontroller, DAC output, timer interrupts, and an LCD display.

The system can generate four different waveforms:

- Sawtooth Wave
- Square Wave
- Sine Wave
- Trapezoidal Wave

A push button connected to `PC13` switches between waveform types in real time.

The generated waveform is output through:

- DAC Channel 1
- Timer interrupt driven sampling
- Lookup table waveform synthesis

---

# Features

- Multiple waveform generation
- Real-time waveform switching using external interrupt
- DAC analog output
- LCD waveform status display
- Timer interrupt driven sampling
- Lookup-table based waveform generation

---

# Hardware Requirements

## Microcontroller
STM32 board with:
- DAC peripheral
- TIM2
- GPIO interrupt support

Example:
- STM32L4 series
- STM32F3 series
- STM32F4 series

---

# Peripheral Usage

| Peripheral | Purpose |
|---|---|
| DAC1 CH1 | Analog waveform output |
| TIM2 | Sampling interrupt trigger |
| GPIO PC13 | Waveform switch button |
| GPIOA | LCD interface |
| LCD | Waveform name display |

---

# Waveform Types

## 1. Sawtooth Wave
Generated using a linear increment lookup table.

---

## 2. Square Wave
First half of table:
- DAC = 4095

Second half:
- DAC = 0

---

## 3. Sine Wave

Formula:

```math
y = (\sin(\frac{2\pi i}{N}) + 1) \times 2027

Where:

N = TABLE_SIZE

## 4. Trapezoidal Wave


Generated using:

Rising edge
High plateau
Falling edge
Low plateau

Software Architecture
Lookup Tables
uint16_t sawTable[TABLE_SIZE];
uint16_t squareTable[TABLE_SIZE];
uint16_t sineTable[TABLE_SIZE];
uint16_t trapezoidTable[TABLE_SIZE];

Each waveform is precomputed and stored in memory.

Current Waveform Pointer
uint16_t *currentTable = sawTable;

The pointer dynamically selects the active waveform table.

Waveform Enumeration
typedef enum {
    Sawtooth,
    Square,
    Sine,
    Trapezoidal
} Waveform_t;

Used for waveform state management.

Timer Interrupt Operation

Waveform samples are updated inside:

HAL_TIM_PeriodElapsedCallback()

Core operation:

HAL_DAC_SetValue(
    &hdac1,
    DAC_CHANNEL_1,
    DAC_ALIGN_12B_R,
    currentTable[indexWave]
);

The timer determines waveform frequency.

Waveform Switching

The push button interrupt is handled by:

HAL_GPIO_EXTI_Callback()

Button press sequence:

Sawtooth
   ↓
Square
   ↓
Sine
   ↓
Trapezoidal
   ↓
Repeat
LCD Display

LCD continuously displays the current waveform name.

Example:

Waveform style:
Sinewave
Timer Configuration
TIM2 Settings
Prescaler = 3999
Period     = 19

Interrupt frequency:

Timer Frequency =
SystemClock / ((Prescaler + 1) × (Period + 1))




	​






;




