Towers of Hanoi CLI simulation
==============================

A simple simulation demonstrating a recursive solution to the Towers of Hanoi
problem.

Build
-----

    $ make
    
Run
---

Usage: `$ ./hanoi [-p] ndisks [delay_ms]`

- `ndisks`: number of disks in the tower

- `delay_ms`: delay between moves (ms)

- `-p`: pretty output


Screenshot
----------

Run as `./hanoi 7 100`

    After 26 moves: 
    [Tower 0]:  7  6  3             
    [Tower 1]:  2                   
    [Tower 2]:  5  4  1             
     - - - - - - - - - - 

Run as `./hanoi -p 7 100`

    After 39 moves: 
                  |                              |                              |              
                  |                              |                              |              
                  |                              |                              |              
                  |                             _|_                             |              
                  |                           _|___|_                           |              
                  |                         _|_______|_                  _______|_______       
     _____________|_____________      _____|___________|_____          _|_______________|_     
    |___________________________|    |_______________________|        |___________________|    
    =============================  =============================  =============================
