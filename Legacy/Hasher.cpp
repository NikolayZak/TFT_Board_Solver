#include "Hasher.hpp"

// constructor
Hasher::Hasher(const string &hash_file){
    filename = hash_file;
}

// deconstructor
Hasher::~Hasher(){
    champions.clear();
    traits.clear();
    hashed_boards.clear();
}

// configures the required info
void Hasher::Configure(const int &cost_restriction, const vector<int> &required_champions,
                        const vector<int> &required_traits, const int &size){
    cost_limit = cost_restriction;
    champions = required_champions;
    traits = required_traits;
    board_size = size;
}

// creates a barcode based on the given inputs
void Hasher::Barcode(){
    string new_barcode;

    // adds the cost limit and board size
    new_barcode.append(to_string(cost_limit));
    new_barcode.append("|");
    new_barcode.append(to_string(board_size));
    new_barcode.append("|");


    // adds the champion ids
    for(int i = 0; i < (int)champions.size(); i++){
        if(i + 1 == (int)champions.size()){
            new_barcode.append(to_string(champions[i]));
        }else{
            new_barcode.append(to_string(champions[i]));
            new_barcode.append(",");
        }
    }
    new_barcode.append("|");

    // adds the trait ids
    for(int i = 0; i < (int)traits.size(); i++){
        if(i + 1 == (int)traits.size()){
            new_barcode.append(to_string(traits[i]));
        }else{
            new_barcode.append(to_string(traits[i]));
            new_barcode.append(",");
        }
    }
    barcode = new_barcode;
}

// checks if the boards is hashed and loads it, if it is   O(n)
bool Hasher::Check_Hashed(){
    Barcode();
    ifstream file;
    string line, field;
    vector<int> board;

    file.open(filename);
    while(getline(file, line)){
        if(line == barcode){
            hashed_boards.clear();
            getline(file, line);
            while(line != "|"){
                stringstream linestream(line);
                while(getline(linestream, field, ' ')){
                    board.push_back(stoi(field));
                }
                hashed_boards.push_back(board);
                board.clear();
                getline(file, line);
            }
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// converts the vector of boards to a string
string Hasher::Vector_To_String(const vector<vector<int>> &vector_boards){
    string ans;
    for(auto &board : vector_boards){
        for(auto &champ : board){
            ans.append(to_string(champ));
            ans.append(" ");
        }
        ans = ans.substr(0, ans.length() - 1);
        ans.append("\n");
    }
    return ans;
}


// hashes compressed boards
void Hasher::Hash(const vector<vector<int>> &compressed_boards){
    ofstream file;
    string boards = Vector_To_String(compressed_boards);

    file.open(filename, std::ios::app);
    file << barcode << endl << boards << "|" << endl;
    file.close();
}

// returns the hashed boards   O(1)
// precondition: you must check if it's hashed first
vector<vector<int>> Hasher::Fetch(){
    return hashed_boards;
}