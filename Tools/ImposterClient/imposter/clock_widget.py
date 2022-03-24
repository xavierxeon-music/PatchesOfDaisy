#!/usr/bin/env python3

from PySide6.QtWidgets import QWidget, QPushButton, QLabel,  QVBoxLayout

class ClockWidget(QWidget):

   def __init__(self, bpm):

      QWidget.__init__(self)
      self._setupUi()

      self.once = False
      self.running = False
      self.reset = False
      self.mSecsPerTick = float(60 * 1000) / float(4 * bpm)
      self.elapsedTime = 0.0

   def fillInput(self, input, callBackIntervall):

      if self.reset:

         self.reset = False
         self.gateIndex = 0
         input.gate[1] = True
      
      elif self.running or self.once:

         self.elapsedTime += callBackIntervall

         # empulate clock pulse

         if self.elapsedTime  > self.mSecsPerTick:
            self.once = False
            self.elapsedTime = 0.0
            input.gate[0] = True 
         else:
            input.gate[0] =  False

   def _setupUi(self):

      label = QLabel()
      label.setText('Imposter Clock')

      self.stepButton = QPushButton()
      self.stepButton.setText('1 Tick')
      self.stepButton.clicked.connect(self._singleTick)

      self.startButton = QPushButton()
      self.startButton.setText('Start')
      self.startButton.clicked.connect(self._startClock)

      self.stopButton = QPushButton()
      self.stopButton.setText('Stop')
      self.stopButton.clicked.connect(self._stopClock)
      
      self.resetButton = QPushButton()
      self.resetButton.setText('Reset')
      self.resetButton.clicked.connect(self._resetClock)

      masterLayout = QVBoxLayout()
      masterLayout.addWidget(label)
      masterLayout.addWidget(self.stepButton)
      masterLayout.addWidget(self.startButton)
      masterLayout.addWidget(self.stopButton)
      masterLayout.addWidget(self.resetButton)
      self.setLayout(masterLayout)

      self.setMinimumWidth(200)

   def _singleTick(self):

      print('Single Tick')
      if self.running or self.reset:
         return

      self.elapsedTime =  self.mSecsPerTick + 1
      self.once = True

   def _startClock(self) :

      print('Start Clock')

      self.elapsedTime =  self.mSecsPerTick + 1
      self.running = True

   def _stopClock(self) :

      print('Stop Clock')
      
      self.running = False

   def _resetClock(self) :

      print('Reset Clock')
      
      self.reset = True
