#!/usr/bin/env python3

import sys
import os


def patch(plistLocation):

    content = list()
    endIndex = 0
    with open(plistLocation, 'r') as infile:
        for index, line in enumerate(infile.readlines()):
            content.append(line)
            if '</dict>' in line:
                endIndex = index
            if 'NSMicrophoneUsageDescription' in line:
                return

    with open(plistLocation, 'w') as outfile:
        for index, line in enumerate(content):
            if endIndex == index:
                outfile.write('\t<key>NSMicrophoneUsageDescription</key>\n')
                outfile.write('\t<string>you WANT audio?</string>\n')
            outfile.write(line)


def main():

    targetDir = sys.argv[2]
    if len(sys.argv) == 4:
        targetDir = sys.argv[3]

    plistLocation = targetDir + '/' + sys.argv[1] + '.app/Contents/Info.plist'
    if not os.path.exists(plistLocation):
        return

    print('patch Info.plist')
    patch(plistLocation)


if __name__ == '__main__':
    main()
