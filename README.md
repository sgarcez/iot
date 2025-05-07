# IOT Nodes

Source for 3 ESP8266 based IOT nodes. The nodes publish MQTT messages which are plumbed to Grafana via an MQTT-to-Prometheus bridge.

## Components

- 3 x D1 mini ESP8266 boards ([Amazon](https://www.amazon.co.uk/gp/product/B0754W6Z2F)).
- 2 x BME 280 Temperature/Humidity Sensor ([Amazon](https://www.amazon.co.uk/gp/product/B07KY8WY4M/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1))
- 1 x PMS 5003 Particulate Matter Sensor ([Data sheet](https://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms5003-manual_v2-3.pdf))

## Nodes

- Node 1: DEPRECATED
- Node 2:
  - PMS 5003 Particulate Matter Sensor
  - Location: Living Room
- Node 3:
  - BME 280 Temperature/Humidity Sensor
  - Location: Living Room (green PCB)
- Node 4:
  - BME 280 Temperature/Humidity Sensor
  - Location: Bedroom
