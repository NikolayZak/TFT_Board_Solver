#include "Traits.hpp"

/*  TRAITS_CSV formatting!!!!
    ORDER DOES NOT MATTER!

    Example Trait: Challenger
    First type the name of the trait and end with a newline
    Second: type all the tiers of the trait separated with a ',' and ending with a newline
    Third: type the values of each tier you just typed. (This option is for incentivizing the program to make specific pairs or avoid pairs)

    Q/A: When will Third be used?
    In sets where you have a trait like Ace where you don't want more then 1 Ace unless you have all 4.


    EX:
    Challenger
    0,2,4,6,8
    0,2,4,6,8
*/

const char* TRAITS_CSV = R"csv(Darkin
0,2
0,2
Slayer
0,2,4,6
0,2,4,6
Juggernaut
0,2,4,6
0,2,4,6
Ionia
0,3,6,9
0,3,6,9
Sorcerer
0,2,4,6,8
0,2,4,6,8
Demacia
0,3,5,7,9
0,3,5,7,9
Empress
0,1
0,0
Void
0,3,6,8
0,3,6,8
Piltover
0,3,6
0,3,6
Technogenius
0,1
0,0
Shurima
0,2,4,6,9
0,2,4,6,9
Bastion
0,2,4,6,8
0,2,4,6,8
Wanderer
0,1
0,0
Invoker
0,2,4,6,8
0,2,4,6,8
Gunner
0,2,4,6
0,2,4,6
Noxus
0,3,5,7,9
0,3,5,7,9
Bruiser
0,2,4,6
0,2,4,6
Targon
0,2,3,4
0,2,3,4
Vanquisher
0,2,4,6
0,2,4,6
Strategist
0,2,3,4,5
0,2,3,4,5
Challenger
0,2,4,6,8
0,2,4,6,8
Freljord
0,2,3
0,2,3
Zaun
0,2,4,6
0,2,4,6
Rogue
0,2,4
0,2,4
Multicaster
0,2,3,4
0,2,3,5
Ixtal
0,2,3,4
0,2,3,4
Bilgewater
0,3,5,7,9
0,3,5,7,9
Reaver King
0,1
0,0
)csv";