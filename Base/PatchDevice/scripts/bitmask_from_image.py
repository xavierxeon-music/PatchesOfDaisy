#!/usr/bin/env python3

import sys, os

try:
   from matplotlib.image import imread
except ModuleNotFoundError:
   print('pip3 install --user matplotlib')
   sys.exit()

def main():

   if len(sys.argv) != 2:
      print('no file name given')
      sys.exit()

   fileName = os.getcwd() + '/' + sys.argv[1]
   fileName = os.path.realpath(fileName)

   im = imread(fileName)
   if im.shape[2] != 3:
      print('image must be in RGB')
      sys.exit()

   width = im.shape[1]
   if width > 128:
      print('image to wide for screen')

   height = im.shape[0]
   if height > 64:
      print('image to high for screen')

   print(f'const BitMask<{width}, {height}>::Data data = ')
   print('{')
   for x in range(width):
      if 0 != x:
         print(',')
      print('   { ', end='')
      for y in range(height):
         if 0 != y:
            print(', ', end='')
         pixel = im[y][x]
         sum = pixel[0] + pixel[1] + pixel[2]
         white = 'true' if (sum > 2) else 'false'
         print(white, end='')
      print(' }', end='')
   print('')
   print('};')

if __name__ == '__main__':
   main()
