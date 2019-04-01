 # PIC Microcontroller smart wireless High Voltage dimmer


### Table of Contents

1. [ Description. ](#desc)
2. [ Requirements. ](#Requirements)
3. [ Circuit schematic. ](#Circuit)
4. [ Warning. ](#Warnings)
5. [ Manual control. ](#Control)
6. [ API control ](#api)
7. [ Wireless control ](#Wireless)
8. [ Internet ](#Internet)
9. [ PCB schematic. ](#PCB)






<a name="desc"></a>
## 1. Description
Controlling an alternative High Voltage (as an example we will use 220 volts lamp) from a 5 volts microcontroller. The project is compiled using CCS C Compiler and use 16F866 PIC Microcontroller. 
The main microcontroller program is multifunctiondimmer.c

Circuit schematic, Programs, HEX files PCB circuit and proteus files are included in the root folder and the ref directory.

The High Voltage control is done by observing the 60 or 50 MHz 220 volts pulse and detecting the rise and the fall of its curve with the optocoupler 4N35 that cause an interrupt on each half oscillation in the microcontroller where we process according to the selected mode and output a signal accordingly to the optoisolator MOC3020 then to the TRIAC that open up a gate for the High alternative Voltage on each half oscillation (60*2 per second) in order for the light to dime without a resistor.

 Functions:
 * Light dimmer with manual adjustment through a potentiometer
 * Light dimmes according to the sourounded lightning
 * Light dimmes automaticaly from high to low and vise-versa on an infinit loop
 * Lights on if it detected a body movement 
 * Lights on if it detected a body movement and if there is little or no sourounding light
 * Light up any mode by an API request 


<a name="Requirements"></a>
## 2. Requirements: 


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

## 3. Circuit schematic

<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/ref/circuit.JPG" alt="circuit" title="project circuit"></p>

#### 3.2 Power
The device is powered by 220v AC for the lamp circuit and 5 Volts DC for the microcontroller circuit.

Make sure that the is no physical connection between the High volts(220V) circuit and the Low volts circuit(5V), the only links between them should only be the optocouplers(4N35) and the optoisolator(MOC3020).

Make also sure that the control interface is connected through wire connectors and separated from the circuit.

<a name="Warnings"></a>
## 4. Warning
##### USE AT YOUR OWN RISK
##### High Voltage, Electrical Hazards.
    Risk Of Electrical Shock and Danger Of Death!!! 


<a name="Control"></a>
## Controls
### 5. Manual control:       
The manual controller consists of one button and a variable resistor:

```The button is the mode selector and the variable resistor function according to the selected mode. Long button press changes to the next mode.```

##### Modes
- 1 : manual luminosity adjustment by the variable resistor.
- 2 : auto adjusting luminosity to the surrounding light.
- 3 : lights on body movement detection.
- 4 : lights if the surrounding light is not enough and there is a body motion detected.
- 5 : will dime the lamp from the lowest level to the highest level and vise-versa on an infinite loop.
- 6 : Light on and can be controlled by normal switch (our system is hiding).
- 7 : system is on and light is off.





### 6. API control

Uart protocol is the application programming interface of this project its baud rate is 1200, communication can be established through computer usb-to-serial-adapter that is (wired or wirelessly) connected to our circuit through the J4 connectors or the rx port of the microcontroller. 

    Each instruction need to contain two bytes:
 
    - a key byte that needs to be 0xBD 
    (the hexadecimal 0xBD is equal to the ASCII ½)

    - an instruction byte that need to be as follows:
        - from 0x30 to 0x39 will light up the lamp accordingly: 0x33 the lamp is low dimmed, 0x39 the lamp is high dimmed
        - 0x40 will light if the surrounding light is not enough and there is a body motion detected
        - 0x41 light on body movement detection
        - 0x42 adjusting to the surrounding light by the light modeSelector 
        - 0x43 manual adjustment by the variable resistor
        - 0x61 will dime the lamp from the lowest level to the highest level on infinite loop

### 7. Wireless control through the RF 433 MHz
The RF 433Mhz is a cheap module that is used to send wireless signals, and its signals are a bit difficult to deal with. 
It is said that the lunch distance is between 20 and 200 meter.
I didn't test it on a long distance but with 5 meter works fine, the key in succeeding is sending every signal 20 to 100 time through the transmitter in order for one signal to reaching the other end of the receiver.

When dealing with the rf 433MHz its important that the project is successfully working through a wired connection before implementing the rf 433 MHz. (from the tx of the usb-to-serial-converter wired-connected to the rx of our circuit).

The file responsible for checking and sending signals is the serialConnector.py, the file is python program so make sure that python is installed on your computer. 

### 8. The internet
A simple php site hosted on the internet that contains a form with the project parameters, after selecting our desired mode, on submit it will write the changes in a json file on an internet location, the changes then will be detected by our python program that is checking the internet file regularly and will send a signals with the corresponding instruction to the device.


##### Steps:

1. The rf433 Mhz receiver is connected to our device through the J4 connectors. (please check Circuit schematic for the J4 connectors)

2. The rf433 Mhz transmitter is connected to a computer through and usb to serial converter.

3. The computer is connected to the internet

4. A python program running on pc, loads a json file from the internet and load another json file from the local storage and compere the both file against one another.(please check the python program serialConnector.py)

5. if the last entry(value) in the locally stored json file doesn't match the the first entry of the internet loaded json file, then it send a uart signal through the USB port (usb to serial converter) and append the new value from the internet loaded json to a new line in the the locally stored json file, so the instructions will execute only if the internet json file is changed.(serialConnector.py)

6. then repeat on an infinite loop every 3 to 6 seconds.(serialConnector.py)

The php site is internetControle.php, the file is python 

The file responsible for checking and sending signals is the serialConnector.py, the file is python program so make sure that python is installed on your computer. 

##### Modifying internetJson and the changes will reflect on our High Voltage Current output


In order to succeed with he rf433 Mhz make sure that the project is working through a wired connection ( from the tx of the usb to serial converter to the rx of our circuit . please check Circuit schematic).   





<a name="PCB"></a>

## 9. PCB schematic

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
- The High Voltage input is on the far left corner and the output is directly under the input
</p>
<p align="center">
<img src="https://raw.githubusercontent.com/Moh-Snoussi/microcontroller-smart-dimmer/master/ref/3d_card.JPG" alt="circuit" title="components side">
</p>
