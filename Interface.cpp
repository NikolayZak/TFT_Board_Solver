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

// local functions
// Function to calculate the Levenshtein distance between two strings (private)
int Calculate_Levenshtein_Distance(const string& str1, const string& str2) {
    const size_t len1 = str1.size();
    const size_t len2 = str2.size();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));

    for (size_t i = 1; i <= len1; ++i) {
        dp[i][0] = i;
    }

    for (size_t j = 1; j <= len2; ++j) {
        dp[0][j] = j;
    }

    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            int cost = (str1[i - 1] != str2[j - 1]);
            dp[i][j] = std::min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
        }
    }

    return dp[len1][len2];
}

// Local Funciton
// Lowercases a string   O(n)
string toLowerCase(const std::string& input) {
    string result;
    for (char c : input) {
        result += tolower(c);
    }
    return result;
}


// Local Function
// Function to find the closest match in the dataset
string Find_Closest_Match(const string &misspelled, const vector<string> &names) {
    int minDistance = 10000;
    string closestMatch;

    for(const string &current : names) {
        int distance = Calculate_Levenshtein_Distance(toLowerCase(misspelled), toLowerCase(current));
        if(distance < minDistance) {
            minDistance = distance;
            closestMatch = current;
        }
    }

    return closestMatch;
}

// gets the integer input of a user and checks if it's either an int or a misspelled
// uses a custom return struct
Input Get_Input(const int &lower, const int &higher, const string &prompt, const vector<string> &data){
    string input;
    Input ans;
    bool loop = true;
    cout << prompt;
    while(loop){
        cout << separator << "Option: ";
        cin >> input;

        // Try to convert the input string to an integer
        std::istringstream stream(input);
        if (stream >> ans.name) {
            // Conversion successful, check if it's within the specified range
            if (ans.name >= lower && ans.name <= higher) {
                ans.Is_Int = true;
                loop = false;
            }
        }else{
            ans.misspelled = Find_Closest_Match(input, data);
            ans.Is_Int = false;
            loop = false;
        }
        // Clear any remaining characters in the input buffer
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    cout << separator;
    return ans;
}

// prints the custom settings
void Print_Settings(const vector<string> &champions_added, const vector<string> &traits_added, const int &cost_restriction){
    cout << "CHAMPIONS ADDED: ";
    if(champions_added.size() == 0){
        cout << "NONE";
    }else{
        for(auto& champ : champions_added){
            cout << champ << " ";
        }
    }

    cout << endl << "EMBLEMS ADDED: ";
    if(traits_added.size() == 0){
        cout << "NONE";
    }else{
        for(auto& trait : traits_added){
            cout << trait << " ";
        }
    }
    
    cout << endl << "COST LIMITATION: ";
    if(cost_restriction == 0){
        cout << "DEFAULT" << endl;
    }else{
        cout << cost_restriction << endl;
    }
}