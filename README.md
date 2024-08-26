# bobulator
bobulator is a tiny, modular, and portable virtual machine that is made in C++.  
NOTE: bobulator is currently only available for linux for now, i plan on adding the windows version once the linux version works well

# how to run
bobulator uses json files with the file extension ".mpcc" (Mini PC component) for storing part data and a json file with the file extension ".bobvm" (Bob VM) to store the actual virtual machine data<br>
if you have not made a ".bobvm" file, do  ```./bobulator make vmName cpuName.mpcc ramName.mpcc driveName.mpcc gpuName.mpcc``` and a file called "vmName.bobvm" will be generated with the inputted pc parts<br>
if you have a ".bobvm" file ready, do ```./bobulator run vmName.bobvm```
