# Pongbot

A new tf2 bot fully written from scratch.
As of now, it is only working on Windows. Linux support is being considered later on.

[Steam Group](https://steamcommunity.com/groups/pongbot)

## Building & Developing

Import the project to Visual Studio (preferably 2019) on Windows.
Afterwards follow these steps to set the project up properly:
```
- Select Release build flavour (Debug won't compile as of now)
- Select x86 as target architecture (Can't compile hl2 sdk with x64 as target)
- Open Project Properties
- Under General, set Configuration Type to "Dynamic Library (.dll)"
- Open C/C++ -> General and add "lib" to Additional Include Directories
- Open Linker and do the following:
	- In General, add "lib/hlsdk/lib" to Additional Library Directories
	- In Input, add the following to Additional Dependencies: "legacy_stdio_definitions.lib;tier0.lib;tier1.lib;tier2.lib;tier3.lib;vstdlib.lib;mathlib.lib"
- Check if there's a command line set in Build Events -> Post-Build Events
	- If there is, either change it the path of your choosing or completely remove it
```
And everything should be ready to go!

## Installation

See [here](https://steamcommunity.com/groups/pongbot/discussions/0/3561682880008371471/).

## Credits

- [rcbotCheeseh](https://github.com/rcbotCheeseh) - Being much of a help and using parts of his RCBot2 as reference