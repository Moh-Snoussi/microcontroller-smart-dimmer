# PIC Microcontroller smart wireless High Voltage dimmer
### Description
Controlling an alternative 220/110 volts from a 5 volts microcontroller.

#### Control interface
##### API
    uart protocol with a baud rate of 1200 that can be connected and communication can be established through computer usb to serial adapter directly or trough a radio frequencies module with an rf module 433mhz transmiter where the receiver is connected to the uart microcontroller that fires an interupt if it recieve a signal. 
 
      - The instraction need to contain two bytes one byte key and one byte instraction:
        - the first byte is a key and it need to be 0xBD
        - the second byte is the instraction and it need to be as follows:
          - from 0x33 to 0x39 will light up the lamp accordingly: 0x33 the lamp is low dimmed, 0x39 the lamp is high dimmed
          - 0x40 will light if the surrounding light is not enough and there is a body motion detected
          - 0x41 light on body movement detection
          - 0x42 adjusting to the surrounding light by the light modeSelector 
          - 0x43 manual adjustment by the variable resistor
          - 0x61 will dime the lamp from the lowest level to the highest level on infinit loop
##### Manual control:       
    The manual controller consists of one button and variable resistor: 
         - The button is the mode selector and the variable resistor function according to the selected mode
           Long buttonPresschange the mode (a two color led diode indicate the selected mode  ) :
  
           - 1 : manual luminosity adjustment by the variable resistor
           - 2 : auto adjusting luminosity to the surrounding light
           - 3 : lights on body movement detection
           - 4 : lights if the surrounding light is not enough and there is a body motion detected
           - 5 : will dime the lamp from the lowest level to the highest level and vise-versa on an infinit loop
           - 6 : Light on and can be controlled by normal switch (our system is hiding) 
           - 7 : system is on and light is off
  

### DANGER
High Voltage, Electrical Hazards.

    Risk Of Electrical Shock and Danger Of Death!!! 
##### USE IS AT YOUR OWN RISK

### Compiler
The project compiled with CCS C Compiler and use 16F866 PIC Microcontroller.
### Requirements: 
    - 1 PIC microcontroller 16f866
    - 4 resistor 2.2k 1/4w
    - 3 resistors 100K 1/4w
    - 1 resistor 51k OHM 1/4w
    - 2 resistors 680 OHM 1/2wat
    - 1 risitor 10 k 1/4w
    - 2 resitor 220 OHM 1/4w
    - 2 30pf ceramic resistors
    - 1 35v 10uf electrolic capasitor
    - 1 bridge rectifier 3A 700V
    - 1 optocoupleur phototransistor 4N35
    - 1 optoisolator transistor MOC3020
    - 1 20MHz Crystal Oscillator
    - 1 relay 10A 5/25 DC - 110/220AC
    - 3 NPN transistors C33740
    - 1 Triac sensitive gate BT136X

    - 2 Button
    - 1 100K OHM Potentiometer
    - 1 Photoresistor LDR (light sensor) (optional)
    - 1 Infrared PIR Motion Sensor (optional)
    - 1 connectors 9 pins (optional)
    - 1 connectors 4 pins (optional)
    - USB to serial converter (optional)
    - rf module 433mhz receiver (optional)
    - rf module 433mhz transmitter (optional)

### Circuit

<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/circuit.JPG" alt="circuit" title="project circuit"></p>