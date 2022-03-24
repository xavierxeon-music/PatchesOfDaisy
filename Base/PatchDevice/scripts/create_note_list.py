#!/usr/bin/env python3

import json


def freqET(midi, octave):

   # see https://en.wikipedia.org/wiki/Equal_temperament

   base = 2.0**(1.0/12.0)  # 12th root of 2
   pitchA4 = 440

   octaveC = 12 + (12*octave)
   octaveA = octaveC + 9
   octavePitch = pitchA4*(2**(octave - 4))

   diff = midi - octaveA
   f = octavePitch * (base**diff)

   return f

def freqFromCV(voltage):

   baseFreq = [ 65.40639132514963, 130.81278265029925, 261.6255653005985, 523.251130601197,  1046.502261202394, 2093.004522404789]
   octave = int(voltage)
   rest = voltage - octave
   value = baseFreq[octave] * 2**rest
   return value

def createNameDict():

   baseNames = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
   midiC2 = 36

   midi = midiC2
   allNames = dict()

   def addToNames(noteName, midi):

      allNames[noteName] = {
         'midi': midi,
         'freq': freqET(midi, index),
         'cv': (midi - midiC2) / 12
      }

   for index in range(2, 7):
      for baseName in baseNames:
         name = baseName + str(index)
         addToNames(name, midi)
         midi += 1

   addToNames('C7', midi) 

   return allNames

def exportNoteList():

   allNames = createNameDict()

   print('#include <Sound/Note.h>')
   print('')
   print('// clang-format off')
   print('const Note::List Note::availableNotes =')
   print('{')

   for name, entry in allNames.items():
      print('   {' + f'"{name}", {entry["freq"]}f, {entry["cv"]}f, {entry["midi"]}' + ' },')

   print('};')
   print('// clang-format on')

def test():

   allNames = createNameDict()
   for name, entry in allNames.items():
      cv = entry["cv"]
      freqCalc = freqFromCV(cv)
      freqTarget = entry["freq"]
      print(f'{name}, {cv:.2f} => {freqTarget:.2f}, {freqCalc:.2f}')

if __name__ == '__main__':
   #test()
   exportNoteList()
