#!/usr/local/bin/python3
# -*- coding: utf-8 -*-

#  for testing the communication between Python and Arduino

import time

# === custom module ===
import serial_comm as comm

# === commands to Arduino ===
def commandStart():
    print("")
    print("=== send start command ===")
    print("write to Arduino: '!' ")
    ser.write(b'!')
    time.sleep(0.5)
    response = ser.read()
    print(response)
    while response != b'C':
        print("retry...")
        ser.reset_input_buffer()
        time.sleep(0.5)
        ser.write(b'!')
        time.sleep(0.5)
        response = ser.read()
        print(response)
    print("response of Arduino: 'C' ")
    print("=== send start command successful! ===")

def commandStop():
    print("")
    print("=== send stop command ===")
    print("write to Arduino: '*' ")
    ser.write(b'*')
    time.sleep(0.5)
    response = ser.read()
    print(response)
    while response != b'K':
        print("retry...")
        ser.reset_input_buffer()
        time.sleep(0.5)
        ser.write(b'*')
        time.sleep(0.5)
        response = ser.read()
        print(response)
    print("response of Arduino: 'K' ")
    print("=== send stop command successful! ===")


ser = comm.serial.Serial('/dev/cu.usbserial-AL03KKZE', baudrate=9600, timeout=1)
while not ser.is_open:
    print("retry open seriral port...")
    time.sleep(0.5)
    ser = comm.serial.Serial('/dev/cu.usbserial-AL03KKZE', baudrate=9600, timeout=1)
print(ser.name + " opened succefully!")
# to empty the buffer, it's better to use ser.read(1000)
# ser.flush()
ser.read(1000)

# according to Arduino offical documentation：https://playground.arduino.cc/Interfacing/Python
#
# The Arduino serial device takes some time to load, and when a serial connection
# is established it resets the Arduino. Any write() commands issued before the device
# initialised will be lost. A robust server side script will read from the serial
# port until the Arduino declares itself ready, and then issue write commands.
# Alternatively It is possible to work around this issue by simply placing a
# 'time.sleep(2)' call between the serial connection and the write call.
#
time.sleep(2)
commandStart()

while True:
    line = ser.readline()
    print(line)
    print("input waiting: " + str(ser.in_waiting))
    print("output waiting: " + str(ser.out_waiting))
    # print(ser.get_settings())

    # s = ser.read(1000)
    # print(s)

    time.sleep(1)
