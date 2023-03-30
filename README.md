# G431B_ESC_V1

Yet another port of the Mini Cheetah motor controller firmware; this time to ST's G431B-ESC dev board. https://www.st.com/en/evaluation-tools/b-g431b-esc1.html

Firmware architecture based on: https://github.com/bgkatz/motorcontrol

Some hardware calculations and hardware specific implementation details from: https://github.com/pat92fr/BrushlessServoController and https://github.com/simplefoc/Arduino-FOC 

____________________________________________________________________________________________________________________________________________________________________________________
Main branch uses an AS50XX (in my case, an AS5048A) external encoder on a custom breakout (gerbers, stp, and Eagle files in 'Hardware' folder). 

Communicates over 3 wire SPI (firmware bitbangs due to lack of broken out SPI peripheral pins on dev board). 

To use the breakout, do not populate the 5V->3V3 shorting jumper. Populate the MOSI pullup jumper (MOSI needs to be pulled high when not used).

To use SPI, the 3 resistors boxed in the wiring image need to be replaced with 0ohm 0402 resistors (or bridged with solder). 

____________________________________________________________________________________________________________________________________________________________________________________
Various branches have varying support of I2C and PWM encoder implementations. Use of I2C and PWM encoders do not require modification of the dev board. 

 ____________________________________________________________________________________________________________________________________________________________________________________

**Done:**

Serial

Flash

Encoder (3 Wire SPI, I2C, PWM)

PWM

Current Sense

State Machine

FOC (10khz)

CAN

Mosfet temperature sense


**Next Steps**

Increase loop frequency


