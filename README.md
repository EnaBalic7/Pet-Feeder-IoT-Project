# Pet-Feeder-IoT-Project

Made for college subject.

The topic of my project is a pet feeder. The idea is to make things easier for the pet owner by dispensing food automatically every two or three hours, which is very convenient if the owner is not at home or is busy with work. The owner should only make sure that there is enough food in the food container.

For presentation purposes, food dispensing is set to happen every 10 seconds.

The HTML page shows the distance of the sensor from the food, the greater it is, the less food there is in the container. That is nicely shown by the percentage and the progress bar. There is also a button that can be clicked to start dispensing food whenever the owner wishes, after which a notification is displayed below the button. If the amount of food falls below a certain level, the owner will receive a notification.

**Equipment:** NodeMCU ESP8266, Ultrasonic sensor, Servo motor
**Control side:** HTML, CSS, JavaScript
**Executive side:** Equipment & C/C++ in ArduinoIDE
**Connectivity:** Google Firebase

