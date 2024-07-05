# car-nfc-reader
 Using a PN532 reader to unlock my car

Adafruit PN532 NFC Reader Connection

Adafruit PN532 Pin > Ethernet Cable Color > Ethernet Port Pin > ESP32 Pin
5v                 > ORANGE               > 2                 >  5v
GND                > BROWN                > 8                 >  GND
MOSI               > GREEN                > 6                 >  5
SS                 > WHITE/GREEN          > 3                 >  6
MISO               > BLUE                 > 4                 >  7
SCK                > WHITE/BLUE           > 5                 >  8

Above working as of 2024-07-03

For 2003 Corolla: Remote buttons are LOW when inactive and driving them HIGH will activate

For 2008 honda: Remote buttons are HIGH when inactive and driving them LOW will activate

2008 Honda Remote Pins:

White/Orange - 3v3 in - ETH 1
Brown        - GND    - ETH 8
Green        - Unlock - ETH 6
White/Green  - Lock   - ETH 3
Blue         - Trunk  - ETH 4