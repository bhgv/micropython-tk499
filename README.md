port for tk499 with 5" screen with RGB interface and ft54x6 capasitive touchscreen

How to build:
=============
```
cd mpy-cross
make

# for RGB-display 5" model
cd ../ports/tk499-5i-rgb

# OR
# for MCU-888 (TK88) -display model
cd ../ports/tk499-4.3i-mcu-lcd

make submodules
make
```

ROM to upload:
--------------
```
ports/tk499-5i-rgb/build-TKM32/firmware.bin
```

How to upload:
-------------
connect your tk499 board with USB cable to your computer, press buttons "BOOT" and "RESET" on the board. release "RESET" button. AFTER, release "BOOT" button.

on your computer should appear new USB disk "TK499". just copy your compiled ROM there. after flashing this disk  will disappear.

connect with your prefered terminal to /dev/ttyACM0 (or 1, 2, 3.. if you have many ttyACM devices), 115200, 8, 1

now you have micropython console.

or you may use `Thonny` python editor with support of remote micropython

Quick start (translation from chinese):
=======================================


Delay and time
-----------
Use the timemodule:
```
import time

time.sleep(1)           # sleep for 1 second
time.sleep_ms(500)      # Sleep for 500 milliseconds
time.sleep_us(10)       # Sleep 10 subtle
start = time.ticks_ms() # Get the millisecond timer start value
delta = time.ticks_diff(time.ticks_ms(), start) # Calculate the difference from the start to the current time
```

Timer
-----
TKM32F499 has 10 hardware timers. Use the machine.Timer class by setting the timer ID number to 1-10
```
from machine import Timer

tim0 = Timer(1)
tim0.init(period=5000, mode=Timer.ONE_SHOT, callback=lambda t:print(0))

tim1 = Timer(2)
tim1.init(period=2000, mode=Timer.PERIODIC, callback=lambda t:print(1))
```
The unit of this period is milliseconds (ms), and virtual timers are temporarily not supported: id=-1.

Pins and GPIO ports
-------------------
Use machine.Pin module:
```
from machine import Pin

p0 = Pin('C4', Pin.OUT)             # Create object p0, corresponding to C4 output
p0.on()                             # Set the pin to "on" (1) high level
p0.off()                            # Set the pin to "off" (0) low level
p0.value(1)                         # Set the pin to "on" (1) high level

p2 = Pin('A0', Pin.IN)              # Create object p2, corresponding to A0 port input
print(p2.value())                   # Get the pin input value, 0 (low level) or 1 (high level)

p4 = Pin('C6', Pin.IN, Pin.PULL_UP) # Turn on the internal pull-up resistor
p5 = Pin('C7', Pin.OUT, value=1)    # Set the pin value to 1 (high level) during initialization
```
TKM32F499 pins use the same naming method as the chip pins, such as PA0–>'A0', PC4–>'C4'. Please refer to its schematic diagram for details.
ADC (Analog to Digital Conversion)

TKM32F499 has a total of 6 ADCs, the corresponding pins are'ADC_4','ADC_5','B0','B1','B2','B3'. Please note that the input voltage on the ADC pin must be between 0.0v Between and 3.3v (corresponding value is 0-4095).

Use the machine.ADC class:
---------------------------
```
from machine import ADC

adc = ADC('ADC_4') # Create ADC object on ADC_4 pin
adc.read_16()      # Read the measured value, 0-4095 means the voltage is from 0.0v-3.3v
```

Software SPI bus
----------------
There are two SPI drivers inside EPS32. One of them is realized by software (bit-banging), and allows configuration to all pins, configured by machine.SoftSPI type module:
```
from machine import Pin, SoftSPI

# Create a SoftSPI bus on the given pin
# (Polarity) polarity refers to the state of SCK when it is idle
# (Phase) phase=0 means SCK starts sampling at the first edge, phase=1 means it starts at the second edge.
spi = SoftSPI(baudrate=100000, polarity=1, phase=0, sck=Pin(0), mosi=Pin(2), miso=Pin(4))

spi.init(baudrate=200000)        # set frequency

spi.read(10)                     # read 10 bytes of data on the MISO pin
spi.read(10, 0xff)               # read 10 bytes of data on the MISO pin and output 0xff on the MOSI

buf = bytearray(50)              # Create a buffer
spi.readinto(buf)                # read the data and store it in the buffer (here 50 bytes are read)
spi.readinto(buf, 0xff)          # Read the data and store it in the buffer, while outputting 0xff in MOSI

spi.write(b'12345')              # Write 5 bytes of data on the MOSI pin

buf = bytearray(4)               # Create buffer
spi.write_readinto(b'1234', buf) # Write data on the MOSI pin and store the MISO read data in the buffer
spi.write_readinto(buf, buf)     # Write the data in the buffer on the MOSI pin and store the MISO read data in the buffe
```

