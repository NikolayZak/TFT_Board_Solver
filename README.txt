# TFT_Board_Solver
Solves TFT boards with the most amount of synergies active.
Current Set: 9.5

// Overview
Includes methods for adding champions, emblems/hearts, printing settings, clearing settings, as well as limiting the cost or keeping its default probabilities.
(uses the board size as your current level and if a tier is possible those cost units are allowed)
This program is easily modifiable for future sets. Best optimized when the most "favoured" champions are put at the top of the lists in the embeded text folders.

The main class is very overfilled with functions that should be in their own class cpps, however I just squished them in for ease of programming main and avoided using void* as I didn't know it existed.
I may create a "client" version of the code with the barebones for manipulating the database and search, but this main branche will contain all of the auxillary functions that I use to make the most
out of easy access to the datastructure.

// Time Complexity
let n be the number of champions on each board; let x be the number of champions in the set
Each board is computed in O(log(x)) as we constuct them recursively. The number of possible boards is O(x choose n) there are optimizations that prune boards that are doomed to fail to save computation time.

The worst case senario is O(log(x)*(x choose n))

// Interesting Outcomes
It turns out that spamming 9 noxus or 8 void is almost never the most efficient use of traits!
My program prunes these out very fast and unless you set the value of 9 noxus really high, raise the value of BLANK_SCORE and MAX_INCREASE accoardingly causing the program to load for ungodly amount of time,
you won't ever see these outcomes.

In the future I plan on adding an edge case detector which will add these anomalies to the computed boards, if the user ups the value of these rare stacked traits.
