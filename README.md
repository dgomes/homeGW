# Home GW

A simple and trivial to reproduce HomeGW for cheap 433MHz based weather stations and sensors.

## Introduction

The objective of this library is to decode information coming from weather station units which can be found online very cheap.

Furthermore, this library and the example code [hass_serial](https://github.com/dgomes/homeGW/tree/master/examples/hass_serial) can be used with [Home Assistant](http://home-assistant.io) through the use of a [custom component](https://github.com/dgomes/home-assistant-custom-components)

## Bill of Materials (BOM)

- Arduino (any kind of Arduino will do, including esp8266)
- RF 433MHz Receiver (I recommend a superheterodyne)
- Breadboard + wires (optionally a perfboard and soldering iron)
- Antenna (17.4 cm of straight wire)

## Schematics

![Alt text](docs/HomeGW.png?raw=true "Schematic")


## Plugins

### weather

Decodes the "Prologue Temperature Sensor" protocol.

Works with a unbranded weather station sold on eBay.

### digoo

Decodes the "Nexus Temperature & Humidity Sensor" protocol.

Works with **Digoo DG-R8H** 433MHz Wireless Digital Hygrometer Thermometer Weather Station Outdoor Sensor and **Sencor SWS 21TS**. You can find it online in several stores.

### generic

This plugin should work with a broad number of OOK based devices such as power plugs and remotes

### fanju 

Chinese Weather Station Outdoor Sensor. Supports all station within FANJU brand, ex. PT-3378, FJ3378, FJ3391, FJ3389, FJV4

## Examples

### dump.ino

This example prints a dump of the raw packet it receives/identifies for each plugin.

### hass_serial.ino

This example outputs a JSON object for each packet it decodes.

The example can easily be integrated in a Home Assistant install using the following configuration:

```yaml
- platform: serial
  serial_port: /dev/ttyUSB0
  baudrate: 115200

- platform: template
  sensors:
    weatherstation_temperature:
      friendly_name: "Temperature"
      value_template:  >-
        {% if is_state_attr('sensor.serial_sensor', 'dev' ,'weather') %}
        {{ states.sensor.serial_sensor.attributes.in_temp }}
        {% else %}
        {{ states.sensor.weatherstation_temperature.state }}
        {% endif %}
      unit_of_measurement: "°C"
```

or through the use of a [custom component](https://github.com/dgomes/home-assistant-custom-components)

## Contributors
- [Panagiotis](https://github.com/maragelis)
- [aquaticus](https://github.com/aquaticus)
- [danilkorotkov](https://github.com/danilkorotkov)
