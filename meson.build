project('playgrounduino', 'c', 'cpp',
  version: '1',
)

# TODO
# Move these to a "cross board file", and the hwsrc location should
# be a Meson option and/or autodetected from a few well-known paths.
#
hwsrc = '/usr/share/arduino/hardware/archlinux-arduino/avr'
mcu = 'atmega328p'
mhz = 16
core = 'arduino'
variant = 'standard'

add_project_arguments([
  '-mmcu=@0@'.format(mcu),
  '-DF_CPU=@0@000000L'.format(mhz),
], language: ['c', 'cpp'])

tc_objcopy = find_program('objcopy', required: true)

##
## libArduino
##
libArduino_dir = hwsrc / 'cores' / core
libArduino_files = [
    'abi.cpp',
    'CDC.cpp',
    'HardwareSerial.cpp',
    'HardwareSerial0.cpp',
    'HardwareSerial1.cpp',
    'HardwareSerial2.cpp',
    'HardwareSerial3.cpp',
    'hooks.c',
    'IPAddress.cpp',
    'main.cpp',
    'new.cpp',
    'PluggableUSB.cpp',
    'Print.cpp',
    'Stream.cpp',
    'Tone.cpp',
    'USBCore.cpp',
    'WInterrupts.c',
    'wiring_analog.c',
    'wiring.c',
    'wiring_digital.c',
    'wiring_pulse.c',
    'wiring_pulse.S',
    'wiring_shift.c',
    'WMath.cpp',
    'WString.cpp',
  ]
libArduino_src = []
foreach item : libArduino_files
  libArduino_src += [libArduino_dir / item]
endforeach
libArduino_inc = include_directories(
  libArduino_dir,
  hwsrc / 'variants' / variant,
)
libArduino_lib = static_library('Arduino',
  libArduino_src,
  include_directories: libArduino_inc,
  install: false,
)
libArduino_dep = declare_dependency(
  link_with: libArduino_lib,
  include_directories: libArduino_inc,
)

##
## libWire
##
libWire_dir = hwsrc / 'libraries' / 'Wire' / 'src'
libWire_lib = static_library('Wire',
  libWire_dir / 'Wire.cpp',
  libWire_dir / 'utility' / 'twi.c',
  dependencies: libArduino_dep,
  install: false,
)
libWire_dep = declare_dependency(
  link_with: libWire_lib,
  include_directories: include_directories(libWire_dir),
  dependencies: libArduino_dep,
)

##
## libEEPROM
##
libEEPROM_dir = hwsrc / 'libraries' / 'EEPROM' / 'src'
libEEPROM_dep = declare_dependency(
  include_directories: include_directories(libEEPROM_dir),
  dependencies: libArduino_dep,
)

##
## libSPI
##
libSPI_dir = hwsrc / 'libraries' / 'SPI' / 'src'
libSPI_lib = static_library('SPI',
  libSPI_dir / 'SPI.cpp',
  dependencies: libArduino_dep,
  install: false,
)
libSPI_dep = declare_dependency(
  link_with: libSPI_lib,
  dependencies: libArduino_dep,
  include_directories: include_directories(libSPI_dir),
)

##
## libBusIO
##
libBusIO_dir = 'thirdparty' / 'BusIO'
libBusIO_lib = static_library('BusIO',
  libBusIO_dir / 'Adafruit_BusIO_Register.cpp',
  libBusIO_dir / 'Adafruit_I2CDevice.cpp',
  libBusIO_dir / 'Adafruit_SPIDevice.cpp',
  include_directories: include_directories(libBusIO_dir),
  dependencies: [libSPI_dep, libWire_dep],
  install: false,
)
libBusIO_dep = declare_dependency(
  link_with: libBusIO_lib,
  dependencies: libWire_dep,
  include_directories: include_directories(libBusIO_dir),
)

##
## libRTC
##
libRTC_dir = 'thirdparty' / 'RTClib' / 'src'
libRTC_lib = static_library('RTC',
  libRTC_dir / 'RTC_DS1307.cpp',
  libRTC_dir / 'RTC_DS3231.cpp',
  libRTC_dir / 'RTClib.cpp',
  libRTC_dir / 'RTC_Micros.cpp',
  libRTC_dir / 'RTC_Millis.cpp',
  libRTC_dir / 'RTC_PCF8523.cpp',
  libRTC_dir / 'RTC_PCF8563.cpp',
  dependencies: libBusIO_dep,
  install: false,
)
libRTC_dep = declare_dependency(
  link_with: libRTC_lib,
  include_directories: include_directories(libRTC_dir),
  dependencies: libBusIO_dep,
)

all_exe = []

##
## eeprom-clear
##
all_exe += [
  executable('eeprom-clear',
    'eeprom' / 'clear.cc',
    dependencies: libEEPROM_dep,
    install: false,
  ),
]

##
## i2c
##
all_exe += [
  executable('i2c-scan',
    'i2c' / 'scan.cc',
    dependencies: libWire_dep,
    install: false,
  ),
  executable('i2c-rtc',
    'i2c' / 'rtc.cc',
    dependencies: libRTC_dep,
    install: false,
  ),
]

##
## blinky
##
foreach item : ['blinky', 'knight', 'pwmfade', 'rgbfade']
  all_exe += [
    executable(item,
      'blinky' / '@0@.cc'.format(item),
      dependencies: libArduino_dep, install: false,
    )
  ]
endforeach

##
## s7777
##
all_exe += [
  executable('s7777-test',
    's7777' / 'test.cc',
    dependencies: libArduino_dep,
    install: false,
  ),
  executable('s7777-clock',
    's7777' / 'clock.cc',
    dependencies: libRTC_dep,
    install: false,
  ),
]

##
## Produce .hex + .eep outputs
##
foreach item : all_exe
  custom_target('@0@.hex'.format(item.name()),
    output: '@0@.hex'.format(item.name()),
    input: item,
    command: [tc_objcopy, '-O', 'ihex', '-R', '.eeprom=0', '@INPUT@', '@OUTPUT@'],
    build_by_default: true,
    install: false,
  )
  custom_target('@0@.eep'.format(item.name()),
    output: '@0@.eep'.format(item.name()),
    input: item,
    command: [tc_objcopy, '-O', 'ihex', '-j', '.eeprom',
      '--set-section-flags=.eeprom=alloc,load',
      '--no-change-warnings', '--change-section-lma', '.eeprom=0',
      '@INPUT@', '@OUTPUT@'
    ],
    build_by_default: true,
    install: false,
  )
endforeach
