# Home GW

A simple and trivial to reproduce HomeGW for cheap 433Mhz based weather stations and sensors.

## Introduction

The objective of this library is to decode information coming from weather station units which can be found online very cheap.

## Bill of Materials (BOM)

- Arduino (any kind of Arduino will do)
- RF 433Mhz Receiver (I recommend a superheterodyne)
- Breadboard + wires (optianaly a perfboard and soldering iron)
- Antenna (17,4 cm of straigh wire)

## Schematics

![Alt text](/docs/HomeGW.png?raw=true "Schematic")

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
