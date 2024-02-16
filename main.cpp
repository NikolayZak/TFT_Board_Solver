#include "Multithreading.hpp"
#include "Hasher.hpp"
#include "Interface.hpp"



int main(){
    // initialization
    int size, cost;
    Input I;
    vector<string> champions_added, traits_added;
    Multithreaded_Solver S(16, "traits.csv", "champs.csv"); // default accuracy is 100%
    //S.Blank_Score(S.Average_Blank_Score() + 1); // this will speed up the program at the cost of accuracy


    Hasher H("saved_boards.txt");
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
                S.Solve(size);
                Print_Boards(S.Optimal_Boards());
                cout << separator << "Execution Time: " << S.Runtime() << " Seconds\n";
                cout << "Score: " << S.Highscore() << endl;
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


// Benchmarking
/*
int main(){
    Multithreaded_Solver* tmp;
    float average;
    for(int i = 1; i < 33; i++){
        tmp = new Multithreaded_Solver(i, "embeded_text/traits10.csv", "embeded_text/champs10.csv");
        average = 0;
        cout << "Number of Threads: " << i << endl;
        for(int j = 1; j < 4; j++){
            tmp->Solve(8);
            cout << "Iteration #" << j << " Runtime: " << tmp->Runtime() << endl;
            average += tmp->Runtime();
        }
        cout << "Average Runtime: " << average/3 << endl; // hardcoding
        delete tmp;
    }
    return 0;
}
*/