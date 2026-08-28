# STM32F446RE Bare-Metal Peripheral Drivers

Register-level peripheral drivers and hardware examples for the STM32F446RE, written in C to explore embedded-system fundamentals without using the STM32 HAL for peripheral configuration.

The project uses CMSIS device definitions while configuring clocks, GPIO modes, alternate functions, interrupts, timers, ADCs, PWM, and USART peripherals directly through registers.

> **Status:** Work in progress. The current implementation assumes the default 16 MHz HSI system clock.

## What This Project Demonstrates

- Register-level STM32 peripheral configuration
- Reading and applying information from datasheets and reference manuals
- GPIO input, output, and alternate-function configuration
- Blocking and interrupt-driven USART communication
- General-purpose timers and SysTick
- External and timer interrupts
- ADC polling and interrupt-driven conversion
- PWM generation and ADC-controlled duty cycles
- Hardware validation using a serial terminal and logic analyzer

## Target Hardware

- **Development board:** NUCLEO-F446RE
- **Microcontroller:** STM32F446RET6
- **Processor:** Arm Cortex-M4 with FPU
- **System clock:** Default 16 MHz HSI
- **Programmer/debugger:** Onboard ST-LINK
- **Serial connection:** ST-LINK Virtual COM Port

See the [NUCLEO-F446RE board notes](boards/nucleo-f446re.md) for pin mappings, clock assumptions, and serial settings.

## Implemented Drivers

| Driver | Current functionality |
|---|---|
| GPIO | GPIOA clock control, pin modes, onboard LED control |
| USART | USART2 TX/RX console, line input, USART1 TX |
| Timers | SysTick delays and general-purpose timer initialization |
| Interrupts | PC13 EXTI, TIM2 update interrupt, USART2 RX interrupt |
| ADC | ADC1 polling and ADC2 interrupt-driven conversions |
| PWM | TIM1 and TIM8 PWM configuration and duty-cycle control |
| Error handling | Driver status codes and readable error names |

## Examples

| Example | Description |
|---|---|
| `01_gpio_blink` | Blink the onboard LED using GPIOA |
| `02_usart_console` | Print messages through USART2 |
| `03_usart_readline` | Receive and echo terminal input |
| `04_usart1_tx_logic_analyzer` | Validate USART1 TX using a logic analyzer |
| `05_timer_2_3_LED` | Exercise TIM2 and TIM3 with the onboard LED |
| `06_button_exti_interrupt` | Toggle the LED using the PC13 user-button interrupt |
| `07_timer_interrupt_blink` | Toggle the LED from a TIM2 interrupt |
| `08_adc_polling_potentiometer` | Read a potentiometer using ADC polling |
| `09_adc_interrupt_potentiometer` | Read a potentiometer through ADC interrupts |
| `10_pwm_led_dimming` | Change LED brightness using PWM |
| `11_adc_pwm_potentiometer_led` | Control PWM duty cycle with a potentiometer |
| `12_timer_adc_sampling` | Sample the ADC at timer-controlled intervals |
| `13_uart_command_led` | Control the LED using serial commands |

## Repository Structure

```text
.
├── boards/              Board-specific documentation
├── drivers/
│   ├── inc/             Public driver headers
│   └── src/             Driver implementations
├── examples/            Standalone peripheral demonstrations
├── tests/               Hardware-validation notes and screenshots
└── README.md
```

## Getting Started

### Requirements

- NUCLEO-F446RE development board
- STM32CubeIDE or an Arm GNU toolchain
- STM32F446 CMSIS core and device headers
- USB cable for the onboard ST-LINK
- Serial terminal configured for 115200 baud, 8-N-1

### Using STM32CubeIDE

1. Create an STM32CubeIDE project for the NUCLEO-F446RE.
2. Keep the system clock at the default 16 MHz HSI configuration.
3. Add `drivers/src` to the project’s source directories.
4. Add `drivers/inc` to the compiler include paths.
5. Ensure the `STM32F446xx` preprocessor definition and STM32F4 CMSIS headers are available.
6. Select one example and use its `main.c` as the application entry point.
7. Build the project and flash it using the onboard ST-LINK.

The repository currently focuses on driver and example source code. CMSIS files, startup code, the linker script, and system initialization are expected to come from the STM32CubeIDE project.

## Hardware Validation

### USART2 Terminal Test

USART2 TX and RX were tested through the ST-LINK Virtual COM Port at 115200 baud.

![USART2 terminal validation](tests/terminal/screenshots/usart2_terminal.jpeg)

Additional details are available in the [USART2 console validation notes](tests/terminal/usart2_console_validation.md).

### USART1 Logic-Analyzer Test

USART1 TX on PA9 was captured and decoded using an asynchronous serial analyzer.

![USART1 logic analyzer validation](tests/logic-analyzer/screenshots/logic_analyzer_usart1tx.png)

See the [USART1 logic-analyzer validation notes](tests/logic-analyzer/usart1_tx_validation.md).

## ADC Safety

The STM32F446 ADC input must remain within the configured analog reference range. Do not connect 5 V directly to an ADC input.

If measuring a signal above `VREF+`, use a correctly designed voltage divider or other signal-conditioning circuit and document the scaling calculation.

## Current Limitations

- Peripheral-clock calculations assume a 16 MHz clock.
- Several USART operations use blocking polling.
- Blocking operations do not currently implement timeouts.
- Interrupt-driven USART reception is still being developed.
- Drivers currently target the NUCLEO-F446RE rather than providing full STM32 portability.
- The repository does not yet provide a standalone Makefile or CMake build.

## Roadmap

- Add a reproducible command-line build
- Add compiler checks through GitHub Actions
- Derive peripheral clocks dynamically
- Add timeout-aware USART and ADC operations
- Improve interrupt-driven USART reception with a ring buffer
- Add unit tests for hardware-independent calculations
- Add SPI and I²C drivers
- Publish a tagged `v0.1.0` release

## References

- [NUCLEO-F446RE product page](https://www.st.com/en/evaluation-tools/nucleo-f446re.html)
- [STM32F446RE datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)
- [STM32F446 reference manual](https://www.st.com/resource/en/reference_manual/DM00135183.pdf)

## License

Choose and add a project license before distributing the code as a reusable library. If you select the MIT License, add a `LICENSE` file and state that the project is available under that license.
