/**
 * @Author: Mohamed Snoussi
 * @email: sooniic@live.com
 * @Compiler: CCS C Compiler
 * 
 * @ALERT: USE IS AT YOUR OWN RISK
 * 
 * @DANGER: Risk Of Electrical Shock and Danger Of Death!!! High Voltage, electrical hazards.
 * This progect is dangerous and can threatten your life, damage or fire
 * 
 * @project: controlling an alternative 220/110 volts from a 5 volts microcontroller.
 * The project compiled with CCS C Compiler and use 16F866 PIC Microcontroller
 * 
 * control interface :
 * 
 * 1 : API: uart protocol with a baud rate of 1200 communication can be established through 
 *          computer usb to serial adaptaeur directly or trough a radio frequencies rf 433mhz 
 *          module transmiter where the receiver is connected to the uart microcontroller that 
 *          fires an interupt if it recieve a signal. 
 *
 *     - The instraction need to contain two bytes:
 *       - the first byte is a key and it need to be 0xBD
 *       - the second byte is the instraction and it need to be as follows:
 *         - from 0x33 to 0x39 will light up the lamp accordingly: 0x33 the lamp is low dimmed, 0x39 the lamp is high dimmed
 *         - 0x40 will light if the surrounding light is not enough and there is a body motion detected
 *         - 0x41 light on body movement detection
 *         - 0x42 adjusting to the surrounding light by the light modeSelector 
 *         - 0x43 manual adjustment by the variable resistor
 *         - 0x61 will dime the lamp from the lowest level to the highest level on infinit loop
 *         
 * 2 : Manual control
 *     The manual controller consists of one button and variable resistor: 
 *        - The button is the mode selector and the variable resistor function according to the selected mode
 *          Long buttonPresschange the mode (a two color led diode indicate the selected mode  ) :
 * 
 *          - 1 : manual luminosity adjustment by the variable resistor
 *          - 2 : auto adjusting luminosity to the surrounding light
 *          - 3 : lights on body movement detection
 *          - 4 : lights if the surrounding light is not enough and there is a body motion detected
 *          - 5 : will dime the lamp from the lowest level to the highest level and vise-versa on an infinit loop
 *          - 6 : Light on and can be controlled by normal switch (our system is hiding) 
 *          - 7 : system is on and light is off
 * 
 * 
 * 
 * @WARNING make sure that the is no physical connection between the 220/110 volts circuit and 5 volts circuit
 * the only links bettween the 220/110 volts circuit and the 5 volts circuit is the optocouplers(4N35) and the optoisolator(MOC3020).
 * Make also sure that the conrole interface is connected through connectors and seperated from the circuit 
 * 
 * 
 * the lamp control is done by observing the alternative current pulse and detecting the rise of its curve with 4n35 
 * that cause an interupt on each oscillation in the microcontroller where we proccess according to the selected mode 
 * and output a signal accordingly to the moc3020 then to the triac that open up a gate for the High alternative Voltage on each half oscillation in order for the 
 * light to dime without resistor.
 * 
 * The solution functions:
 * Light dimmer with manual adjustment
 * Light dimmes according to the sourounded lightning
 * Light dimmes automaticaly from hight to low on an infinit loop
 * Lights on if detected a body movement 
 * Lights on if detected a body movement only if there is little to no sourounding light
 * Light up any mode by an API request 
 * 
 * 
 * @Requirements: 
 *       - 1 PIC microcontroller 16f866
 *       - 4 resistor 2.2k 1/4w
 *       - 3 resistors 100K 1/4w
 *       - 1 resistor 51k OHM 1/4w
 *       - 2 resistors 680 OHM 1/2wat
 *       - 1 resistor 10 k 1/4w
 *       - 2 resistor 220 OHM 1/4w

 *       - 2 30pf ceramic resistors
 *       - 1 35v 10uf electronic capacitor
 *       - 1 bridge rectifier 3A 700V
 *       - 1 optocouplers phototransistor 4N35
 *       - 1 optoisolator transistor MOC3020
 *       - 1 20MHz Crystal Oscillator
 *       - 1 relay 10A 5/25 DC - 110/220AC
 *       - 3 NPN transistors C33740
 *       - 1 Triac sensitive gate BT136X

 *       - 2 Button
 *       - 1 100K OHM Potentiometer
 *       - 1 Photoresistor LDR (light sensor) (optional)
 *       - 1 Infrared PIR Motion Sensor (optional)
 *       - 1 connectors 9 pins (optional)
 *       - 1 connectors 4 pins (optional)
 *       - USB to serial converter (optional)
 *       - rf module 433mhz receiver (optional)
 *       - rf module 433mhz transmitter (optional)
 * 
 */

#include <mainlast final.h>
#include <stdlibm.h>
#include "math.h"

