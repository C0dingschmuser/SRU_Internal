```
                     _____/\\\\\\\\\\\______/\\\\\\\\\______/\\\________/\\\_
                      ___/\\\/////////\\\__/\\\///////\\\___\/\\\_______\/\\\_
                       __\//\\\______\///__\/\\\_____\/\\\___\/\\\_______\/\\\_
                        ___\////\\\_________\/\\\\\\\\\\\/____\/\\\_______\/\\\_
                         ______\////\\\______\/\\\//////\\\____\/\\\_______\/\\\_
                          _________\////\\\___\/\\\____\//\\\___\/\\\_______\/\\\_
                           __/\\\______\//\\\__\/\\\_____\//\\\__\//\\\______/\\\__
                            _\///\\\\\\\\\\\/___\/\\\______\//\\\__\///\\\\\\\\\/___
__/\\\\\\\\\\\__________________\///////////_____\///________\///_____\/////////______________________/\\\\\\____
 _\/////\\\///________________________________________________________________________________________\////\\\____
  _____\/\\\_______________________/\\\___________________________________________________________________\/\\\____
   _____\/\\\______/\\/\\\\\\____/\\\\\\\\\\\_____/\\\\\\\\___/\\/\\\\\\\___/\\/\\\\\\____/\\\\\\\\\_______\/\\\____
    _____\/\\\_____\/\\\////\\\__\////\\\////____/\\\/////\\\_\/\\\/////\\\_\/\\\////\\\__\////////\\\______\/\\\____
     _____\/\\\_____\/\\\__\//\\\____\/\\\_______/\\\\\\\\\\\__\/\\\___\///__\/\\\__\//\\\___/\\\\\\\\\\_____\/\\\____
      _____\/\\\_____\/\\\___\/\\\____\/\\\_/\\__\//\\///////___\/\\\_________\/\\\___\/\\\__/\\\/////\\\_____\/\\\____
       __/\\\\\\\\\\\_\/\\\___\/\\\____\//\\\\\____\//\\\\\\\\\\_\/\\\_________\/\\\___\/\\\_\//\\\\\\\\/\\__/\\\\\\\\\_
        _\///////////__\///____\///______\/////______\//////////__\///__________\///____\///___\////////\//__\/////////__
```          

## SRU_Internal - The **Ultimate** Trainer for Supreme Ruler Ultimate

| [Features](#features) - [Usage](#usage) - [Troubleshooting](#troubleshooting) - [How it works](#how-it-works) - [Releases & Changelogs](#releases--changelogs) - [License](#license)|
:----------------------------------------------------------: |

<img src="https://bruh.games/internal/sru/github/main.png" width="700" height="545" />

### Features
- Modify stats of all countries (treasury, approval ratings, goods, defcon, etc.)
- Force treaties between countries like declaration of war, formal alliances, etc.
- Change diplomatic relations between countries
- Spawn Units (Customizable amount, owner, location, deployement type etc.)
- Spawn Facilities (Cities, Bases, Factories etc.)
- Edit Map (Change Borders, Loyalty, Annex/Colonize/Liberate etc.)
- Paint your own Islands, Land connections and/or Resources (Petroleum etc.)
- Lock/Unlock Technologies and Unit Designs for each country
- Per country unit modifiers (Invincibility, Lightspeed movement, Max Supply/Morale/Fuel/etc.)
- Modify every Minister of a Country
- Modify all Rules of Engagement of a Country
- Change AI Stance of a Country
- Change Hex Population
- Mouse Hover unit modifiers (Changing owner country, health, etc.)
- Adjustable global minimum hex supply, so units don't get stuck in the middle of nowhere
- Build Roads/Rails/Bridges everywhere! (no limits)
- Default AI can create colonies insted of just annexing everything
- Change Country colors, names and flags
- Much more!

### Usage
Simply download the latest Release from [here](https://github.com/C0dingschmuser/SRU_Internal/releases) or from the [Steam Workshop](https://steamcommunity.com/sharedfiles/filedetails/?id=2874935554)
Extract the zip file to a place of your choice and simply run the executable. The trainer will launch as soon as the game is running.
If evereything worked, you'll see the SRU_Internal window open. Once you enter a game you'll see all available options. Have fun!

### How it works
Contrary to ordinary mods or trainers, SRU_Interal gets injected into the core game process and has access to basically everything the game has access to. This means SRU_Internal is able to directly call, edit or hook game functions. To do so, it utilizes a slightly modified version of <br> [Dear ImGui](https://github.com/ocornut/imgui), [Kiero](https://github.com/Rebzzel/kiero) and [Minhook](https://github.com/TsudaKageyu/minhook).

### Troubleshooting
Since SRU_Internal directly manipulates the game's process memory it's possible that your antivirus flags this as harmful. **That is a false positive. The Trainer is safe. (For a detailed explanation, look below)** If that happens, you need to manually add an exception in your antivirus to exclude the trainer or a folder which contains the trainer. [Here](https://support.microsoft.com/en-us/windows/add-an-exclusion-to-windows-security-811816c0-4dfd-af4a-47e4-c301afe13b26) is a tutorial on how to do that with Windows Defender.
If you find a bug, feel free to open an [Issue](https://github.com/C0dingschmuser/SRU_Internal/issues) with a detailed description or contact me via **info@bruh.games**.

### Why can the AV flag this as harmful?
If you download a new unknow file, your AV (Antivirus) performs something called a "Heuristic analysis" to evaluate what the File/Program might do and compare it with a database of things that could be harmful. A good example for this is if a program listens for keypresses. SRU_Internal does this with the CAPSLOCK and CTRL Keys to check whether a player wants to paint the map or not. **But the AV doesn’t know that**, it just knows that said program listens for keypresses, and that other actual malicious programs listen for keypresses too, so it comes to the conclusion “oh, that’s dangerous”. And it’s not just that, game trainers/cheats will also manipulate process memory to change ingame variables (which in turn requires admin rights) etc. and this all accumulates together; basically, it’s too tasty for the AV to not flag it even though it's harmless.

### Releases & Changelogs
See [Releases](https://github.com/C0dingschmuser/SRU_Internal/releases) page for builds and decorated Changelogs. 

### License
Copyright 2022 C0dingschmuser - All rights reserved
