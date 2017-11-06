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

Author
------

Stephen Albert <stephen@sherbet.space>

License
-------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
