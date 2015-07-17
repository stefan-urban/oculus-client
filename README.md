
Oculus Rift - eDVS Robot - Client Software
==========================================

#Requirements
Tested with Ubuntu 14.04 and Ubuntu 15.04 with Nouveau graphics drivers.<br>
Does not work with Intel graphic adapters!

Install the following packages from the repository:
- git
- libboost-system-dev
- libboost-thread-dev
- libglew-dev
- libopencv-dev
- qt5-qmake
- libglm-dev

All in one command:<br>
- `sudo apt install git libboost-system-dev libboost-thread-dev libopencv-dev libglew-dev  qt5-qmake libglm-dev`

Install the following packages by following the individual installation instructions:
- [OpenCTM](https://github.com/stefan-urban/OpenCTM.git)
- [GLFW v3](https://github.com/glfw/glfw.git) - Make sure to install it as a shared library!<br>`cmake -DBUILD_SHARED_LIBS=ON ..`
- [OGLplus](https://github.com/matus-chochlik/oglplus.git)
- [Oculus SDK for Linux (Experimental) 0.5.0.1-beta](https://developer.oculus.com/)

# Building

1. clone this repository and make sure to get the submodules<br>`git clone https://github.com/stefan-urban/oculus-client.git && cd oculus-client`<br>`git submodule update --init`
2. `mkdir build && cd build`
3. `qmake ..`
4. `make`

# Starting

1. Start the Oculus Rift Service with `ovrd`<br>(It is possible to start this command as daemon.)
2. Run application with `./oculus-client <server-ip> <port>`

<hr>

#License

Copyright (C) 2015  Stefan Urban

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.<br>
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.<br>
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

The OVR framework was released under a different license, see below.

<hr>

# Oculus Basis Application from "Oculus Rift In Action"
Extracted from https://github.com/OculusRiftInAction/OculusRiftInAction which is part of an upcoming book by Bradley Austin Davis, Karen Bryla, and Phillips Alexander Benton: [Oculus Rift in Action](http://www.manning.com/bdavis/)

Modifications:
 - Changed coding style to BSD
 - Removed many OS switches, now is Linux only
 - Added error handling for OpenGL

The Apache License, Version 2.0 file is included in `LICENSE.Apache`.
