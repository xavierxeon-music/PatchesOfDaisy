#!/usr/bin/env python3

from .note import Note

class Wave(dict):

   class Shape:

      Sine = 0
      Saw = 1
      Square = 2
      SlopeUp = 3
      SlopeDown = 4

   def __init__(self):

      dict.__init__(self, note = Note.Z0, shape = Wave.Shape.Sine)

   @property
   def note(self):
      return self['note']

   @note.setter
   def note(self, newNote):
      self['note'] = newNote

   @property
   def shape(self):
      return self['shape']

   @shape.setter
   def shape(self, newShape):
      self['shape'] = newShape      

