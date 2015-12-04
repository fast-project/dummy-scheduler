# dummy-scheduler
This repository implements a dummy scheduler which handle MQTT messages.
The implemented MQTT messages follow the format described in git@chili.zdv.uni-mainz.de:repos/fast/docs/kommunikation.md 

The scheduler interact to agents initialization requests and initialize agents with random configurations.
The scheduler send random stopMonitor messages to the initialized agents.

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

## Testing interaction with agents
1. start the mosquitto daemon 

    ```bash  
    mosquitto -c /etc/mosquitto/mosquitto.conf
    ```
2. listen to all transmitted messages

    ```bash
    mosquitto_sub -t "fast/agent/#"
    ```
3. simulate agents initialization requests

    ```bash
    ./examples/init_agent.bash
    ```
4. run dummy-scheduler

    ```bash
    ./dummy-scheduler  //the scheduler configuration can be see in this file  ./scheduler.conf  
    ```
   
## ToDo

1. Add a doxygen documentation.

2. Add multi-threading to accelerate library.