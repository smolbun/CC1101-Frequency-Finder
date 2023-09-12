# Get RF frequencies that are above the set threshold by hopping through a list of commonly used frequencies

[CC1101 Module](https://a.aliexpress.com/_mK4kt1M) 

[CC1101 Library by LSatan](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib)

## Commonly used frequencies
```cpp
const uint8_t freqCount = 5;
const float freq[freqCount] = { 310.0, 315.0, 318.0, 390.0, 433.92 };
```
## Example output

```
Started!
433.92 MHz RSSI: -36
433.92 MHz RSSI: -36
433.92 MHz RSSI: -37
433.92 MHz RSSI: -38
```
