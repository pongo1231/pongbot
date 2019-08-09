# Pongbot

A new tf2 bot fully written from scratch.

[Steam Group](https://steamcommunity.com/groups/pongbot)

# Building & Developing

## Windows

**Currently broken. Will look into it later.**

<details>
    <summary>Old Instructions</summary>
    
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
</details>

## Linux

Install `gcc` and `make` first.
Download the [hl2sdk](https://github.com/alliedmodders/hl2sdk/tree/tf2) (tf2-specific) and [metamod source code](https://github.com/alliedmodders/metamod-source).
Put the hl2sdk in a folder and rename it to hlsdk.
Put metamod/core into the same folder and rename it to metamod.
Now run the following in the project's root folder:
```
MLIBS=/path/to/the/folder/containing/hlsdk/and/metamod make
```
It should build and generate a Pongbot.so to build/bin/

# Installation

See [here](https://steamcommunity.com/groups/pongbot/discussions/0/3561682880008371471/).

# Credits

- [rcbotCheeseh](https://github.com/rcbotCheeseh) - Being much of a help and using parts of his RCBot2 as reference
