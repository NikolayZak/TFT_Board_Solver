# TFT_Board_Solver
Solves TFT boards with the most active synergies.
Current Set: 11

// Overview
Includes methods for adding champions, traits, printing settings, clearing settings, as well as limiting the cost or keeping its default restrictions.
(uses the board size as your current level and if a tier is possible those cost units are allowed)
This program is easily modifiable for future sets. Best optimized when the most "favoured" champions (usually with the most traits) are at the top of "champs.csv".

// Rebuild
The Rebuilt version now contains multiple embeded classes that build off one-another and is much cleaner and easier to maintain/edit.
MultiThreadedSolver(Solver(Board(Database))) is the main class, where (Interface) and (Hasher) are independent classes that only are used for the interface and quality of life.
Recently added (Fast_Vector) which uses specific facts about the project to optimize std::vector methods

// Time Complexity
let c be the number of champions in the set
let c` be the number of champions on a given board

The main solver has a worst case runtime of O(c choose c')
You can think of the runtime as this O(# of boards possible)
(when Blank_Score == Max_Increase you will 100% have the optimal boards && the worst time complexity)
Blank_Score is the score we give to empty slots while building the board, this way we can prune the boards before we even compute them.
Max_Increase is the biggest score increase any 1 champion can offer.

// Multithreading
multithreading has been added to split the work evenly among threads
due to some optimizations not all threads will finish at the same time without redistributing the work. Aka: bottle-necking has been ignored for now

// Interesting Outcomes
The solver loves a having 8+ active first tier traits avoiding higher tiers as they are usually inefficient.
It turns out that spamming 9 noxus or 8 void is almost never the most efficient use of traits!
My program prunes these out very fast and unless you set the value of 9 noxus really high, raise the value of Blank_Score and Max_Increase accoardingly causing the program to load for an ungodly amount of time,
you won't ever see these outcomes.

// How To Compile
to compile the executable, you need to open your IDE of choice and ENTER: "make run". Provided you have all the libraries, it should compile and produce "TFT Board Solver #.exe"
You can then copy the application to a folder with a copy of "traits.csv" and "champs.csv" for the application to run.

// Future endeavours
In the future I plan on adding an edge case detector which will add these anomalies to the computed boards, if the user ups the value of these rare stacked traits.

// Key points for programmers
If you are looking to use the repository commercially please email me at nikolayzakirov31@gmail.com, otherwise I don't care if you use it.
Start by reading ***Multithreaded_Solver.hpp IS THE LIBRARY OF INTEREST*** and undestand the functions, every other file is undelying methods for Multithreaded_Solver.hpp