*Warning*

Currently when creating software SPI object sck, mosiand miso all the pins must be defined.

Hardware SPI bus
----------------
There are two hardware SPI channels allowing higher rate transfers. It can also be configured as any pin, but the relevant pins must conform to the directionality of input and output, which can be referred to (see pins and GPIO ports ). Using custom pins instead of default pins will reduce the transmission speed, and the upper limit is 40MHz. The following are the default pins of the hardware SPI bus:

|      | HSPI (id=1) | VSPI (id=2) |
|------|-------------|-------------|
| sck  | B2          | A5          |
| mosi | B0          | A7          |
| miso | B1          | A6          |


Hardware SPI is accessed via the machine.SPI class and has the same methods as software SPI above:
```
from machine import Pin, SPI

hspi = SPI(1, 10000000)
vspi = SPI(2, baudrate=80000000, polarity=0, phase=0, bits=8, firstbit=0)
```

SoftI2C bus
-----------
The I2C bus is divided into software and hardware objects. The hardware can define 0 and 1, and the function can be changed on any pin through configuration. For details, please see the machine.SoftI2C module:
```
from machine import Pin, SoftI2C

# Build 1 I2C object
i2c = SoftI2C(scl=Pin(5), sda=Pin(4), freq=100000)

# Build a hardware I2C bus
i2c = I2C(0)
i2c = I2C(1, scl=Pin(5), sda=Pin(4), freq=400000)

i2c.scan()              # Scan the slave device

i2c.readfrom(0x3a, 4)   # read 4 bytes of data from the slave device with address 0x3a
i2c.writeto(0x3a, '12') # write data "12" to the slave device with address 0x3a

buf = bytearray(10)     # Create a 10-byte buffer
i2c.writeto(0x3a, buf)  # Write buffer data to slave 
```

Hardware I2C bus
----------------
There are two hardware I2C peripherals with identifiers 0 and 1. Any available output-capable pins can be used for SCL and SDA but the defaults are given below.

|       | I2C(1) | I2C(3) |
|-------|--------|--------|
| scl   | B2     | C1     |
| sda   | B0     | C0     |

The driver is accessed via the machine.I2C class and has the same methods as software I2C above:
```
from machine import Pin, I2C

i2c = I2C(0)
```

Real Time Clock (RTC)
---------------------
(See machine.RTC)
```
from machine import RTC

rtc = RTC()
rtc.datetime((2017, 8, 23, 1, 12, 48, 0, 0)) # Set time (year, month, day, week, hour, minute, second, microsecond)
                                             # Where the week uses 0-6 to indicate Monday to Sunday.
rtc.datetime()                               # Get the current date and time 
```

DHT driver
----------
DHT temperature and humidity drive allows to realize on each pin through software:
```
import dht
import machine

d = dht.DHT11(machine.Pin(4))
d.measure()
d.temperature() # eg. 23 (°C)
d.humidity()    # eg. 41 (% RH)

d = dht.DHT22(machine.Pin(4))
d.measure()
d.temperature() # eg. 23.6 (°C)
d.humidity()    # eg. 41.3 (% RH)
```

Network
-------
(For details, please refer to network.ESP8266)
```
import network
from machine import UART

uart = UART(2, 115200)
wlan = network.ESP8266(uart)      # Create station interface
wlan.connect('essid', 'password') # connect to the specified WiFi network
wlan.ifconfig()                   # Get the IP/netmask (subnet mask)/gw (gateway)/DNS address of the interface 
```

Once the network is established, you can socketcreate a module and use TCP / UDP sockets communications, as well as by urequestssending an HTTP request module easily.

4.3" (or 5") RGB display
------------------------
(Please refer to tftlcd.LCD43R and touch.FT5436)