int lightLevel, oldInstruction;
int1 loopBreak, StateOnStart, buttonPress, check, indicatorFlag, motionFlag;
int32 clickTiming, to, manualControl;
int modeSelector, out;

/**
 * @brief API interface
 * UART communication baud rate 1200
 * This function fires only if an uart signal have 0xBD as first byte the check is done by the interrupt function 
 * that is next to this function.
 * Second uart byte is the instruction and the mode selector 
 * this function should have an instruction byte as parametter
 * 
 * @name apiState
 * @param instruction 
 */
void apiState(int instruction)
{

    if ((instruction != oldInstruction) && (instruction == 0x33 || instruction == 0x32 || instruction == 0x31 || instruction == 0x30 || instruction == 0x34 || instruction == 0x35 || instruction == 0x36 || instruction == 0x37 || instruction == 0x38 || instruction == 0x39 || instruction == 0x61 || instruction == 0x62 || instruction == 0x73))
    {
        oldInstruction = instruction;
        switch (instruction)
        {

        case 0x30: // no light output, no loop dimming lightLevel => 128 no light --- 0 full light

            lightLevel = 128;    //  no light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x31: // low light output, no loop dimming lightLevel => 128 no light --- 0 full light

            lightLevel = 115;    //  low light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x32: // mid low light output, no loop dimming lightLevel => 128 no light --- 0 full light

            lightLevel = 100;    //  mid low light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x33:
            lightLevel = 88;     //  mid low level 2 light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x34:               // mid light output, no loop dimming lightLevel => 128 no light --- 0 full light
            lightLevel = 75;     // mid light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x35:               // mid level 2 light output, no loop dimming lightLevel => 128 no light --- 0 full light
            lightLevel = 60;     // mid level 2 light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x36:               // mid hight light output, no loop dimming lightLevel => 128 no light --- 0 full light
            lightLevel = 48;     // mid hight light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x37:               // high low 2 light output, no loop dimming lightLevel => 128 no light --- 0 full light
            lightLevel = 35;     // high low 2 light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x38:               // high mid light output, no loop dimming lightLevel => 128 no light --- 0 full light
            lightLevel = 20;     // high mid light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;

        case 0x39:               // full light output, no loop dimming lightLevel => 128 no light --- 0 full light
            lightLevel = 10;     // full light output
            loopBreak = 0;       // no loop dimming
            manualControl = 0;   // no manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x40:
            loopBreak = 0;       // no loop dimming
            modeSelector = 4;    // Mode 4 is selected, light up only if the surrounding light is not enough and there is a body motion detected
            manualControl = 1;   // allow manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x41:
            loopBreak = 0;       // no loop dimming
            modeSelector = 3;    // Mode 4 is selected,light on body movement detection
            manualControl = 1;   // allow manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x42:
            loopBreak = 0; // no loop dimming
            modeSelector = 2;// adjusting to the surrounding light by the light modeSelector
            // the lamp is lightning according to outside light that is observed from the light sensor:
            // light sensor to observed the out side light
            // variable resistor:- if is greater then first half then the more light outside the more light our lamp lights
            //                  :- if is less then first half then the more light outside the less light
            manualControl = 1;
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x43:
            loopBreak = 0; // no loop dimming
            modeSelector = 1; // manual adjustment by the variable resistor
            manualControl = 1;// allow manual control
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        case 0x61:
            loopBreak = 1;       // auto dimming loop active
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current
            break;

        default:
            lightLevel = 0;
            manualControl = 0;
            loopBreak = 0;       // no loop dimming
            OUTPUT_HIGH(PIN_B7); // output 5 volts from pin B7 to allow the relay flow of current

            break;
        }
    }
}

/**
 * @brief API interface
 * UART communication baud rate 1200
 * This function is uart interrupt it fires only if an uart signal is received
 * 
 * it chekes if the first byte is 0xBD if true it send the second byte the the apiState function
 * 
 * @name RDA_ISR (interrupt)
 */
#int_RDA
void RDA_ISR(void)
{
    int receivedByte;
    if (getc() == 0xBD) // procced only if we have 0xBD received
    {
        receivedByte = getc();  // get the next byte
        apiState(receivedByte); // fire the apiState function with the received parameters
    }
}

/**
 * @brief this the key function of the application
 * where we create a sequential delay 
 * 
 * @param delayNeeded 
 */
