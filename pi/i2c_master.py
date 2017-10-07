#!/usr/bin/env python3
import smbus


class Arduino():
    def __init__(self, address=0x2f, bus_num=1):
        self.bus = smbus.SMBus(bus_num)  # 1 if Pi Zero or 3, 0 if Pi 2 or 1
        self.address = address

    def send(self, message, register=0x00, verify=True):
        '''Send message to Arduino.'''
        self.bus.write_i2c_block_data(self.address, register,
                                      [ord(char) for char in message])
        if verify:
            return_message = self.recv()
            print(('Register: 0x{:0>2x}\nMessage: {}\nResponse: {}'
                   ).format(register, message, return_message))
            return return_message

    def recv(self):
        message_bytes = self.bus.read_i2c_block_data(self.address, 0x00)
        message_parts = [chr(byte) for byte in message_bytes]
        return ''.join(message_parts).strip()


if __name__ == '__main__':
    ard = Arduino()
    while True:
        print('Which register would you like to talk to?',
              '\t0x00\tSERVO_1',
              '\t0x10\tELEVATOR_THRUSTER', sep='\n')
        register = input('Enter a register: ')
        try:
            register = int(register, 16)
        except ValueError:
            register = int(register)
        message = str(input('Enter a message to send: '))
        ard.send(message, register=register)
        print()
