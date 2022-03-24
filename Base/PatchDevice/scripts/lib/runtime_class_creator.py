#!/usr/bin/env python3

# see https://www.python-course.eu/python3_classes_and_type.php

import json, sys, inspect, types

class Test:

   a = str()
   b = [int(), int()]

   def __init__(self):

      self.a = str()
      self.b = [5, 5]

   def doStuff(self, name):

      print(name)

class RuntimeClassCreator:

   def __init__(self, fileName):

      with open(fileName, 'r') as infile:
         self.meta = json.load(infile)

      self.metaDict = dict()

      self.firstPass()

      for className, propertyDict in self.metaDict.items():
         globals()[className] = type(className, (), propertyDict)            


   def firstPass(self):
   
      for index, (className, content) in enumerate(self.meta.items()):

         propertyDict = dict()
         if 'properties' in content:
            self._properties(propertyDict, content['properties'])
         if 'functions' in content:
            self._functions(propertyDict, content['functions'])
         self.metaDict[className] = propertyDict

   def _properties(self, propertyDict, content):

      if not content:
         return

      for name, info in content.items():
         in_build = bool(info['dataType']['in_build'])
         dataType = info['dataType']['type']
         default = info['default']
         size = int(info['size'])

         if in_build:
            dataType = eval(dataType)
            default = dataType(default)
            if 1 == size:
               propertyDict[name] = default
            else:
               dataList = [default] * size
               propertyDict[name] = dataList      

   def _functions(self, propertyDict, content):

      if not content:
         return

      def create_function(name, argCount):
         def template(): pass

         y_code = types.CodeType(argCount,
                                 template.co_posonlyargcount,
                                 template.co_stacksize,
                                 template.co_flags,
                                 template.co_code,
                                 template.co_consts,
                                 template.co_names,
                                 template.co_varnames,
                                 template.co_filename,
                                 name,
                                 template.co_firstlineno,
                                 template.co_lnotab)

         return types.FunctionType(y_code, template.func_globals, name)         

      for name, info in content.items():
         print('  function', name, info)
         #propertyDict[name] = lambda self: print("Hi, I am " + name)
         propertyDict[name] = create_function(name, 1)

   @staticmethod
   def listAllClassesCreated():

      current_module = sys.modules[__name__]
      #print(current_module)
      for name, obj in inspect.getmembers(current_module):
         if not inspect.isclass(obj):
            continue

         className = obj.__name__
         if className == RuntimeClassCreator.__name__:
            continue

         print(className)      
         for key, value  in obj.__dict__.items():
            if ketemplate.startswith('__'):
               continue
            print('  ', key, value, type(value))