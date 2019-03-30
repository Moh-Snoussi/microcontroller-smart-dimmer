# PIC Microcontroller smart wireless High Voltage dimmer


### Table of Contents

1. [ Description. ](#desc)
2. [ Warning. ](#Warnings)
3. [ Control interface. ](#Control)
4. [ Requirements. ](#Requirements)
5. [ Circuit schematic. ](#Circuit)
6. [ PCB schematic. ](#PCB)






<a name="desc"></a>
## 2. Description
Controlling an alternative 220/110 volts from a 5 volts microcontroller. The project compiled with CCS C Compiler and use 16F866 PIC Microcontroller. circuit schematic, PCB circuit and proteus files are included.

The lamp control is done by observing the 60 or 50 MHz High alternative current pulse and detecting the rise and the fall of its curve with the 4n35 that cause an interrupt on each oscillation in the microcontroller where we process according to the selected mode and output a signal accordingly to the moc3020 then to the triac that open up a gate for the High alternative Voltage on each half oscillation in order for the light to dime without resistor.

 Functions:
 * Light dimmer with manual adjustment
 * Light dimmes according to the sourounded lightning
 * Light dimmes automaticaly from hight to low on an infinit loop
 * Lights on if detected a body movement 
 * Lights on if detected a body movement only if there is little to no sourounding light
 * Light up any mode by an API request 

#### 2.1 Power
The device is powered by 220v AC for the lamp circuit and 5 Volts DC for the microcontroller circuit.

Make sure that the is no physical connection between the High volts(220V) circuit and the Low volts circuit(5V), the only links between them should only be the optocouplers(4N35) and the optoisolator(MOC3020).

Make also sure that the control interface is connected through wire connectors and separated from the circuit.

<a name="Warnings"></a>
## 2. Warning
##### USE AT YOUR OWN RISK
##### High Voltage, Electrical Hazards.
    Risk Of Electrical Shock and Danger Of Death!!! 


<a name="Control"></a>
## 3. Control interface
##### API
Uart protocol is the application programming interface of this project its baud rate is 1200, communication can be established through computer usb to serial adapter directly or trough a radio frequencies rf 433mhz module transmitter where the receiver is connected to the uart microcontroller that fires an interrupt if it receives a signal. 

    The signal need to contain two bytes:
 
    - a key byte that needs to be 0xBD
    - an instruction that need to be as follows:
        - from 0x33 to 0x39 will light up the lamp accordingly: 0x33 the lamp is low dimmed, 0x39 the lamp is high dimmed
        - 0x40 will light if the surrounding light is not enough and there is a body motion detected
        - 0x41 light on body movement detection
        - 0x42 adjusting to the surrounding light by the light modeSelector 
        - 0x43 manual adjustment by the variable resistor
        - 0x61 will dime the lamp from the lowest level to the highest level on infinite loop 

##### Manual control:       
```The manual controller consists of one button and a variable resistor:``` 

    The button is the mode selector and the variable resistor function according to
    the selected mode. Long button press changes the mode.
           
        - 1 : manual luminosity adjustment by the variable resistor
        - 2 : auto adjusting luminosity to the surrounding light
        - 3 : lights on body movement detection
        - 4 : lights if the surrounding light is not enough and there is a body motion detected
        - 5 : will dime the lamp from the lowest level to the highest level and vise-versa on an infinite loop
        - 6 : Light on and can be controlled by normal switch (our system is hiding)
        - 7 : system is on and light is off
  
<a name="Requirements"></a>
## 4. Requirements: 

##### Compiler
In order to modify the program you need to have CCS C Compiler installed and a Microcontroller programer or a Boot-loader.

    - 1 PIC microcontroller 16f866
    - 4 resistor 2.2k 1/4w
    - 3 resistors 100K 1/4w
    - 1 resistor 51k OHM 1/4w
    - 2 resistors 680 OHM 1/2wat
    - 1 resistor 10 k 1/4w
    - 2 resistor 220 OHM 1/4w
    - 2 30pf ceramic resistors
    - 1 35v 10uf electronic capacitor
    - 1 bridge rectifier 3A 700V
    - 1 optocouplers phototransistor 4N35
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


<a name="Circuit"></a>

## 5. Circuit schematic

<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/ref/circuit.JPG" alt="circuit" title="project circuit"></p>

<a name="PCD"></a>

## 5. PCB schematic

Using double-faced Photoresist Board and taking advantage by connecting the ground on the components side.

<p align="center">
 Bottom copper
</p>
<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/ref/circuit_front_git.JPG" alt="circuit" title="back side"></p>
<p align="center">------------------------------------------------------------------</p>
<p align="center">
Taking advantage of the double-faced Photoresist Board by connecting the ground on the components side.</p>
<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/ref/circuit_git.JPG" alt="circuit" title="components side">
</p>
<p align="center">------------------------------------------------------------------</p>
<p align="center">
- The High Voltage input is on the far left corner the output is directly under the input
</p>
<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/ref/3d_card.JPG" alt="circuit" title="components side">
</p>