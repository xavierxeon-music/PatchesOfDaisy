#!/usr/bin/env python3

from .lib import Client

from .input import Input

class ImposterClient(Client):

   def __init__(self):

      Client.__init__(self)

      self.setInputFunction = None

   def onSetInput(self, function):

      self.setInputFunction = function

   def run(self):

      Client._run(self)

   def _processRequest(self, requestDict):

      if 'SetInput' == requestDict['request']:
         
         input = Input()
         audioCallbackRate = requestDict['audioCallbackRate']
         callBackIntervall = 1000.0 / float(audioCallbackRate)
         if self.setInputFunction:
            self.setInputFunction(input, callBackIntervall)

         return {
            'reply': 'SetInput',
            'data': {
               'cv': input.cv,
               'gate': input.gate,
               'wave': input.wave
            }
         }      

      return Client._processRequest(requestDict)
