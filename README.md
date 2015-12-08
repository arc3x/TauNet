# TauNet

(c) Matthew Slocum 2015

### Dependancies
boost::asio

### Build Instructions

Install the boost libraries with the command:
    
    sudo apt-get install libboost-all-dev
    
Run the make file with the command:
    
    make all
    
    
### Building Test Suite

run the make file with the command

    make test


### Running TauNet

TauNet must be run as a root level process to make network connections

    sudo ./tauNet [-msg]
    
### Notice

TauNet is distributed with a predefined key. Change this value in the setting menu or by directly editing the key.sec file
    
### License

MIT, see LICENCE file  