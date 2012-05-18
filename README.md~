# Deadlands


## Overview
A simple kernel-level rootkit.

Deadlands hides process.
It prevents taskmngr from viewing a running process (for example, cmd.exe).
Usercode from the project takes the PID of a process to hide, and communicates with our driver through ioctl.

## Guidelines
Kenelcode uses DKOM (Direct Kernel Object Manipulation) to hide underlying structures of a process. 
