# Aruco Natar tracker

**C++** implementation of an Aruco tracker for a camera using Natar.

## Prerequisites

- [CMake](https://cmake.org)
- [Hiredis](https://github.com/redis/hiredis)
- [Libev](https://)
- 
- [OpenCV](https://opencv.com) 
- [RedisImageHelper](https://github.com/natar-io/redis-image-helper)
- [cxxopts](https://github.com/jarro2783/cxxopts) Used to manage commandline options.

## Compiling & Installing
Go into project folder and run the following:
```
mkdir build
cd build
cmake ..
make -j8
```
This should create one executables : **natar-tracker-aruco**.

## Running the tracker

The tracker server comes with a lot of command line options. You can check them all using the following command `./natar-tracker-aruco -h`.
```
Usage:
 natar-tracker-aruco [OPTION...]

      --redis-port arg         The port to which the redis client should 
                               try to connect.
      --redis-host arg         The host adress to which the redis client 
                               should try to connect
  -i, --input arg              The redis input key where data are going to 
                               arrive.
  -o, --output arg             The redis output key where to set output 
                               data.
  -s, --stream                 Activate stream mode. In stream mode the 
                               program will constantly process input data 
                               and publish output data.
  -u, --unique                 Activate unique mode. In unique mode the 
                               program will only read and output data one 
                               time.
  -g, --stream-set             Enable stream get/set mode. If stream mode 
                               is already enabled setting this will cause 
                               to publish and set the same data at the same 
                               time
  -c, --camera-parameters arg  The redis input key where camera-parameters 
                               are going to arrive.
  -v, --verbose                Enable verbose mode. This will print 
                               helpfull process informations on the 
                               standard error stream.
  -h, --help                   Print this help message.
```

### How to use
 
Run the tracker like this. Note that the verbose mode (-v) is activated.

```
./natar-tracker-aruco -i camera0 -s -v
```

This will wait for `Publish` events on `camera0`, read the `camera0` key, try to find the marker corners. In the output, you will get a list of markers with their data. The data is `set` to `camera0:markers`. 

``` json 
{"markers":[{"id":45,"dir":0,"confidence":100,"type":"ARUCO","center":[263.75,141.25],"corners":[300.0,103.0,301.0,178.0,228.0,179.0,226.0,105.0]}]}
```
