# LCC_TELNET
A software component which can be used as part of an [OpenLCB/LCC](https://openlcb.org/) node. It has been developed using PlatformIO for an Arduino Nano ESP32. The LCC node is designed to connect over WiFi to the LCC hub provided by JMRI.

## Purpose

It has been developed to allow a telnet server to be hosted by the node. When a connection is made to this telnet server various messages can be sent from the node to be displayed on the telnet client.

## Dependencies

- [ESPTelnet](https://github.com/LennartHennigs/ESPTelnet.git)

## PlatformIO installation

It can be installed in PlatformIO by adding the following line to platform.ini;-

lib_deps = https://github.com/JohnCallingham/LCC_TELNET.git
