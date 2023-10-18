# TFT_Board_Solver
Solves TFT boards with the most amount of synergies active.
Current Set: 9.5

// Overview
Includes methods for adding champions, emblems/hearts, printing settings, clearing settings, as well as limiting the cost or keeping its default probabilities.
(uses the board size as your current level and if a tier is possible those cost units are allowed)

This program is easily modifiable for future sets. Best optimized when the most "favoured" champions are put at the top of the lists in the embeded text folders.

// Time Complexity
let n be the number of champions on each board; let x be the number of champions in the set
Each board is computed in O(log(n)) as we constuct them recursively. The number of possible boards is O(x choose n) there are optimizations that prune boards that are doomed to fail to save computation time.

The worst case senario is O(log(n)*(x choose n))

Because of this, I use methods that will almost never compute 6 or more in 1 trait in exchange for signifigantly faster compution time. It also turns out that
the way this TFT set is designed these boards are 100% of the time the most efficient use of traits since there are no traits that only reap benifits after fielding
4 or more traits.

In a future set if they re-add something similar to the Ace trait, I may need to loosen the efficiency to improve searchspace or develope a designated search for
those edge cases; as it would be a smaller subset of boards.
