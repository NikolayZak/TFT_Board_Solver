# TFT_Board_Solver
Solves TFT boards with the most amount of synergies active.
Current Set: 9.5

Includes methods for adding champions, emblems/hearts, printing settings, clearing settings, as well as limiting the cost or keeping its default probabilities.
(uses the board size as your current level and if a tier is possible those cost units are allowed)

This program is easily modifiable for future sets. Best optimized when the most "favoured" champions are put at the top of the lists in the embeded text folders.

let n be the number of champions on each board; let x be the number of champions in the set
Each board is computed in O(log(n)) as we constuct them recursively. The number of possible boards is O(x choose n) there are optimizations that prune boards that are doomed to fail to save computation time.

The worst case senario is O(log(n)*(x choose n))
