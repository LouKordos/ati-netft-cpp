# ATI NETFT (Force / Torque Sensor) Driver for C++
Allows redaing force and torque data, force and torque units and internal calibration / scaling factors via TCP at the max supported frequency of the ATI NetFT sensor.

In contrast to other solutions I found, the data is read via TCP and not UDP, which just lets me sleep better at night, and performance is good enough because the NetFT sensor can only read data at ~1kHZ anyway.

Use the docker container to build.

IMPORTANT: This library is header-only and, to keep it simple, assumes only ONE THREAD is calling / using it at the same time. This is required to avoid using locks and thus offer maximum performance.
If you need the data from multiple threads, have one update thread and read the data from a global variable.

Also, the decision was made to not provide this threading functionality internally in order to keep things simple (again). Spawning threads inside the library may cause unintended behavior for the caller and offering both options would increase complexity too much for such a simple library.

The library uses the [Tracy Profiler](https://github.com/wolfpld/tracy), you need to install the server on your GUI host, the rest is included by just building this library.
