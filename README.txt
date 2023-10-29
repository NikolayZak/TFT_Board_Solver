# TFT_Board_Solver
Solves TFT boards with the most amount of synergies active.
Current Set: 9.5

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
(when Blank_Score == Max_Increase)
Blank_Score is the score we give to empty slots while building the board, this way we can prune the boards before we even compute them.
Max_Increase is the biggest score increase any 1 champion can offer.



// Interesting Outcomes
It turns out that spamming 9 noxus or 8 void is almost never the most efficient use of traits!
My program prunes these out very fast and unless you set the value of 9 noxus really high, raise the value of BLANK_SCORE and MAX_INCREASE accoardingly causing the program to load for ungodly amount of time,
you won't ever see these outcomes.

In the future I plan on adding an edge case detector which will add these anomalies to the computed boards, if the user ups the value of these rare stacked traits.
