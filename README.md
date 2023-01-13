# Patches Of Daisy

This stared as a support library (Base/Common/PatchDevice) for creating Dasiy Patch firmware.

It now also features:
   * a library of useful tools (Base/Common/MusicTools) for creating music related applications
   * a Qt applications (Base/PatchImposter) with which to develop those firmwares. 
   * a python interface (Tools/ImposterClient) to script user input for the imposter.
   * a framework (Base/Common/Remember) to load and save the current state of the firmware (to flash memory on the seed or to file in an application)
   * Qt based companion apps (Base/SeedCompanion) to provide a PC gui. Uses USB midi to load and save the state of the Daisy Seed hardware.
   * The ability to write Qt applications that use DC-coupled audio hardware (tested with Expert Sleepers ES8), with input/output classe for Gates, CV & Audio, that can be used with MusicTools
   * Soon to come: write firmware for NoiseEngineering Versio devices

# Installation   

## Hardware

 * use installer from daisy wiki
 * build libdaisy with Base/build_libdaisy.sh (only if you want to deploy to seed hardware)

## PatchImposter (Qt GUI to test programs)

 * needs Qt6 
 * (optional) for python scripting 
   * PySide6 
   * run devel_setup.sh in ImposterClient

# Usage

## device upload using USB 
 * make program-dfu (after entering bootloader mode)
 * make program (with attached debugger)

## debug seed

 * see https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment
 * from repository root folder call set_debug_project.py an point it to you current porject, e.g set_debug_project.py Project/TimeLord
 * open repository root folder in VS Code
 
