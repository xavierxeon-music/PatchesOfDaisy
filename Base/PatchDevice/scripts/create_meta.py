#!/usr/bin/env python3

import json, os

class DataType(dict):

   def __init__(self, dataType):

      dict.__init__(self)

      if not dataType:
         self['in_build'] = True
         self['type'] = None
      elif isinstance(dataType, str):
         self['in_build'] = False
         self['type'] = dataType
      else:
         self['in_build'] = True
         self['type'] = dataType.__name__
      

class Array(dict):

   def __init__(self, dataType, size, default = None):

      dict.__init__(self)

      self['size'] = size
      self['dataType'] = DataType(dataType)
      self['default'] = default

class Scalar(Array):

   def __init__(self, dataType, default = None):

      Array.__init__(self, dataType, 1 , default)

class FunctionArg(dict):

   def __init__(self, name, dataType):
      
      dict.__init__(self)

      self['name'] = name
      self['dataType'] = DataType(dataType)

metaData = {
   'Wave': {
      'properties': {
         'note': Scalar(int, 36),
         'shape': Scalar(int, 0)
      },
      'functions': {}
   },
   'Input': {
      'properties': {
         'cvs': Array(float, 4, 0.0),
         'gates': Array(bool, 2, None),
         'waves': Array('Wave', 4)
      },
      'functions': {}
   },
   'Output': {
      'properties': {
         'cvs': Array(float, 2, 0.0),
         'gate': Scalar(bool, False),
         'led': Scalar(bool, False)
      },
      'functions': {}
   },
   "Imposter": {
      'properties': {},
      'functions': {
         'fillInput': {
            'out': DataType('Input'),
            'in': [
               FunctionArg('test', int)
            ]
         },
         'loopDone': {
            'out': DataType(None),
            'in': [
               FunctionArg('output', 'Output')
            ]
         }
      }
   }      
}
   
def main():

   with open('meta.json', 'w') as outfile:
      json.dump(metaData, outfile, indent = 3)

if __name__ == '__main__':
   main()
   