# mms-cpp

A Micromouse maze-solving algorithm in C++.
Basic Micromouse Solver for 16 x 16 square grids,
where the mouse starts from the lower left corner while facing northwards
and the target is any one of the four central squares.

Made by a-MAZE!:  [@sohamroy19](https://github.com/sohamroy19), 
                  [@Chinmaya6699](https://github.com/Chinmaya6699), 
                  [@Ishaan1742](https://github.com/Ishaan1742), 
                  [@Kishore-R-0x07](https://github.com/Kishore-R-0x07), 
                  [@wigglytuff-tu](https://github.com/wigglytuff-tu) 
for Shaastra 2021, IIT-M on 23-02-2021.

For use with [mackorone/mms](https://github.com/mackorone/mms), a Micromouse simulator.

## Setup

1. [Download the Micromouse simulator](https://github.com/mackorone/mms#download)
2. Run the simulator and click the "+" button to configure a new algorithm
3. Enter the config for your algorithm (name, directory, build command, and run command)
4. Click the "Run" button

## Examples

Windows:

![](https://github.com/mackorone/mms-cpp/blob/master/config-windows.png)

Linux (Ubuntu):

![](https://github.com/mackorone/mms-cpp/blob/master/config-linux.png)


## Notes

- If you're using Windows, you may need to download and install [MinGW](http://mingw.org/wiki/Getting_Started)
- Communication with the simulator is done via stdin/stdout, use stderr to print output
- Descriptions of all available API methods can be found at [mackorone/mms#mouse-api](https://github.com/mackorone/mms#mouse-api)
