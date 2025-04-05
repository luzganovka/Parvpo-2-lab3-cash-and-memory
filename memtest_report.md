L1 Cache:   32KB    170 GB/s
L2 Cache:   512KB   69.1 GB/s
L3 Cache:   16MB    72 GB/s
Memory:     15.4GB  32.1 GB/s

// Trying to figure out the DRAM timings

# modprobe ee1004 
# decode-dimms 
No EEPROM found, the kernel probably does not support your hardware.

# i2cdetect -l | grep smbus
#
// nothing showed

# ls /sys/bus/i2c/devices/*/eeprom
ls: cannot access '/sys/bus/i2c/devices/*/eeprom': No such file or directory

# inxi -mxx
Memory:
  RAM: total: 15.03 GiB used: 5.98 GiB (39.8%)
  Array-1: capacity: 32 GiB slots: 2 EC: None max-module-size: 16 GiB
    note: est.
  Device-1: DIMM 0 type: LPDDR4 size: 8 GiB speed: 4266 MT/s volts: N/A
    manufacturer: Micron part-no: MT53E1G32D2NP-046
  Device-2: DIMM 0 type: LPDDR4 size: 8 GiB speed: 4266 MT/s volts: N/A
    manufacturer: Micron part-no: MT53E1G32D2NP-046

На некоторых ноутбуках SPD-чипы не подключены к системной шине I²C (или аппаратно защищены).
Особенно часто это с модулями LPDDR (Low Power DDR, как в ультрабуках), или припаянной RAM.
Также I²C может быть отключён в BIOS/UEFI.