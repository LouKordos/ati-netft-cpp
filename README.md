# ATI NETFT (Force / Torque Sensor) Driver for C++
Allows redaing force and torque data, force and torque units and internal calibration / scaling factors via TCP at the max supported frequency of the ATI NetFT sensor.

In contrast to other solutions I found, the data is read via TCP and not UDP, which just lets me sleep better at night, and performance is good enough because the NetFT sensor can only read data at ~1kHZ anyway.

Use the docker container to build.