#include "Solver.hpp"
#include "Hasher.hpp"
#include "Interface.hpp"


int main(){
    // initialization
    int size, trait, champion, cost;
    vector<string> champions_added, traits_added;
    Solver S("traits.csv", "champs.csv");
    Hasher H("saved_boards.txt");
    int champs_in_set = S.Get_All_Champions().size();
    int traits_in_set = S.Get_All_Traits().size();
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
                Print_Boards(S.Uncompress_Boards(H.Fetch()));
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
            champion = Get_Input(0, champs_in_set, champions_prompt);
            if(champion != 0){
                S.Add_Champion(champion - 1);
                cout << S.Champion_To_String(champion - 1) << " Added\n";
            }else{
                cout << "Canceled\n";
            }
            break;

        case 3: //add trait
            Print_Elements(all_traits);
            trait = Get_Input(0, traits_in_set, traits_prompt);
            if(trait != 0){
                S.Add_Trait(trait - 1);
                cout << S.Trait_To_String(trait - 1) << " Added\n";
            }else{
                cout << "Canceled\n";
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
            champions_added = S.Uncompress_Board(S.Champions_Added());
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
    S.~Solver();
    H.~Hasher();
    champions_added.clear();
    traits_added.clear();
    all_champions.clear();
    all_traits.clear();
    return 0;
}






/*
    // command for adding a champion
    Print_Elements(all_champions);
    champion = Get_Input(0, champs_in_set, champions_prompt);
    if(champion != 0){
        S.Add_Champion(champion - 1);
    }

    // command for adding a trait
    Print_Elements(all_traits);
    trait = Get_Input(0, traits_in_set, traits_prompt);
    if(trait != 0){
        S.Add_Trait(trait - 1);
    }



    // command for printing settings
    champions_added = S.Uncompress_Board(S.Champions_Added());
    traits_added = S.Uncompress_Traits(S.Traits_Added());
    Print_Settings(champions_added, traits_added, S.Cost_Restriction());



    // logic for computing a board
    size = Get_Input(S.Champions_Added().size() + 1, champs_in_set - S.Champions_Added().size(), size_prompt);
    H.Configure(S.Cost_Restriction(), S.Champions_Added(), S.Traits_Added(), size);
    if(H.Check_Hashed()){
        Print_Boards(S.Uncompress_Boards(H.Fetch()));
    }else{
        S.Compute_Optimal_Boards(size);
        Print_Boards(S.Optimal_Boards());
        cout << "Execution Time: " << S.Runtime() << " Seconds\n";
        if(S.Runtime() > 5){
            H.Hash(S.Compressed_Optimal_Boards());
        }
    }

    // terminate the program
    S.~Solver();
    H.~Hasher();
    cout << goodbye;
    return 0;
}







int main(){

    cout << separator << welcome;
    while(loop){
        cout << separator << main_options;
        choice = Get_Input(1, 7, main_options);
        switch (choice){
        case 1: //compute board
            cout << "What is the board size?\n";
            board_size = get_int_input(1 + db.board_size(), 15 + db.board_size());
            db.set_max_size(board_size);
            if(cost_restriction){
                db.default_cost_restriction();
            }
            cout << db.return_optimal_boards();
            break;

        case 2: //add champion
            cout << "Type the name or number of the champ you want to add! (enter 0 to cancel)\n";
            tmp = db.get_champ_input();
            if(tmp != -1){
                db.feild_champion(tmp);
                cout << db.champ_i_to_s(tmp) << " added\n";
            }else{
                cout << "Canceled\n";
            }
            break;

        case 3: //add emblem
            cout << "Type the name or number of the trait you want to add! (enter 0 to cancel)\n";
            tmp = db.get_trait_input();
            if(tmp != -1){
                db.feild_trait(tmp);
                cout << db.trait_i_to_s(tmp) << " added\n";
            }else{
                cout << "Canceled\n";
            }
            break;
        case 4: //clear board
            db.reset();
            cout << "Board Reset\n";
            break;
        
        case 5: //set cost limitation
            cout << "What is the cost restriction? 1-5 (enter 0 for default probabilities)\n";
            cost = get_int_input(0,5);
            if(cost == 0){
                cost_restriction = true;
            }else{
                cost_restriction = false;
                db.cost_restriction(cost);
            }
            cout << "Cost restriction changed\n";
            break;

        case 6: //print custom settings
            db.print_custom_settings();
            if(cost_restriction){
                cout << "Default" << endl;
            }else{
                cout << cost << endl;
            }
            break;
        
        case 7: //exits
            cout << goodbye << separator;
            loop = false;
            break;

        default:
            break;
        }
    }
    return 0;
}

*/