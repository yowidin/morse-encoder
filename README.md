# Morse Encoder

A simple NodeMCU-based project, showcasing PlatformIO, OLED display, rotary encoder
and sound generator usage.

A silly video:

[![Hello World](https://img.youtube.com/vi/_DtdpNvrGD8/0.jpg)](https://youtu.be/_DtdpNvrGD8)

## Requirements
- A [NodeMCU V2](https://en.wikipedia.org/wiki/NodeMCU) Board
- [PlatformIO CLI](https://docs.platformio.org/en/latest//core/index.html) (or IDE)

## Building

Just run the `platformio` command, it will download and build all dependencies.

```shell
platformio run -e esp_debug
```

## Uploading

Connect the `NodeMCU` board via USB, and issue the following command:
```shell
platformio run -t upload -e esp_debug
```

> NOTE: make sure the board [drivers](https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers) are installed.