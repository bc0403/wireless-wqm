# wireless-wqm
wireless (Bluetooth) water quality monitor based on Arduino Uno/nano and Python3

## Usage
`python3 monitor.py`

## protocol between Arduino and Python
- The Arduion will send out the 3 float data points at every one second, that is temperature of solution (℃), voltage between pH electrode and Ag/AgCl reference electrode (mV), current flow out from working electrode of free Cl sensor, respectively.