void delay(int delayNeeded)
{
    int x;
    for (x = 1; x < delayNeeded; x++)
    {
        delay_us(65); // wait 65 micro second adjusted for 60Hz
    }
}
/**
 * @brief external interrupt   ( this is the most important function ! )
 * 
 * @function observing the 60 MHZ oscillation of the 220/110 volts current and controlling our output, 
 * the following function will fire when receiving a signal on the microcontroller external interupt (from the 4N35)
 * the 4N35 (with the bridge) will send a signal on every half oscillation of the 220/110V 60 MHZ input current
 * that means on a 60 MHZ current it will fire 120 times (60 * 2) on every second, 
 * because of 60 MHZ => means 60 oscillation per second and as we said before it fire on every half oscillation. 
 * When it recieve a signal from the 4N35 it will wait a specific period and send a 10 microseconds signal 
 * to the MOC3020 that the MOC3020(with the triac) will allow the 220/110 V current to follow starting 
 * from the time the microcontroller send that signal until the end of a half oscillation
 * 
 * @WARNING make sure that the is no physical connection between the 220/110 volts circuit and 5 volts circuit
 * 
 * 
 */
#int_EXT
void EXT_isr(void)
{
    delay(lightLevel);   // time to hold before procceding
    OUTPUT_HIGH(PIN_B6); // activating the MOC3020 and allowing the 220/110 volts current to flow
    delay_us(10);        // hold the  MOC3020 active
    OUTPUT_LOW(PIN_B6);  // prevent the 220/110 volts current to flow by disactivating the MOC3020
}

/**
 * @brief Construct a new adc object 
 * Select the analog to digital chanel
 * Read the volts that is drawwed  on our microcontroller
 *  
 * @param pinRead 
 * @return the value that has been read from the volts that is drawwed into our microcontroller
 */

adc(int pinRead)
{
    set_adc_channel(pinRead);
    delay_us(20);
    out = read_adc();
    return out;
}

/**
 * @brief Manual control controller action depends on the modeSelector function
 * 
 */
void provide_action()
{
    if (modeSelector > 0 && manualControl == 1)
    {
        switch (modeSelector)
        {

        case 1: // manual luminosity adjustment by the variable resistor
            // variable resistor
            clickTiming = 0;              // reset the press counter
            OUTPUT_HIGH(PIN_B7);          // relay on
            output_high(PIN_B3);          // Light indicator red on
            output_Low(PIN_B4);           // Light indicator green off
            lightLevel = (int)adc(0) / 2; // analog to digital convertion from the variable resistor
            break;

        case 2: // adjusting luminosity to the surrounding light
            // the lamp is lightning according to outside light that is observed from the light sensor:
            // light sensor to observed the out side light
            // variable resistor:- if is greater then first half then the more light outside the more light our lamp lights
            //                  :- if is less then first half then the more light outside the less light
            clickTiming = 0;     // reset the press counter
            OUTPUT_HIGH(PIN_B7); // relay on
            output_Low(PIN_B3);  // Light indicator green on
            output_high(PIN_B4); // Light indicator red off
            if (adc(0) > 128)    // check if analog to digital convertion of variable resistor more then the 128
            {
                // light adjusting opposite to the surrounding light (if outside there is light then our lamp will not light and vise-versa)
                lightLevel = (128 - ((int)adc(1) / 2));
            }
            else
            {
                // light adjusting to the surrounding light (if outside there is light then our lamp will light and vise-versa)
                lightLevel = (int)adc(1) / 2;
            }
            break;

        case 3: // light on body movement detection
            // movement detector : to detect movement
            // Variable resistor : adjust the desired lightening level
            clickTiming = 0;     // reset the press counter
            OUTPUT_HIGH(PIN_B7); // relay on
            output_high(PIN_B3); // led indicator
            output_high(PIN_B4); // led indicator
            if (input(PIN_A2))   // a movement has been detected
            {
                lightLevel = (int)adc(0) / 2; // get the variable resistor state and assign it to light level
                output_low(PIN_B4);           // led indicator, indicate a movement has been detected
            }
            if (!input(PIN_A2)) // no movement detected
            {
                lightLevel = 128;    // light off
                output_high(PIN_B4); // led indicator, indicate no movement has been detected
                OUTPUT_LOW(PIN_B7);  // prevent the current from flowing to the lamp
            }
            break;

        case 4: // mode 4 will lights if the surrounding light is not enough and there is a body motion detected
            // light sensor: to detect if there is sufisan light
            // motion detector : to detect if a body is moving
            // variable resistor : to adjust the prefered output
            clickTiming = 0; // reset the press counter
            OUTPUT_HIGH(PIN_B7);
            if (input(PIN_A2) && adc(1) < 40) // input(PIN_A2) if active then motion is detected
            // adc(1) < 40 : analog to digital convert of port 1 (light sensor), 40 is the base condition can be change deppending on the sourrounding
            {
                lightLevel = (int)adc(0) / 2; // light level output will be taken from
                if (motionFlag)               // function fire first time after detecting a movement
                {
                    output_toggle(PIN_B3); // led light indication
                    motionFlag = 0;        // to prevent the loop from firing again
                }
            }
            if (!input(PIN_A2)) // no motion detected
            {
                lightLevel = 128; // lights off
                if (!motionFlag)  // function fire first time after not detecting any movement
                {
                    output_toggle(PIN_B3); // led light indication
                    motionFlag = 1;        // to prevent the loop from firing again
                }
            }
            break;

        case 5: // this mode will dime the lamp from the lowest level to the highest level and vise-versa on an infinit loop
            // variable resistor to control the speed of the loop
            OUTPUT_HIGH(PIN_B7); // relay on
            loopBreak = 1;       // loop mode is on
            clickTiming = 0;     // reset the press timing
            break;

        case 6:                 // exit loop mode lamp light is off
            clickTiming = 0;    // reset the press timing
            OUTPUT_Low(PIN_B7); //relay off
            loopBreak = 0;      // no loop
            lightLevel = 128;   // light level off
            break;

        case 7:                  // final mode lamp just full on light
            OUTPUT_high(PIN_B7); //relay on
            loopBreak = 0;       // no loop
            clickTiming = 0;     // reset the press timing
            lightLevel = 128;    // light level high
            break;
        }
    }
}

