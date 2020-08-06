# NucleoF303K8_HighAccuracyPWMreader
2 channels high accuracy PWM acquisition using NUCLEO-F303K8 development board

![](/Images/Nucleo.PNG) 

# Description 
This module is capable of reading up to 2 high-frequency PWM signals and to provide the user with accurate values via the serial communication. 

The SW module runs on a NUCLEO-F303K8 board and makes use of general-purpose timers TIM3 and TIM15, which consist in 16-bit auto-reload counters and programmable prescalers. 
When configured in "Input capture" mode, these timers can be used to measure the pulse length of input signals. 

The module has been tested with an automotive position hall-sensor from TE connectivity delivering 1KHz PWM signal

The picture below shows the system architecture together with the general functioning: 

![](/Images/Diagram.PNG) 

Concerning the HW, the Serial reader provides power to the Nucleo development board which in turn provides power to the sensors. All share the same GND.  The sensors outputs are connected to the Nucleo development board pins pA3 and pA6. The board can be connected to the Serial reader through USB cable or by mean of the single pins (5V, GND, Tx, Rx)

The sensors output a 1KHw PWM which is read by the Nucleo board. The SW loop allows the module to read the digital signal from GPIO ports. The rising and falling edge of the PWM signals are captured with general purpose timers via Interrupt service routines. During each cycle, the PWM period is computed (in order to cover for sensors with small variations on the output frequecy) and then the duty ratio in terms of percentage is evaluated with high accuracy (2 decimal digits). Data are then sent to the Serial port (printf() or pc.putc() )

# Usage instruction
In order to program NUCLEO-F303K8 development board I've chosen to make use of Mbed online compiler (https://ide.mbed.com/)

Please refer to the following procedure: 
* Create an Mbed profile
* Create new program within the IDE program workspace
  * Choose the  NUCLEO-F303K8 development board
* Substitute the main.cpp script
* Continue with standard procedure to flash the SW

Once the SW module has been flashed correctly to the development board, one can decide to read the values through the serRead.py script by following these steps:
* Connect the output of the PWM provider/s (sensor/s) with NUCLEO-F303K8 development board through pA3 and pA6 pins
* Connect NUCLEO-F303K8 development board to your pc via serial port
* Change the COM Port in line #4 of serRead.py as per needed
* Run the script: the stream of values will appear on the console
