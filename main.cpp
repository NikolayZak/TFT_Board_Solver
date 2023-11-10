#include "Solver.hpp"
#include "Hasher.hpp"
#include "Interface.hpp"




int main(){
    // initialization
    int size, cost;
    Input I;
    vector<string> champions_added, traits_added;
    Solver S("embeded_text/traits.csv", "embeded_text/champs.csv");
    //S.Blank_Score(8);  //This Sets the solver to always produce the best results at the cost of time efficiency
    //S.Max_Increase(8); //This increases the max_increase

    Hasher H("embeded_text/saved_boards.txt");
    int champs_in_set = S.Champions_In_Set();
    int traits_in_set = S.Traits_In_Set();
    vector<string> all_champions = S.Get_All_Champions();
    vector<string> all_traits = S.Get_All_Traits();
    bool loop = true;
    int choice;



    // main loop
    cout << separator << welcome;
    while(loop){
        cout << separator;
        choice = Get_Input(1, 7, main_options);
        switch (choice){
        case 1: //compute board
            size = Get_Input(S.Champions_Added().size() + 1, champs_in_set - S.Champions_Added().size(), size_prompt);
            H.Configure(S.Cost_Restriction(), S.Champions_Added(), S.Traits_Added(), size);
            if(H.Check_Hashed()){
                Print_Boards(S.Uncompress_Champions(H.Fetch()));
            }else{
                S.Compute_Optimal_Boards(size);
                Print_Boards(S.Optimal_Boards());
                cout << separator << "Execution Time: " << S.Runtime() << " Seconds\n";
                if(S.Runtime() > 5){
                    H.Hash(S.Compressed_Optimal_Boards());
                }
            }
            break;

        case 2: //add champion
            Print_Elements(all_champions);
            I = Get_Input(0, champs_in_set, champions_prompt, all_champions);
            if(I.Is_Int){
                if(I.name != 0){
                    S.Add_Champion(I.name - 1);
                    cout << S.Uncompress_Champions(I.name - 1) << " Added\n";
                }else{
                    cout << "Cancelled\n";
                }
            }else{
                I.name = S.Compress_Champions(I.misspelled);
                S.Add_Champion(I.name);
                cout << S.Uncompress_Champions(I.name) << " Added\n";
            }
            break;

        case 3: //add trait
            Print_Elements(all_traits);
            I = Get_Input(0, traits_in_set, traits_prompt, all_traits);
            if(I.Is_Int){
                if(I.name != 0){
                    S.Add_Trait(I.name - 1);
                    cout << S.Uncompress_Traits(I.name - 1) << " Added\n";
                }else{
                    cout << "Cancelled\n";
                }
            }else{
                I.name = S.Compress_Traits(I.misspelled);
                S.Add_Trait(I.name);
                cout << S.Uncompress_Traits(I.name) << " Added\n";
            }
            break;
        case 4: //clear board
            S.Reset();
            cout << "Board Reset\n";
            break;
        
        case 5: //set cost limitation
            cost = Get_Input(0, std::numeric_limits<int>::max(), cost_prompt);
            S.Cost_Restriction(cost);
            cout << "Cost restriction changed\n";
            break;

        case 6: //print custom settings
            champions_added = S.Uncompress_Champions(S.Champions_Added());
            traits_added = S.Uncompress_Traits(S.Traits_Added());
            Print_Settings(champions_added, traits_added, S.Cost_Restriction());
            break;
        
        case 7: //exits
            cout << goodbye << separator;
            loop = false;
            break;

        default:
            break;
        }
    }

    // clean up
    champions_added.clear();
    traits_added.clear();
    all_champions.clear();
    all_traits.clear();
    return 0;
}


/*
Example of multithreading

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

// The function you want to call
void myFunction(int condition) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread did task " << condition << std::endl;
    // Your function's work goes here
}

int main() {
    vector<int> initial_conditions = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int numTasks = 100;
    vector<thread> threads;

    for (int i = 0; i < numTasks; i++) {
        threads.emplace_back(myFunction, initial_conditions[i%10]);
    }

    // Join all threads to wait for them to finish
    for (std::thread &t : threads) {
        t.join();
        cout << "#";
    }

    return 0;
}
*/