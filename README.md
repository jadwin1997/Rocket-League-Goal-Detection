# Rocket League Goal-Reactive LED Controller

A real-time Rocket League LED controller that listens for in-game goal events and triggers addressable LED animations based on which team scored.

When a goal is detected, a Python script receives Rocket League match event data, parses the scorer/team information, and sends a JSON command over serial to an Arduino running FastLED. The LED strip then blinks blue or orange depending on the scoring team.

## Demo Behavior

- Blue team goal → LED strip blinks blue
- Orange team goal → LED strip blinks orange
- Default state → animated rainbow effect
- Commands are sent from Python to Arduino using JSON over serial

## Project Overview

This project connects three systems together:

1. **Rocket League Stats API / WebSocket**
   - Provides live game event data.
   - Python listens for events such as `GoalScored`.

2. **Python Event Handler**
   - Connects to the Rocket League event socket.
   - Parses JSON packets.
   - Detects goal events.
   - Determines the scoring team.
   - Sends a JSON LED command to the Arduino.

3. **Arduino LED Controller**
   - Receives JSON commands over serial.
   - Decodes commands using ArduinoJson.
   - Controls a WS2812 LED strip using FastLED.
   - Supports multiple lighting modes.

## Features

- Real-time Rocket League goal detection
- Team-based LED reactions
- JSON-based command protocol
- Addressable WS2812 LED control
- Multiple LED modes:
  - Rainbow
  - Static color
  - Blink
  - Off
- Easily expandable for more Rocket League events

## Hardware Used

- Arduino-compatible microcontroller
- WS2812 addressable LED strip
- USB serial connection between PC and Arduino
- PC running Rocket League and the Python listener

## Software Used

- Python
- PySerial
- ArduinoJson
- FastLED
- Rocket League event socket/API

## Python Dependencies

Install the required Python package:

```bash
pip install pyserial
