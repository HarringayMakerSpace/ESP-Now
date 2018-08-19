# ESP-Now

### Update: *** As of 19 Aug 2018 the latest ESP8266/Arduino code has fixed [Issue 3408](https://github.com/esp8266/Arduino/issues/3408) and added a deepSleepInstant function. This means an ESP-Now sensor can do the wakeup-send-sleep cycle in around 40 milliseconds, about an order of magnitude faster than previously. I've added a minimal example demonstrating this in [espnow-sensor-minimal](https://github.com/HarringayMakerSpace/ESP-Now/blob/master/espnow-sensor-minimal/espnow-sensor-minimal.ino) ***

ESP-Now is an interesting ESP8266 protocol that could be used for battery powered sensors. Its built on top of 802.11 vendor-specific action frames which enables sending data without having to first establish a WiFi AP to Station connection which is time consuming and so adds significantly to draining the batteries on each sensor wakeup.  

I've given some examples of it before, [here](https://github.com/HarringayMakerSpace/IoT/tree/master/ESP-Now), however ESP-Now doesn't work with Wifi so a gateway receiving ESP-Now transmissions can't also connect to a Wifi network, so thats made it pretty impractical for most uses.

I've been playing about with ESP-Now again recently and come up with two workarounds for the Wifi co-existence problem. One is to restart the ESP8266 after receiving each ESP-Now transmission, the other is to use two ESP8266's - one as the ESP-Now receiver and the other as the Wifi gateway.

The first approach of restarting the ESP8266 might seem a bit clunky but it actually works ok. The restart takes a few seconds (most of the time spent reconnecting WiFi), so if there aren't too many remote ESP-Now sensors or the sensor transmissions aren't too frequent then the Gateway being regularly unavailable for a few seconds doesn't actually matter.

The second approach with using two ESP8266s is more complicated to set up but also more robust and allows higher throughput. I've used a simple serial link with the ESP-Now receiver simply writing the received data to the Serial port which another ESP8266 can read using SoftwareSerial. You could also try writing a more fancy I2C or SPI driver too. 

These are some ESP-Now devices using this approach. A battery powered temperature/humidity/pressure sensor built into a LED light case and an ESP-Now to Wifi Gateway using the DIY USB powered ESP (described [here](https://github.com/HarringayMakerSpace/usb-esp)). 

Sample code for these are in this repository.

![Alt text](/doc/devices.jpg?raw=true "ESP-Now sensor and Wifi Gateway")

You can see the data from that ESP-Now based BME280 sensor thats in my back garden in London [here](http://s3.eu-west-2.amazonaws.com/torntrousers/espnow-test1.html).
