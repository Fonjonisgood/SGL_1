# Program: RGB sensor Self directed learning
**Author(s):**  Milan Johnson
**Date:**1/24/25

**Modified by:**  
**Date:**

**Purpose:** When a button is pushed the RGB sensor takes a measuremnt and displays it and shows witch color is the greatest value.

## Configuration

Devicetree overlay:

Set up the ISL29125 I2C connection
Disaible the other channles 
Set up the pushbutton
Set up the Int pin at an led
Configure the 3 leds for brightest colors


Kernel configuration via `prj.conf`:
events 
I2C
CBPRINTF_FP_SUPPORT
LOG

## Hardware
### External
1 ISL29125 RGB sensor
1 Push button
3 leds + 3 resistors 
### Internal

## Flow

```mermaid
graph TB
 A[Define Push buttons, leds, Threads, l;ogger, eventmanager, ect] --> B[int main<>]
 B --> C[Configure call back funtions and button]
C --> D
 D[Button pressed<>] --> E[Post BTN evet when button pushed]
 E -- event --> F[RGBtoggle<>]
 F --> 1[INFINITE LOOP]
 1 --> G[wait for button event]
 G --> H[Post RGB event and toggel interupt pin]
 H --> I[sleep 50 ms]
 I --> J[Turn off interupt pin]
 J --> K[clear BTN Event and sleep for 1s]
 K --> 1

 H -- event --> L[talkRGB<>]
 L --> M[Set it to measure all colors, turn on sync mode set lux range]
 M --> N[Set IR protection]
 N --> O[Turn off brown off alert]
 O --> P[INFINITE LOOP]
 P --> Q[wait for RGB alert]
 Q --> R[Read all 6 data registers]
 R --> S[Shift the high bytes and or them]
 S --> T[Display The values]
 T --> U[Post color event and clear RGB event]
 U --> P
 V[brightcolor<>]
 V --> X[INFINITE LOOP]
 X --> Y[Wait for COLOR Event]
 U -- event --> Y
Y --> Z[Is red Bigger then green]
Z -- yes --> a[is red bigger than blue]
a -- yes --> c[Red is the biggest]
a -- no --> d[Blue is the biggest]
Z -- no --> b[is green bigger then blue ]
b -- yes --> e[Green is the biggest]
b -- no --> d
c --> f[display the largest and toggle that led for 1s]
d --> f
e --> f
f --> X




```