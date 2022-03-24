#!/usr/bin/env python3

from lib import RuntimeClassCreator

def main():

   RuntimeClassCreator('meta.json')
   print()
   RuntimeClassCreator.listAllClassesCreated()

   #i = Input()
   #i.cvs[0] = 1.0

if __name__ == '__main__':
   main()   