LCD routine:
```
import tftlcd

d = tftlcd.LCD43R(portrait=1)                  #Build LCD object
d.fill((255, 255, 255))                        #fill white
d.drawRect(0, 0, 100, 100, (255,0,0))          #Draw a red rectangle
d.printStr('Hello 01Studio!', 0, 0, (0,255,0)) #write characters
d.Picture(0, 0,'/flash/curry.jpg')             #display picture 
```

Touch routine:
```
import touch

t = touch.FT5436(portrait=1) #Build a touch screen object
t.read()                     #Get touch status and coordinates 
```

GUI-touch button
----------------
The TouchButton class provides a touch button control interface. By constructing this object, you can easily display touch button applications. For details, please refer to gui.TouchButton.

Example:
```
import gui

B1 = gui.TouchButton(0,0,120,80,(255,0,0),'LED3',(255,255,255),fun1) #Build a button
print(B1.ID())                                                       #Print button number 
```




[![CI badge](https://github.com/micropython/micropython/workflows/unix%20port/badge.svg)](https://github.com/micropython/micropython/actions?query=branch%3Amaster+event%3Apush) [![Coverage badge](https://coveralls.io/repos/micropython/micropython/badge.png?branch=master)](https://coveralls.io/r/micropython/micropython?branch=master)

The MicroPython project
=======================
<p align="center">
  <img src="https://raw.githubusercontent.com/micropython/micropython/master/logo/upython-with-micro.jpg" alt="MicroPython Logo"/>
</p>

This is the MicroPython project, which aims to put an implementation
of Python 3.x on microcontrollers and small embedded systems.
You can find the official website at [micropython.org](http://www.micropython.org).

WARNING: this project is in beta stage and is subject to changes of the
code-base, including project-wide name changes and API changes.

MicroPython implements the entire Python 3.4 syntax (including exceptions,
`with`, `yield from`, etc., and additionally `async`/`await` keywords from
Python 3.5). The following core datatypes are provided: `str` (including
basic Unicode support), `bytes`, `bytearray`, `tuple`, `list`, `dict`, `set`,
`frozenset`, `array.array`, `collections.namedtuple`, classes and instances.
Builtin modules include `sys`, `time`, and `struct`, etc. Select ports have
support for `_thread` module (multithreading). Note that only a subset of
Python 3 functionality is implemented for the data types and modules.

MicroPython can execute scripts in textual source form or from precompiled
bytecode, in both cases either from an on-device filesystem or "frozen" into
the MicroPython executable.

See the repository http://github.com/micropython/pyboard for the MicroPython
board (PyBoard), the officially supported reference electronic circuit board.

Major components in this repository:
- py/ -- the core Python implementation, including compiler, runtime, and
  core library.
- mpy-cross/ -- the MicroPython cross-compiler which is used to turn scripts
  into precompiled bytecode.
- ports/unix/ -- a version of MicroPython that runs on Unix.
- ports/stm32/ -- a version of MicroPython that runs on the PyBoard and similar
  STM32 boards (using ST's Cube HAL drivers).
- ports/minimal/ -- a minimal MicroPython port. Start with this if you want
  to port MicroPython to another microcontroller.
- tests/ -- test framework and test scripts.
- docs/ -- user documentation in Sphinx reStructuredText format. Rendered
  HTML documentation is available at http://docs.micropython.org.

Additional components:
- ports/bare-arm/ -- a bare minimum version of MicroPython for ARM MCUs. Used
  mostly to control code size.
- ports/teensy/ -- a version of MicroPython that runs on the Teensy 3.1
  (preliminary but functional).
- ports/pic16bit/ -- a version of MicroPython for 16-bit PIC microcontrollers.
- ports/cc3200/ -- a version of MicroPython that runs on the CC3200 from TI.
- ports/esp8266/ -- a version of MicroPython that runs on Espressif's ESP8266 SoC.
- ports/esp32/ -- a version of MicroPython that runs on Espressif's ESP32 SoC.
- ports/nrf/ -- a version of MicroPython that runs on Nordic's nRF51 and nRF52 MCUs.
- extmod/ -- additional (non-core) modules implemented in C.
- tools/ -- various tools, including the pyboard.py module.
- examples/ -- a few example Python scripts.

The subdirectories above may include READMEs with additional info.

"make" is used to build the components, or "gmake" on BSD-based systems.
You will also need bash, gcc, and Python 3.3+ available as the command `python3`
(if your system only has Python 2.7 then invoke make with the additional option
`PYTHON=python2`).

The MicroPython cross-compiler, mpy-cross
-----------------------------------------

Most ports require the MicroPython cross-compiler to be built first.  This
program, called mpy-cross, is used to pre-compile Python scripts to .mpy
files which can then be included (frozen) into the firmware/executable for
a port.  To build mpy-cross use:

    $ cd mpy-cross
    $ make

The Unix version
----------------

The "unix" port requires a standard Unix environment with gcc and GNU make.
x86 and x64 architectures are supported (i.e. x86 32- and 64-bit), as well
as ARM and MIPS. Making full-featured port to another architecture requires
writing some assembly code for the exception handling and garbage collection.
Alternatively, fallback implementation based on setjmp/longjmp can be used.

To build (see section below for required dependencies):

    $ cd ports/unix
    $ make submodules
    $ make

Then to give it a try:

    $ ./micropython
    >>> list(5 * x + y for x in range(10) for y in [4, 2, 1])

Use `CTRL-D` (i.e. EOF) to exit the shell.
Learn about command-line options (in particular, how to increase heap size
which may be needed for larger applications):

    $ ./micropython -h

Run complete testsuite:

    $ make test

Unix version comes with a builtin package manager called upip, e.g.:

    $ ./micropython -m upip install micropython-pystone
    $ ./micropython -m pystone

Browse available modules on
[PyPI](https://pypi.python.org/pypi?%3Aaction=search&term=micropython).
Standard library modules come from
[micropython-lib](https://github.com/micropython/micropython-lib) project.

External dependencies
---------------------

Building MicroPython ports may require some dependencies installed.

For Unix port, `libffi` library and `pkg-config` tool are required. On
Debian/Ubuntu/Mint derivative Linux distros, install `build-essential`
(includes toolchain and make), `libffi-dev`, and `pkg-config` packages.

Other dependencies can be built together with MicroPython. This may
be required to enable extra features or capabilities, and in recent
versions of MicroPython, these may be enabled by default. To build
these additional dependencies, in the port directory you're
interested in (e.g. `ports/unix/`) first execute:

    $ make submodules

This will fetch all the relevant git submodules (sub repositories) that
the port needs.  Use the same command to get the latest versions of
submodules as they are updated from time to time. After that execute:

    $ make deplibs

This will build all available dependencies (regardless whether they
are used or not). If you intend to build MicroPython with additional
options (like cross-compiling), the same set of options should be passed
to `make deplibs`. To actually enable/disable use of dependencies, edit
`ports/unix/mpconfigport.mk` file, which has inline descriptions of the options.
For example, to build SSL module (required for `upip` tool described above,
and so enabled by default), `MICROPY_PY_USSL` should be set to 1.

For some ports, building required dependences is transparent, and happens
automatically.  But they still need to be fetched with the `make submodules`
command.

The STM32 version
-----------------

The "stm32" port requires an ARM compiler, arm-none-eabi-gcc, and associated
bin-utils.  For those using Arch Linux, you need arm-none-eabi-binutils,
arm-none-eabi-gcc and arm-none-eabi-newlib packages.  Otherwise, try here:
https://launchpad.net/gcc-arm-embedded

To build:

    $ cd ports/stm32
    $ make submodules
    $ make

You then need to get your board into DFU mode.  On the pyboard, connect the
3V3 pin to the P1/DFU pin with a wire (on PYBv1.0 they are next to each other
on the bottom left of the board, second row from the bottom).

Then to flash the code via USB DFU to your device:

    $ make deploy

This will use the included `tools/pydfu.py` script.  If flashing the firmware
does not work it may be because you don't have the correct permissions, and
need to use `sudo make deploy`.
See the README.md file in the ports/stm32/ directory for further details.

Contributing
------------

MicroPython is an open-source project and welcomes contributions. To be
productive, please be sure to follow the
[Contributors' Guidelines](https://github.com/micropython/micropython/wiki/ContributorGuidelines)
and the [Code Conventions](https://github.com/micropython/micropython/blob/master/CODECONVENTIONS.md).
Note that MicroPython is licenced under the MIT license, and all contributions
should follow this license.
