#!/usr/bin/env python3

import sys, os

def main():

   if len(sys.argv) != 2:
      print('no file name given')
      sys.exit()

   fileName = os.getcwd() + '/' + sys.argv[1]
   fileName = os.path.realpath(fileName)

   width = None
   height = 0

   data = list()

   with open(fileName) as infile:
      for line in infile:
         
         data.append(list())
         row = data[height]

         line = line.strip()
         lineWidth = len(line)
         if None == width:
            width = lineWidth
         elif width != lineWidth:               
            print(f'line {height} width mismatch. should be {width}')
            sys.exit()
         
         for letter in line:
            row.append('false' if letter == '-' else 'true')
         height += 1

   if width > 128:
      print('image to wide for screen')

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
         pixel = data[y][x]
         print(pixel, end='')
      print(' }', end='')
   print('')
   print('};')      


if __name__ == '__main__':
   main()
