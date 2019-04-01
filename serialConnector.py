# @Author : Mohamed Snoussi
# @Email : sooniic@live.com
#################################################################################
# Python program
#################################################################################
# The program load a json file from the internet and compere it with another locally 
# stored json file (computer local storage)
# 
# if the last entry(value) in the locally stored json file doesn't match the the first entry 
# of the internet loaded json file then it send a uart signal through a USB port
# (usb to serial converter needed) and append the new value on a new line in the the locally stored json file.
# then repeat on an infinite loop every randomly period (from 3 to 6 second)
#################################################################################

# importing needed libraries ####################################################
from time import gmtime, strftime
import time, random, os ,serial, json


try:
    # For Python 3.0 and later
    from urllib.request import urlopen
except ImportError:
    # Fall back to Python 2's urllib2
    from urllib2 import urlopen



# functions ####################################################

# @FunctionName : consoleInfo
# @Role : print the received instraction on the console
# @Param : mode (the mode in the json file hosted in the internet)
#          repeat (how many time the instraction will be exuted)   essential to succeed with the radio frequencies rf433Mhz   )

def consoleInfo(mode ,repeat):
    repeat=str(int(repeat)*10) # multiplying by 10 as the sent signal
    return {
    'a': 'Sending '+ repeat +' signals to dime light slowly from full to no brightness ',
    '0': 'Sending '+ repeat +' signals to switch light off',
    '1': 'Sending '+ repeat +' signals to open only 10 %\ of light brightness',
    '2': 'Sending '+ repeat +' signals to open only 21 %\ of light brightness',
    '3': 'Sending '+ repeat +' signals to open 35 %\ of light brightness',
    '4': 'Sending '+ repeat +' signals to open 47 %\ of light brightness',
    '5': 'Sending '+ repeat +' signals to open 58 %\ of light brightness',
    '6': 'Sending '+ repeat +' signals to open 63%\ of light brightness',
    '7': 'Sending '+ repeat +' signals to open 75%\ of light brightness',
    '8': 'Sending '+ repeat +' signals to open 87 %\ of light brightness',
    '9': 'Sending '+ repeat +' signals to open all 100 %\ of light brightness',
    'x': 'error, not right instraction',
    }.get(mode, 'x') # return a log that corresbonds to the mode and a fallback default as second parameter 'x' to the error

# @FunctionName : getJsonFromTheNet
# @Role : read and decode a json from the net 
# @Param : url (the url of the json file)
# @Param : json file
def getJsonFromTheNet(url):
    response = urlopen(url) # load the file
    data = response.read().decode("utf-8") # read and decode the loaded json file
    return json.loads(data) # return the decoded file

# Important variables ####################################################


url = ("http://hospitality-entertainment.com/org/Jaz_Tour_Khalef/inte.json") # url of the json file
usbPort = 'COM3' # usb port
baudRate = 1200 # baud rate
serialPort = serial.Serial(usbPort,baudRate)  # open the serial port
LocalJson = 'modeHistory.json' # local json path 
keyByte = '½' # key byte 0xBD (hexadecimal) == ½ (ASCII)
waitingPeriodLoop = 3 #

# Program starts !!! ####################################################

print ("ONLY ONLINE CONTROLE") # print on terminal

# infinite loop ####################################################

while(1):
    try: 
        randomPeriod=random.uniform(0, 3) # Random waiting period 
        internetLoadedJson=(getJsonFromTheNet(url)) # get and encode the internet loaded json
        instractionRepeat= int(internetLoadedJson["My_home"]["Times"]) # get the signal repeat from the internet loaded json
        internetLoadedMode=(internetLoadedJson["My_home"]["Mode"]) # get the mode from the internet loaded json
        with open(LocalJson) as json_data: # get the locally stored Json
                locallyLoadedJson = json.load(json_data)
        locallyLoadedModeLastEntry=locallyLoadedJson['My_home'][-1]['Mode'] # read the last entry
        if internetLoadedMode != locallyLoadedMode: # if true the internet loaded file is changed !! 
            timeStamp = strftime("%Y-%m-%d %H:%M:%S", gmtime()) + " GM Time" # get the time stamp 
            terminalMessage = consoleInfo(internetLoadedMode,instractionRepeat) # prepare the terminal message
            print('\n- Instruction received on ' + timeStamp + '\n- ' + terminalMessage) # print the terminal message
            newEntry = {"Date":timeStamp ,"Mode":str(internetLoadedMode),'Times':str(instractionRepeat)} #  prepare a new json entry
            locallyLoadedJson['My_home'].append(newEntry) # write and append the internet new json entry to the program memory (buffer)
            with open('inte.json', 'w') as f:
                json.dump(internetLoadedJson, f, indent=4, sort_keys=True) # write from the buffer to the hard driver as a new json file
            serialPort.close() # make sure that previously opened serial connection is closed 
            newSingalToSend = keyByte + str(internetLoadedMode) # the signal the will be send through the usb-serial
            serialPort.open() # open the serial port
            for i in (1,10): # the instraction will be repeated in order one of the signals success reach the other end of the radio frequency receiver
                for j in (0, instractionRepeat*10):
                    serialPort.write(newSingalToSend.encode())
                    time.sleep(0.03) # wait until proceeding the next write
                time.sleep(0.3) # wait until proceeding the next iteration 
            serialPort.close() # make sure that previously opened serial connection is closed 
        print('Waiting for instruction', end='\r') # Print on the terminal 
        time.sleep(3+randomPeriod) # wait until repeat the forever loop
    except KeyboardInterrupt: # if a keyboard key is pressed
        exit() # exit the program if a keyboard key is pressed