# Pongbot

A new tf2 bot fully written from scratch.
As of now, it is only working on Windows. Linux support being considered later on.

[Steam Group](https://steamcommunity.com/groups/pongbot)

## Building & Developing

Import the project to Visual Studio (preferably 2017) on Windows.
Afterwards follow these steps to set the project up properly:
```
- Open Project Properties
- Under General, set Configuration Type to "Dynamic Library (.dll)"
- Open C/C++ -> General and add "lib" to Additional Include Directories
- Open Linker and do the following:
	- In General, add "lib/hlsdk/lib" to Additional Library Directories
	- In Input, add the following to Additional Dependencies: "legacy_stdio_definitions.lib;tier0.lib;tier1.lib;tier2.lib;tier3.lib;vstdlib.lib;mathlib.lib"
```
And everything should be ready to go!

## Installation

See [here](https://steamcommunity.com/groups/pongbot/discussions/0/3561682880008371471/)

## Credits

- [rcbotCheeseh](https://github.com/rcbotCheeseh) - Being much of a help and using parts of his RCBot2 as reference