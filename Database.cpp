#include "Database.hpp"
/*          Time Complexity
    let t be the number of traits in the set
    let t` be the number of traits any given champ has
    let t`` be the number of tiers any given trait has
    let c be the number of champions in the set
*/

//          TRAIT CLASS
// constructor   O(1)
Trait::Trait(const string &name, const vector<int> &tiers, const vector<int> &tier_values){
    this->name = name;
    this->tiers = tiers;
    this->tier_values = tier_values;
    fielded = 0;
    score = 0;
}

// deconstructor
Trait::~Trait(){
    // nothing to put here
}

//          CHAMPION CLASS
// constructor   O(1)
Champion::Champion(const string &name, const int &cost, const vector<Trait*> &traits){
    this->name = name;
    this->cost = cost;
    this->traits = traits;
}

// deconstructor  O(t`) 
Champion::~Champion(){
    traits.clear();
}

//          DATABASE CLASS
// constructor   O(c*t` + t*t``)
Database::Database(const string &traits_file, const string &champions_file){
    Parse_Traits(traits_file);
    Parse_Champions(champions_file);
    traits_in_set = (int)all_traits.size();
    champions_in_set = (int)all_champions.size();
}

// deconstructor   O(t+c)
Database::~Database(){
    for(int i = 0; i < champions_in_set; i++){
        delete all_champions[i];
        all_champions[i] = nullptr;
    }
    for(int i = 0; i < traits_in_set; i++){
        delete all_traits[i];
        all_traits[i] = nullptr;
    }
    all_champions.clear();
    all_traits.clear();
    level_restriction.clear();
}

// returns the pointer to a trait   O(t)
Trait* Database::Find_Trait(const string &name){
    for (auto &current : all_traits) {
        if (current->name == name) {
            return current;
        }
    }
    
    // Return nullptr if the node is not found
    return nullptr;
}

// parses the trait file   O(t*t``)
void Database::Parse_Traits(const string &traits_file){
    // open the file
    std::ifstream file(traits_file);
    if (!file.is_open()) {
        cerr << "Failed to open '" << traits_file << "'" << endl;
        return;
    }

    // variables
    string line, trait_name;
    vector<int> tiers, tier_values;
    //parsing
    while (getline(file, line)) {
        trait_name = line;
        getline(file, line);
        tiers = Parse_Int_Line(line);
        getline(file, line);
        tier_values = Parse_Int_Line(line);
        all_traits.push_back(new Trait(trait_name, tiers, tier_values));
    }
    file.close();
}



// parses the champion file   O(c*t`)
// Precondition: all champion traits must be parsed first
void Database::Parse_Champions(const string &champions_file){
    // open the file
    std::ifstream file(champions_file);
    if (!file.is_open()) {
        cerr << "Failed to open '" << champions_file << "'" << endl;
        return;
    }

    // variables
    vector<string> champion_data;
    string line, name;
    vector<Trait*> traits;

    //parsing
    getline(file, line);
    getline(file, line);
    level_restriction = Parse_Int_Line(line);

    while (getline(file, line)) {
        //get the champion's info
        champion_data = Parse_String_Line(line);
        name = champion_data[0];
        int cost = stoi(champion_data[1]);

        //store the trait*s of the champion
        for (size_t i = 2; i < champion_data.size(); i++) {
            traits.push_back(Find_Trait(champion_data[i]));
        }
        //store the new champion
        all_champions.push_back(new Champion(name, cost, traits));
        traits.clear();
    }
    file.close();
}


// parses a line into strings O(n)
vector<string> Database::Parse_String_Line(const string& line) {
    vector<string> values;
    std::stringstream ss(line);
    string value;

    while (getline(ss, value, ',')) {
        values.push_back(value);
    }

    return values;
}

// parses a line into integers   O(n)
vector<int> Database::Parse_Int_Line(const string& line) {
    vector<int> values;
    std::stringstream ss(line);
    string value;

    while (getline(ss, value, ',')) {
        values.push_back(stoi(value));
    }

    return values;
}