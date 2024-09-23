# CircuitBreakerGadget

## Project Overview
Designed as an introduction into hardware hacking, the Glitchy can do power glitching to alter program execution and power analysis for extracting secrets from chips. 
See the [wiki](https://github.com/miswired/glitchy/wiki) for setup instructions and guides to performing the labs with the kit.

## Install
This project is hosted in two repos. The board firmware and PCB is hosted here. Please see the following instructions for setting up the build enviroment and programming. [Arduino-Setup](https://github.com/miswired/glitchy/wiki/Guide-%E2%80%90-Arduino-Setup)   

**The web gui has be substatually upgradded by Philip Kopyscinski!**  
See the following repo for the files you need to put on the SD card.  
https://github.com/pkopysci/miswired-glitchy-vue-app

## Comming Features
* Scripting power analysis attack
* Configure wifi from a config file on sd card isntead of having to reprogram

## Release Notes
### 2.0.0
#### New Features
* Completely new web GUI!!!
* Adjustability of glitching parameters from web interface

### 1.1.0
#### New Features
* None

#### Bug Fixes
* Fixed loading bug for the web interface

#### Backlog

* Add configuration of the glitching settings to the web interface instead of needing to reflash the chip
* Add wifi configuration to a config.ini file on the SD card instead of needing to reflash the chip

## Project Setup
Install Arduino IDE V1. Please see the following wiki for a setup guide. [Arduino-Setup](https://github.com/miswired/glitchy/wiki/Guide-%E2%80%90-Arduino-Setup)  
