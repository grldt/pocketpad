# PocketPad
*A small macropad that fits in your pocket!*

![PocketPad](.\src\PocketPad.jpg)


The PocketPad is a small macropad with 8+1 switches with an OLED screen, running QMK Firmware on RP2040 Zero microcontroller.

This was originally made as a gift and as a challenge for me to make something useful for someone commuting to different offices but in a smaller footprint, compared to the usual macropad with MX-style switches.

# Specifications

* Omron B3F-4055 micro switches
* 128x32 I2C OLED screen
* RP2040 Zero microcontroller
* Custom PCB designed by myself
* Acrylic sandwich case
* USB C connector

# Photos

Here's some images of the macropad, and my hand for scale.

| <img src=".\src\pcb.png" width="500">    | <img src=".\src\bot.jpg" width="500">    | <img src=".\src\top.jpg" width="500">    |
|------------------------------------------|------------------------------------------|------------------------------------------|
| <img src=".\src\photo1.jpg" width="500"> | <img src=".\src\photo2.jpg" width="500"> | <img src=".\src\photo3.jpg" width="500"> |

# VIA Configurator

PocketPad can be configurated with [VIA](https://usevia.app/) (unofficially) by sideloading the [`pocketpad.json`](.\keymaps\new\pocketpad8.json) file into the Design tab.

<img src=".\src\via.png" width="750">

# Features

PocketPad features a 128x32 OLED screen on the top left. It's usually for showing status like caps lock or which OS is the host, but I tried making something different for it.

## Menu Interface

First of all, I made a menu interface so the user can access a lot of the functions of the macropad easily.

<img src=".\src\menu.gif" width="500">

The user can navigate the menu with the buttons on an inverted T layout, as pictured below:

<img src=".\src\menu.png" width="400">

Where left is back/cancel, and right is okay/enter.

You can find the code in [`menu.c`](.\lib\menu.c).

## Change layer

This menu is for changing between layers on the macropad's keymap. On keyboards/macropads running QMK Firmware, users can add layers to get more function on every button. For example, on a 60% keyboard you can use `Fn + 1` to input the key `F1` since there's no function row on a 60% keyboard. You can view and configure the layers through VIA, and change between layers from this menu.

<img src=".\src\changelayer.gif" width="500">

Here's some example of what the user can put on different layers on the macropad:

| <img src=".\src\layer0.png" width="300"> | <img src=".\src\layer1.png" width="300"> |
|------------------------------------------|------------------------------------------|
| <img src=".\src\layer2.png" width="300"> | <img src=".\src\layer3.png" width="300"> |

You can find the code in [`menu.c`](.\lib\menu.c).

## Apps
This is where the user can open apps that's on the macropad. I made three apps on it: Timer, Dice Roll, and Wishing Simulator.

### Timer
Just a normal timer, you can pause and resume the timer. I haven't implemented a reset for the timer, but this is just a test for running a timer/clock on QMK.

<img src=".\src\timer.gif" width="500">

You can find the code in [`timer.c`](.\lib\timer.c).

### Dice Roll
This is also a test for running a timer on QMK, and added a random number generator from 1 to 6. Also drew a dice to put next to it to try pixel art and showing pictures on the OLED screen.

<img src=".\src\diceroll.gif" width="500">

You can find the code in [`dice.c`](.\lib\dice.c).

### Wishing Simulator
This was quite fun to do. I made a wishing/gacha simulator on the macropad. This was heavily inspired by Genshin Impact and other gacha games by Hoyoverse, where gacha is one of the main mechanics of their games. It's basically "wishing" (or pulling in general) for a character or weapon in the game, usually it's a limited time event, and have to use a special in-game currency that you can (but usually have to) buy with real money to do a wish. I made this so the user can play it without opening the game (and probably spend money).

I referenced the original drop rates/probabilities of the characters/weapons with different rarities, but changed some numbers to make it easier for the user to get higher rarity characters/weapons with fewer pulls. It's a simulator anyway, not the real thing.

<img src=".\src\wish.gif" width="500">

You can find the code in [`wish.c`](.\lib\wish.c).

## Change Screen
Here you can change what's shown on the OLED screen when you're using it outside the menu interface. I made a Stats screen as a default, and implemented Bongo Cat animation as another option.

### Stats
This is the default screen that the user sees when the macropad is connected. It shows which layer it's on, the device's status for caps lock, num lock, and scroll lock, also showing a bar below the WPM counter. The faster the user types on the macropad, the higher the WPM counter is, and the longer the bar showing under the counter. This is similar to the Bongo Cat animation which uses the same WPM counter under the code.

<img src=".\src\stats.gif" width="500">

You can find the code in [`stats.c`](.\lib\stats.c).

### Bongo Cat
Originally from [github.com/nwii/oledbongocat](https://github.com/nwii/oledbongocat/) which has been implemented on a few keyboards on the QMK Firmware repository. The cat's paws moves as the WPM counter on the macropad goes up when the user is pressing the macropad keys quickly.

<img src=".\src\bongo.gif" width="500">

You can find the code in [`bongo.c`](.\lib\bongo.c).

## About Board
This is where I put the board's name, the firmware/build version, and a link where the user can download updates for the macropad.

<img src=".\src\about.gif" width="500">

You can find the code in [`menu.c`](.\lib\menu.c).

## Bootloader Mode
And last but not least, this is where the user can install the updates mentioned above on the macropad. While the macropad is connected to a host computer, bootloader mode turns the macropad into a storage device and the user can just drag and drop the update file on the computer onto the macropad, the macropad will reboot itself and it's ready to be used again.

<img src=".\src\bootloader.gif" width="500">

You can find the code in [`menu.c`](.\lib\menu.c).

---

It's been fun and challenging at the same time while making this. From designing, to building, then writing the firmware and features for it. I set myself a challenge to make this as a useful gift, not just as decoration on a desk. That's all about the macropad, feel free to DM me a question or suggestion on my [Instagram](https://www.instagram.com/grldtmk).
