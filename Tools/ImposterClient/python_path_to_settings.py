#!/usr/bin/env python3

import sys
import json

from PySide6.QtCore import QStandardPaths

settingsFileName = QStandardPaths.writableLocation(QStandardPaths.ConfigLocation) + '/ImposterDaisyPatch/Settings.json'

with open(settingsFileName, 'r') as infile:
    data = json.load(infile)

if not 'python' in data:
    data['python'] = dict()
data['python']['executable'] = sys.executable

with open(settingsFileName, 'w') as outfile:
    json.dump(data, outfile, indent=3)
