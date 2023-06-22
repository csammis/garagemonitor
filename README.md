# garagemonitor
ESP8266 + MQTT gadget to tell us whether the garage door is open

This IOT gadget uses an HC-SR04 ultrasonic distance module mounted at 90 degrees to our garage door and an ESP8266 module to publish distance measurements to a local MQTT server. Our Home Assistant instance loads the entity via MQTT Discovery and maps the distance measurement to a binary sensor which indicates whether the door is open or closed.

The distance sensor is mapped into a binary sensor by adding the following template to the configuration YAML:

```yaml
- binary_sensor: 
    - name: "Garage Door" 
      state: > 
         {{ states('sensor.garage_monitor_distance')|float > 30 or states('sensor.garage_monitor_distance')|float < 0 }} 
      icon: > 
        {% if is_state('binary_sensor.garage_door', 'on') %} 
          mdi:garage-open-variant 
        {% else %} 
          mdi:garage-variant 
        {% endif %}
```
