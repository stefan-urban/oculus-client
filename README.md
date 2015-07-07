
Oculus Rift - eDVS Robot - Client Software
==========================================

#Requirements
Tested with Ubuntu 14.04 and Ubuntu 15.04 with Nouveau graphics drivers.<br>
Does not work with Intel graphic adapters!

Install the following packages from the repository:
- git
- libboost-system1.55-dev
- libboost-thread1.55-dev
- libglew-dev
- libopencv-dev
- qt5-qmake
- libglm-dev

All in one command:<br>
- `sudo apt install git libboost-system1.55-dev libboost-thread1.55-dev libopencv-dev libglew-dev  qt5-qmake libglm-dev`

Install the following packages by following the individual installation instructions:
- [OpenCTM](https://github.com/stefan-urban/OpenCTM.git)
- [GLFW v3](https://github.com/glfw/glfw.git) - Make sure to install it as a shared library!
- [OGLplus](https://github.com/matus-chochlik/oglplus.git)
- [Oculus SDK for Linux (Experimental) 0.5.0.1-beta](https://developer.oculus.com/)

# Building

1. clone this repository
2. `mkdir build && cd build`
3. `qmake ..`
4. `make`

# Starting

1. Start the Oculus Rift Service with `ovrd`<br>(It is possible to start this command as daemon.)
2. Run application with `./oculus-client`

<hr>

# Oculus Basis Application from "Oculus Rift In Action"
Extracted from https://github.com/OculusRiftInAction/OculusRiftInAction which is part of an upcoming book by Bradley Austin Davis, Karen Bryla, and Phillips Alexander Benton: [Oculus Rift in Action](http://www.manning.com/bdavis/)

Modifications:
 - Changed style to BSD
 - Removed many OS independence switches, now is Linux only
 - Moved app class from main.cpp to separate file
 - Eventually resources will be deleted


