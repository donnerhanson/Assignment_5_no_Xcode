<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better, please fork the repo and create a pull request or simply open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
***
***
***
*** To avoid retyping too much info. Do a search and replace for the following:
*** donnerhanson, Mutex_ProdCons, donnerhanson, hanso127@mail.chapman.edu
-->





<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/donnerhanson/Mutex_ProdCons">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Producer Consumer</h3>  

  <p align="center">
  A Multi-Threaded program written in C++ solving the Producer-Consumer problem  
    <br />
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Example](#example)
* [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Program  

This program uses semaphores and thread functionality to demonstrate a multithreaded producer consumer solution to the producer consumer problem. This program uses a dynamically shared memory buffer to store data between threads and signal handling to exit.

If there is a checksum or memory reading error the program will throw an exception and exit.  

This program was tested using CentOS 6.2  


<!-- GETTING STARTED -->
## Getting Started  

To get a local copy up and running follow these simple steps.

### Prerequisites  

* Interfacing with the server
```sh
GNU Compiler Collection (GCC 4.6 or newer with C++11 standard support)
```

### Installation  

1. Clone the repo
```sh
git clone https://github.com/donnerhanson/Mutex_ProdCons.git
```


<!-- USAGE EXAMPLES -->
## Usage  

1. Compile:  
```sh
[centos@centos6 prodcon]$ g++ -c -Wall -Werror -fPIC ip_checksum.c  
[centos@centos6 prodcon]$ g++ -shared -o libip_checksum.so ip_checksum.o  
[centos@centos6 prodcon]$ g++ -L. -I. -std=c++0x -Wall -Wextra -g prodcon.cpp -o prodcon -lip_checksum -lpthread -lrt  
[centos@centos6 prodcon]$ export LD_LIBRARY_PATH=($pwd):$LD_LIBRARY_PATH  

```
2. Run:  
```sh
[centos@centos6 prodcon] $./prodcon <n_items>  
```
3. End:  
```sh
[centos@centos6 prodcon] $<ctrl + c>   
```

## Example  

1. Example 1:  compile, run, save output to file, show first 20 lines, show later output  
```sh
[centos@centos6 prodcon] $ g++ -std=c++0x -Wall -Wextra -g prodcon.cpp -o prodcon -lpthread -lrt   
[centos@centos6 prodcon] $./prodcon 1044 > output.txt  
[centos@centos6 prodcon]$ head -20 output.txt

cksum_2: 0xc3f9  
CheckSum :0xc3f9  
payload:{ 103, 198, 105, 115, 81, 255, 74, 236, 41, 205, 186, 171, 242, 251, 227, 70, 124, 194, 84, 248, 27, 232, 231, 141, 118, 90, 46, 99, 51, 159, 201, 154, 102, 50 }  
in: 1  
out: 1  

cksum_2: 0x11a2  
CheckSum :0x11a2  
payload:{ 13, 183, 49, 88, 163, 90, 37, 93, 5, 23, 88, 233, 94, 212, 171, 178, 205, 198, 155, 180, 84, 17, 14, 130, 116, 65, 33, 61, 220, 135, 112, 233, 62, 161 }  
in: 2  
out: 2  

cksum_2: 0xc967  
CheckSum :0xc967  
payload:{ 65, 225, 252, 103, 62, 1, 126, 151, 234, 220, 107, 150, 143, 56, 92, 42, 236, 176, 59, 251, 50, 175, 60, 84, 236, 24, 219, 92, 2, 26, 254, 67, 251, 250 }  
in: 3  
out: 3  
....
```  
```sh
[centos@centos6 prodcon]$ cat output.txt  

 cksum_2: 0x4b64  
 CheckSum :0x4b64  
 payload:{ 59, 100, 5, 85, 214, 119, 88, 41, 116, 149, 101, 34, 108, 101, 63, 242, 167, 85, 22, 143, 158, 71, 42, 9, 111, 158, 234, 212, 207, 210, 191, 10, 54, 196 }  
 in: 1093  
 out: 1093  

 cksum_2: 0xc5e0  
 CheckSum :0xc5e0  
 payload:{ 95, 12, 59, 183, 54, 175, 77, 155, 209, 185, 0, 17, 172, 167, 102, 194, 54, 5, 9, 96, 14, 121, 255, 248, 77, 206, 202, 12, 216, 0, 208, 56, 13, 11 }  
 in: 1094  
 out: 1094  

 cksum_2: 0x8c9b  
 CheckSum :0x8c9b  
 payload:{ 239, 67, 186, 60, 222, 140, 246, 222, 157, 162, 134, 3, 100, 188, 8, 109, 29, 22, 230, 28, 14, 51, 234, 216, 63, 194, 217, 15, 250, 230, 26, 234, 41, 213 }  
 in: 1095  
 out: 1095  

 cksum_2: 0xd648  
 CheckSum :0xd648  
 payload:{ 38, 7, 97, 28, 229, 254, 190, 107, 1, 34, 40, 10, 144, 69, 32, 118, 97, 47, 170, 75, 7, 233, 13, 224, 249, 8, 198, 19, 242, 239, 232, 24, 246, 73 }  
 in: 1096  
 out: 1096  

 ^C  
 Caught signal 2  

 [centos@centos6 prodcon]$  
```

2. Example 2:  Bad input - Buffer size not passed   

```sh
[centos@centos6 prodcon]$ ./prodcon   
Usage: ./program <nitems>   
```

3. Example 3:  Bad input - Buffer size too small   
```sh
[centos@centos6 prodcon]$ ./prodcon 0   
nitems must be between 0 and 1489   
```

4. Example 4:  Bad input - Buffer size too large   
```sh
[centos@centos6 prodcon]$ ./prodcon 1500      
nitems must be between 0 and 1489   
```
<!-- CONTACT -->
## Contact

Donner Hanson - [@hansondonner](https://twitter.com/hansondonner) - hanso127@mail.chapman.edu - ChapmanID - 001276484

Project Link: [https://github.com/donnerhanson/Mutex_ProdCons](https://github.com/donnerhanson/Mutex_ProdCons)



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/donner-hanson
