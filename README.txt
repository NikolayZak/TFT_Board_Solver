# TFT_Board_Solver
Solves TFT boards with the most active synergies.
Current Set: 10

// Overview
Includes methods for adding champions, traits, printing settings, clearing settings, as well as limiting the cost or keeping its default restrictions.
(uses the board size as your current level and if a tier is possible those cost units are allowed)
This program is easily modifiable for future sets. Best optimized when the most "favoured" champions (usually with the most traits) are at the top of "champs.csv".

// Rebuild
The Rebuilt version now contains multiple embeded classes that build off one-another and is much cleaner and easier to maintain/edit.
Solver(Board(Database)) is the main class, where (Interface) and (Hasher) are independent classes that handle their names respectively.

// Time Complexity
let t` be the number of traits any given champ has
let t`` be the number of tiers any given trait has
let c be the number of champions in the set
let c` be the number of champions on a given board

The main solver has a worst case runtime of O((c choose c')*(t`*t``))
You can think of the runtime as this O((# of boards possible)*(# operations per board))
(t`*t`` is pretty much O(1))
(when Blank_Score == Max_Increase you will 100% have the optimal boards && the worst time complexity)
Blank_Score is the score we give to empty slots while building the board, this way we can prune the boards before we even compute them.
Max_Increase is the biggest score increase any 1 champion can offer.



// Interesting Outcomes
The solver loves a having 8+ active first tier traits avoiding higher tiers as they are usually inefficient.
It turns out that spamming 9 noxus or 8 void is almost never the most efficient use of traits!
My program prunes these out very fast and unless you set the value of 9 noxus really high, raise the value of Blank_Score and Max_Increase accoardingly causing the program to load for an ungodly amount of time,
you won't ever see these outcomes.

In the future I plan on adding an edge case detector which will add these anomalies to the computed boards, if the user ups the value of these rare stacked traits.
