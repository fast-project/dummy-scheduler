# dummy-scheduler
This repository implements a dummy scheduler which handle MQTT messages.
The implemented MQTT message follows the formate described in git@chili.zdv.uni-mainz.de:repos/fast/docs/kommunikation.md 

##The implemented MQTT messages are :

TX side:

    start vm

    stop vm

    migrate vm 

    initAgent

    stopMonitor


RX side:

    vmStarted

    vmStopped

    vmMigrated

    initAgent



## Build instructions

```bash
mkdir build && cd build
cmake ..
make
```

## Running the demo

1. start the mosquitto daemon 

    ```bash  
    mosquitto -c /etc/mosquitto/mosquitto.conf
    ```
2. listen to all transmitted messages

    ```bash
    mosquitto_sub -t "#"
    ```
3. run dummy-scheduler

    ```bash
    ./dummy-scheduler
    ```

    When the dummy-scheduler runs, it sends some dummy messages messages.
4. send MQTT messages to the dummy scheduler

    ```bash
    ./examples/runTest.bash
    ```

    The runTest.bash script send some dummy message to test how the scheduler parse incoming MQTT message.

## ToDo

1. Add a doxygen documentation.

2. Add multi-threading to accelerate library.