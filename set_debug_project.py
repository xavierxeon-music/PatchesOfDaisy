#!/usr/bin/env python3

import argparse
import os
import sys
import json
from shutil import copyfile


class DebuggerFiles:

    def __init__(self, projectDir):

        thisScriptFile = os.path.realpath(__file__)
        thisScriptDir = os.path.dirname(thisScriptFile)

        self.projectDir = None
        self.projectName = None

        self._findProject(thisScriptDir, projectDir)

        if not self.projectDir or not self.projectName:
            print('unable to find projeect')
            sys.exit(1)

        self.targetDir = os.getcwd() + '/.vscode'
        os.makedirs(self.targetDir, exist_ok=True)

        self.sourceDir = thisScriptDir + '/Tools/DebuggerFiles'

    def clean(self):

        fileBlackList = ['settings.json']
        for entry in os.scandir(self.targetDir):
            if entry.name in fileBlackList:
                continue

            print(f'clean up: remove {entry.name}')
            os.remove(entry.path)

    def copyFiles(self):

        self._copyLaunch()
        self._copyTasks()
        copyfile(self.sourceDir + '/STM32H750x.svd',  self.targetDir + '/STM32H750x.svd')

    def _copyLaunch(self):

        with open(self.sourceDir + '/launch.json', 'r') as infile:
            content = json.load(infile)

        config = content['configurations'][0]
        config['cwd'] = self.projectDir
        config['debuggerArgs'] = ['-d', os.getcwd()]
        config['executable'] = self.projectDir + '/build/' + self.projectName + '.elf'
        config['svdFile'] = os.getcwd() + '/.vscode/STM32H750x.svd'

        with open(self.targetDir + '/launch.json', 'w') as outfile:
            json.dump(content, outfile, indent=3)

    def _copyTasks(self):

        with open(self.sourceDir + '/tasks.json', 'r') as infile:
            content = json.load(infile)

        makeTask = content['tasks'][0]
        makeTask['options']['cwd'] = self.projectDir

        with open(self.targetDir + '/tasks.json', 'w') as outfile:
            json.dump(content, outfile, indent=3)

    def _findProject(self, thisScriptDir, projectDir):

        def searchMakefile(searchDir):

            makeFileName = searchDir + '/Makefile'
            if os.path.exists(makeFileName):
                self.projectDir = searchDir
                return makeFileName

            for entry in os.scandir(searchDir):
                if not entry.is_dir():
                    continue
                return searchMakefile(entry.path)

            return None

        startDir = thisScriptDir + '/' + projectDir
        makeFileName = searchMakefile(startDir)

        with open(makeFileName, 'r') as infile:

            for line in infile:
                if not line.startswith('TARGET'):
                    continue
                line = line.replace('TARGET', '')
                line = line.replace('=', '')
                self.projectName = line.strip()
                break


def main():

    parser = argparse.ArgumentParser(description='Set the active debug project.')
    parser.add_argument('projectPath', metavar='PROJECT_PATH', type=str, help='the path to the project')

    args = parser.parse_args()
    projectDir = args.projectPath

    df = DebuggerFiles(projectDir)
    df.clean()
    df.copyFiles()


if __name__ == '__main__':
    main()
