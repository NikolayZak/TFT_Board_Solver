#include "Prompts.hpp"

/*
    here are some common printables that is used throughout the program
*/

const char* main_options = "Please select an option:\n"
                "1) Compute Board!\n"
                "2) Add Champion\n"
                "3) Add Emblem\n"
                "4) Clear Board\n"
                "5) Set Cost Limitation\n"
                "6) Print Custom Settings\n"
                "7) Exit\n";

const char* separator = "====================================================================================================\n";

const char* goodbye = "Thank you for using the TFT Board Solver!\n";

const char* welcome = "Welcome to the TFT Board Solver!\n";

const char* size_prompt = "What is the board size?\n";

const char* champions_prompt = "Type the name or the number of the champion (0 to cancel)\n";

const char* traits_prompt = "Type the name of the number of the trait (0 to cancel)\n";

const char* cost_prompt = "Type the cost restriction (0 for default)\n";