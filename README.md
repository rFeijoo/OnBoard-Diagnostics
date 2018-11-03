# InTrack
On-board diagnostics tool, used to request data from vehicles, using the ELM327 OBD-II connected to an Arduino Uno by HC-05 Bluetooth Shield.

So far, this project is only auto-connecting to OBD-II, as well as keep the connection alive - remember to set the right BT-address from your device. Also, after having it all set right, you can type AT Comands on the Arduino's Serial Console to get others PIDs.

# References & Quick Links
- https://sites.google.com/site/grcbyte/electronica/arduino/obdii-bluetooth
- https://en.wikipedia.org/wiki/OBD-II_PIDs
- https://www.obdsol.com/knowledgebase/obd-software-development/reading-real-time-data/
