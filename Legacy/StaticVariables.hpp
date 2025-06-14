#pragma once
#define MAX_CHAMPION_TRAITS 5 // Maximum number of traits a champion can have
#define MAX_TRAIT_TIERS 15 // Maximum number of tiers a trait can have
#define MAX_CHAMPIONS 64 // Maximum number of champions in a set (THIS CANNOT EXCEED 64 OR IT WILL BREAK FastVector)
#define MAX_PLAYER_LEVEL 10 // Maximum player level in the game
#define LOCAL_DB_PATH "database.db" // Path to the local database file
#define STATIC_DATA_FILE "static" // Path to the static data
#define JOB_EXPIRY_DURATION 67 // Data Available for n seconds
#define JOB_CLEANUP_TIMER 29 // Checks to delete Data every n seconds