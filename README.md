HID Emulator
=======
HID Emulator is a web IIDX controller for iPad that communicates through the DeviceIoControl WINAPI using the [Interception API library](https://github.com/oblitum/Interception) so that the input is captured by the HID Driver. The web server runs on python, while the HID controller runs on C++. Python sends key instructions to the C++ application using the Open Sound Control protocol.
The original i2DX repository available on [GitHub](https://github.com/dtinth/i2DX)

Install instructions
------------
* Install the interception driver using "install-interception.exe /install"
* Install [__Python__](https://www.python.org/downloads/) (version 2.7.9 used during development), [__AutoPy__](http://pypi.python.org/pypi/autopy/), [__setuptools__](pypi.python.org/pypi/setuptools)
* Install [__tornado__](https://pypi.python.org/pypi/tornado) and [__pyOSC__](https://trac.v2.nl/wiki/pyOSC) using the following commands:

    `C:\Python27\Scripts\easy_install tornado`
    
    `C:\Python27\Scripts\easy_install pyOSC`
* Compile the Interception library and the HIDEmulator project
* Run Launch.bat to run the original i2DX python server and the HIDEmulator.
