#!/usr/bin/env python3

import time

from rtmidi import MidiIn, MidiOut

name = 'Daisy Seed Built In'

print('opening virtual ports')

midiout = MidiOut()
midiout.open_virtual_port(name)

midiin = MidiIn()
midiin.open_virtual_port(name)

try:
    print('presss Control+C to exit')
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print('')

print('closing ports')

midiout.close_port()
midiin.close_port()
