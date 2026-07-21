# BASICS :
A cube have 6 Face 
UP , DOWN , RIGHT , LEFT , FRONT , BACK 

Each Face 3 moves :
Either move it left , right or we can move it 180 which we know as half turn 

For all 6 faces this looks like :
U U' U2 D D' D2 L L' L2 R R' R2 F F' F2 B B' B2 = 18 total moves


COST of moves:
Face turns : 1 move like U and U' 
180 turn /half turn : 2 moves 

Kociemba proved that worst case can be a max of 29 FT and 40 half turns 
but humne use kiya h 22 max depth which is pratical cutoff

How this is a graph problem :
Node: One complete , distinct state
Edge :apply one of 18 moves takes us from our node to any of the 18 neighbouring node .
Goal node : every corner and edge is placed perfectly in that state .
start node : Any provided state 

Given graph is a undirected ones tohh apne pass 2 options hote hai :
We can search from scrambled to solved state and solved state back to scrambled state 


Why not brute approach :

total nodes : 4.3*10^19 
how ?? 8 corner peices can be arranged in 8! forms 
each corner has 3 possible orientations 0/1/2 3^8 
but this can be 3^7 because if you fix other 7 it got by itself correct 
kaise pta lagega because inka twist%3 should be equal to 0
arranging 12 peices of edges 12!
every edge has two orientation either ulta ya shi hi hoga pehle se 
2^11(hoga 2^12 pr same as corner 2^11 hojayega yaha total number of flipped even hoti h )

Ek valid cube m corner permutation and edge permutation cannot be independent if corner permutation is odd and edge permutation is odd 
tohh half krdenge divide it by 2 
so overall => 4.3*10^19

At every step we can move through 18 different edges as total 18 moves can be reduced if you not undo your last move 

God's Number : (2010 m ayya tha) : worst case is always <=20 

BFS/DFS kiya tohh ??
18^20 each step can have 18 moves and  minimum 20 moves  = 1.7*10^24

precomputation :
this lags in memory we cannot compute all 20 from 4.3*10^19 states in even 1 byte each storage.

So the way of getting solution cannot be brute as we have a undirected unweighted graph 
So kociemba algo solve it in groups:

Here comes the Group theory :

G0 : iss group m sab moves possible hai (4.3*10^19)
G1 : iss group m only U , D k sabhi moves or R, L , F ,B k moves 180 degree bss tohh ye bohot chota hojata h
kitne chota :
as 3^7 and 2^11 become 1 only that means all orientations are same 
and as middle slice edges are sitting in same order that means instead of 12! it is 8!* 4! 
total = results in 19*10^9 only 


once you are in G1 state which is when :


Every corner is correctly oriented (no twist) — your co[] array is all zeros.
Every edge is correctly oriented (no flip) — your eo[] array is all zeros.
The 4 middle-slice edges (FR, FL, BL, BR) are sitting somewhere in the middle layer (order doesn't matter yet, just that they're there).

FR FL BL BR = front right front left aise vale 


Folder structure of Solver part :
Enums.h : Name diya h sab corner edges faces ko 

cubiecube.h : defined corner permutation and orientation and for edge orientations and permutations 


cubiecube.cpp : ye change krta h cp/co or edges vale ko array indexes m instead of cube state 

coordcube.h : Build tow tables (given coordinate + a move find what the new coordinate) and pruning table (there is given coordinate gives us minimum steps to zero it out)

Search.h/cpp : The actual IDA* search : Solution() does phase 1, totaldepth() function does phase 2

main.cpp: reads 54 letter input , call the search.cpp and print moves 



# ENUMS File 

Yaha sirf indexes define kiye hai nothing else is defined 



# CUBIE CUBE FILES:
Cubiecube.h : this forms two structs side by side in human and machine form conversion other comments are in between the files 






