#!/usr/bin/env python3

import sys

from imposter import ImposterClient, Input, Note, Wave

counter = 0
maxCounter = 3

def setInput(input, callBackIntervall):

   global counter
   global maxCounter

   input.wave[0].shape = Wave.Shape.Sine
   input.wave[0].note = Note.C7

   input.wave[1].shape = Wave.Shape.Saw
   input.wave[1].note = Note.C7

   input.wave[2].shape = Wave.Shape.Square
   input.wave[2].note = Note.C7

   counter += 1
   if counter == maxCounter:
      sys.exit()

def main():

   client = ImposterClient()
   client.onSetInput(setInput)
   client.run()

if __name__ == '__main__':
   main()
