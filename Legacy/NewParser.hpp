/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-6-18
    Last Modified: 2025-6-18

    Description: NewParser.hpp is the Reworked Parser.hpp
    ADT for extracting this set's data on champs, traits, level chance, and trait tiers/tier values
    Formatting must be precise!

    Champion_file must be formatted as such:

    Level Chance:
    1,1,2,3,4,4,5
    Cassiopeia,1,Noxus,Shurima,Invoker
    Aatrox,5,Darkin,Slayer,Juggernaut
    Naafiri,2,Darkin,Shurima,Challenger
    ...

    NOTE: the second line of integers in Traits_file is for editing the values of any given tier
    Traits_file must be formatted as such:
    Darkin
    0,2
    0,2
    Slayer
    0,2,4,6
    0,2,4,6
    Juggernaut
    0,2,4,6
    0,2,4,6

*/