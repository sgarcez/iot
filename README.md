# IOT

## System

There are 3 IOT nodes connected via wifi to an IOT network.

The nodes take sensor measurements and publish MQQT messages to a broker hosted in Aporia.

The broker is a hiveMQ docker container.

There is a separate https://github.com/hikhvar/mqtt2prometheus container which consumes MQTT messages and exposes the latest values as Prometheus metrics.
A Prometheus services scrapes these metrics and a Grafana service displays them.

## Components
- 3 x D1 mini ESP8266 boards ([Amazon](https://www.amazon.co.uk/gp/product/B0754W6Z2F)).
- 2 x BME 280 Temperature/Humidity Sensor ([Amazon](https://www.amazon.co.uk/gp/product/B07KY8WY4M/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1))
- 1 x PMS 5003 Particulate Matter Sensor ([Data sheet](https://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms5003-manual_v2-3.pdf))


Each one has a separate platform.io project, the config is the same:

```
platform = espressif8266
board = d1_mini_lite
framework = arduino
```

## Nodes

- Node 1: DEPRECATED
- Node 2: 
  - PMS 5003 Particulate Matter Sensor
  - Location: next to TV (blue box)
- Node 3:
  - BME 280 Temperature/Humidity Sensor
  - Location: next to TV (green PCB)
- Node 3:
  - BME 280 Temperature/Humidity Sensor
  - Location: bedroom
