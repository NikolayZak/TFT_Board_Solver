#include "Interface.hpp"
/*          Time Complexity
    let b be the number of boards
    let c` be the number of champions on a given board
*/

// prints the elements of a vector
void Print_Elements(const vector<string> &elements){
    const int itemsPerRow = 6;
    const int itemWidth = 14;
    const int numberWidth = 2;

    int counter = 1;

    for(auto &name : elements) {
        cout << left << setw(numberWidth) << counter << ") " << setw(itemWidth) << left << name;
        if (counter % itemsPerRow == 0) {
            cout << endl;
        }
        counter++;
    }

    // Add a newline if the last row is not completely filled
    if ((counter - 1) % itemsPerRow != 0) {
        cout << endl;
    }
}


// prints the boards   O(b*c`)
void Print_Boards(const vector<vector<string>> &boards){
    for(auto &board : boards){
        for(auto &champ : board){
            cout << champ << " ";
        }
        cout << endl;
    }
}

// gets the integer input of a user
int Get_Input(const int &lower, const int &higher, const string &prompt){
    string input;
    int user_int;
    bool valid = false;
    cout << prompt;
    while(!valid){
        cout << separator << "Option: ";
        cin >> input;

        // Try to convert the input string to an integer
        std::istringstream stream(input);
        if (stream >> user_int) {
            // Conversion successful, check if it's within the specified range
            if (user_int >= lower && user_int <= higher) {
                valid = true;
            }
        }

        // Clear any remaining characters in the input buffer
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    cout << separator;
    return user_int;
}

// prints the custom settings
void Print_Settings(const vector<string> &champions_added, const vector<string> &traits_added, const int &cost_restriction){
    cout << "CHAMPIONS ADDED: ";
    for(auto& champ : champions_added){
        cout << champ << " ";
    }
    cout << endl << "EMBLEMS ADDED: ";
    for(auto& trait : traits_added){
        cout << trait << " ";
    }
    cout << endl << "COST LIMITATION: ";
    if(cost_restriction == 0){
        cout << "DEFAULT" << endl;
    }else{
        cout << cost_restriction << endl;
    }
}