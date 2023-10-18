// TFT Board Solver
// Nikolay Zakirov
// 2023-07-15
#include "TFT_Algorithm.hpp"


int get_int_input(int lower, int upper) {
    int input = -1;
    bool input_valid = false;

    while (!input_valid) {
        cout << "Option: ";
        cin >> input;
        if (input < lower || input > upper) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << separator;
            continue;
        }
        input_valid = true;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << separator;
    return input;
}



int main(){
    tft_database db;
    string output;
    bool cost_restriction = true;
    int choice, board_size, tmp, cost;
    bool loop = true;

    cout << separator << welcome;
    while(loop){
        cout << separator << main_options;
        choice = get_int_input(1, 7);
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
