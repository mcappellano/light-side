## LIGHT SIDE - README updated: June 19, 2024


**1. To setup GitHub on VSCode**:
  - Install the VSCode extension available here: https://code.visualstudio.com/docs/sourcecontrol/github
  - Click on windows search and open up "terminal"
  - Navigate to the folder where you want to put the repository files on your computer. Right click on the folder and select "copy as path".
    In terminal, enter "cd" (change directory) followed by the path. I typed in:
    ``cd "C:\Users\jorda\OneDrive\Documents\Robot Summer"``.
    Then press enter.
  - To clone the repository, type ``git clone https://github.com/mcappellano/light-side.git``, then enter.
  - You can now double check that a folder called "light-side" exists in the specified folder. Inside it should include a ".git" folder, in addition to the other files.
  - Open VSCode and remove the previous folders you had in your workspace.
  - Select open folder, and navigate to the new "light-side" folder and open it.
  - You are all done. To commit, push, pull, and use all the other Git features, you can play around with it or check the same website as before: https://code.visualstudio.com/docs/sourcecontrol/github

**2. Before running code, install ESP32 Drivers:**
  - CP210x USB to UART Bridge VCP Drivers from silicon labs: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
  - Select: Downloads -> FILE: CP210x Windows Drivers
  - Make sure you dont install the universal driver
  - Install this file and run the exe

**3. Repository structure:**
  - The actual code we will be using is located in ``main`` (previously ``ESP32 Test2 Bidirectional``)
  - Each source (.cpp) file in ``src`` has a corresponding header (.h) file in ``lib``. The header file contains define statements (eg. ``#define SWITCH PA3``), variable definitions, and function definitions. Take a look at ``main.h`` and ``main.cpp`` as an example. At the top of source files we type ``#include "xxxx.h"``, and then we have access to all the functions and variables we made in ``xxxx.cpp``.
  - Any code that we have written that isn't currently being used, but don't want to lose goes in ``referencecode`` (previously ``Sample Code``)
  - Ignore ``esp32test1``
