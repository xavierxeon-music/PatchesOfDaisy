#!/usr/bin/env python3

from .wave import Wave

class Input:

   def __init__(self):

      self.cv = [0.0, 0.0, 0.0, 0.0]
      self.gate = [False, False]
      self.wave = [Wave(), Wave(), Wave(), Wave()]
      