/**
 * @brief observe the button pressin behavior, increment the mode selector onh each long press and provide action accordingly
 */

void GetclickEvent()
{
    while (input(PIN_B1) != 1) // button is pressed
    {
        clickTiming++;   // increment counter
        buttonPress = 1; // button flag is raised
    }

    if (buttonPress && clickTiming > 40000) // button is long pressed
    {
        modeSelector++;       // increment the mode selector
        buttonPress = 0;      // reset the button press
        clickTiming = 0;      // reset the button press counter
        manualControl = 1;    // manual control activated
        if (modeSelector > 7) // only seven mode allowded
        {
            modeSelector = 0;
        }
        provide_action();
    }
}

void main()
{
    unsigned incrementLoop, decrement;
    loopBreak = 0;    // starts the program without loop
    lightLevel = 128; // starts the program lamp is off

    set_tris_a(0b00000111); // setting the I/O pins of port A --> 1 pin will be set as input --- 0 as digital output
    set_tris_b(0b0100101);  // setting the I/O pins of port B --> 1 pin will be set as input --- 0 as digital output
    OUTPUT_LOW(PIN_B7);     // rellay is off
    OUTPUT_LOW(PIN_B3);     // led light indicators
    OUTPUT_LOW(PIN_B4);     // led light indicators

    setup_adc_ports(sAN0 | sAN1); // set up the analoge to degital converter ports (two ports selected)
    setup_adc(ADC_CLOCK_DIV_32);  // set up the analoge to degital converter settings (two ports selected)
    enable_interrupts(INT_EXT);   // enabling external interrupt
    enable_interrupts(INT_RDA);   // enabling uarl interrupt
    ext_int_edge(H_TO_L);         // external interrupt raise when signal goes from high to low
    enable_interrupts(GLOBAL);    // all interrupt are enabled

    /**
     ***************************************************************************
     *                             MAIN LOOP
     * *************************************************************************
     * Finally reaching this final stage 
     * cant wait !!!
     * 
     * the following loop will execute as long as the device is on
     */

    while (1) // infinit loop
    {
        GetclickEvent(); // observe the the click behavior and provide an event accordingly

        if (loopBreak) // loop flag is raised dime the lamp from the lowest level to the highest level on infinit loop and vise-versa
        {
            for (decrement = 127; decrement >= 1; decrement--) // dimming from low to high
            {
                output_low(PIN_B3);     // led light indication
                output_high(PIN_B4);    // led light indication
                lightLevel = decrement; // lamp lightning recieve the current loop state value
                delay_ms(adc(0));       // adc(0) is the variable resistor , will wait on current state as the variable resistor indicate
                if (input(PIN_B1) != 1) // break the loop if button is pressed
                {
                    loopBreak = 0;    // loop disactivated
                    modeSelector = 6; // selected mode lamp is off
                }

                if (!loopBreak) // go outside of the loop if the flag is not raised
                    break;
            }
            for (incrementLoop = 1; incrementLoop <= 127; incrementLoop++) // dimming from high to low
            {
                output_low(PIN_B4);         // led light indication
                output_high(PIN_B3);        // led light indication
                lightLevel = incrementLoop; // lamp lightning recieve the current incrementLoop state value
                delay_ms(adc(0));           // adc(0) is the variable resistor , will wait on current state as the variable resistor indicate
                if (input(PIN_B1) != 1)     // break the loop if button is pressed
                {
                    loopBreak = 0;    // loop disactivated
                    modeSelector = 6; // selected mode lamp is off
                }

                if (!loopBreak) // go outside of the loop if the flag is not raised
                    break;
            }
        }
    }
}
