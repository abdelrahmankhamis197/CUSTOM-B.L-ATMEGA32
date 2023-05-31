# CUSTOM-B.L-ATMEGA32

## 1. Introduction 
* This is a custom Bootloader based on ATMEGA32 target complaint with UDS ( Unified Diagnostic Services ).
* UART is used as communication protocol between the client side [i.e Laptop] and the server side [ECU].
* UDS is a software communication protocol,in this project only few services related to flashing process have been used.

* The main job of UDS in such operation would be:
1. To set the server into a reprogramming session mode.
2. To start a reprogramming sequence.
3.T o handle the start and stop of data transfer.
4. To handle the size and right order of both the data blocks being sent/received and the memory blocks where such data blocks should be stored.
5. To allow the client to start/stop a routine that runs on the server.
6. To allow the client to request a software reset event on the server.

## 2. Design
* Using UDS commiunication protocol client side will send Service ID (0x10) followed by (03) as a request for programming session.
* Either we have positive (Service ID+0x40) or negative response (0x7F).
* Second step will be sending SI (0x34 + data size) which means I ask to flash the dtat size that will be normally the flash page size.
* Dpending on positive response i will procced with the third step send the data itself with SI (0x36 + flash page).
* Till everything is done Request transfer exit will be sent (0x37).

## Resources
* Arabic
1. https://www.youtube.com/watch?v=s3Iecog7hrM .
2. https://www.youtube.com/watch?v=clgxoJOygqA .
* English
1. https://www.youtube.com/watch?v=CV_B8tJgI5E&pp=ygUDdWRz .
