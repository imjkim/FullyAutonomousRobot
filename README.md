# Mechatronics
Group project at ucsc capstone
https://imjkim.com/projects/mechatronics

This is the embedded firmware for a fully autonomous robot. It finds an IR beacon, drives to the target tower, lines up on it using track-wire and bumper sensors, and fires its payload with a motor-driven puncher. All of it runs with no human input.

## Tech stack
| Layer | Details |

| **Microcontroller** | Digilent Uno32 (Microchip PIC32MX320) |

| **Language / toolchain** | C · Microchip XC32 compiler · MPLAB X IDE |

| **Software architecture** | Event-driven **Events & Services framework**: a **hierarchical state machine (HSM)** with nested sub-FSMs, event checkers, and timer-driven services |

| **Actuators** | 4 DC motors (left/right drive, a mid motor, a puncher/launcher), driven by PWM through H-bridges with direction control |

| **Sensors** | IR beacon detector (analog) · 2 inductive **track-wire** sensors (analog) · 8 IR tape sensors · 6 bumper switches |

| **Peripherals used** | PWM, ADC, GPIO, hardware timers, UART debug output |

## Software architecture
