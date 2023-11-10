#include "Board.hpp"
/*          Time Complexity
    let t be the number of traits in the set
    let t` be the number of traits any given champ has
    let t`` be the number of tiers any given trait has
    let c be the number of champions in the set
*/



// Constructor   O(c*t` + t*t``)
Board::Board(const string &traits_file, const string &champions_file){
    db = new Database(traits_file, champions_file);
    board_score = 0;
    cost_restriction = db->level_restriction.back();
}

// Copy Constructor
Board::Board(const Board &a_board){
    db = new Database(*a_board.db);
    board_score = a_board.board_score;
    cost_restriction = a_board.cost_restriction;
}

// Deconstructor   O(t+c)
Board::~Board(){
    delete db;
    db = nullptr;
    current_board.clear();
}


// gets the compressed current board   O(1)
vector<int> Board::Get_Board(){
    return current_board;
}

// uncompresses a compressed board   O(c`)
vector<string> Board::Uncompress_Champions(const vector<int> &compressed){
    vector<string> current;
    for(size_t i = 0; i < compressed.size(); i++){
        current.push_back(db->all_champions[compressed[i]]->name);
    }
    sort(current.begin(), current.end());
    return current;
}

// uncompresses a Compressed vector of traits
vector<string> Board::Uncompress_Traits(const vector<int> &traits){
    vector<string> current;
    for(size_t i = 0; i < traits.size(); i++){
        current.push_back(db->all_traits[traits[i]]->name);
    }
    sort(current.begin(), current.end());
    return current;
}

// uncompresses a champion
string Board::Uncompress_Champion(const int &champion){
    return db->all_champions[champion]->name;
}

// uncompresses a trait
string Board::Uncompress_Trait(const int &trait){
    return db->all_traits[trait]->name;
}

// returns the champion integer of the last added champ   O(1)
int Board::Back(){
    return current_board.back();
}

// returns the board size O(1)
int Board::Size(){
    return (int)current_board.size();
}

// returns the board score   O(1)
int Board::Score(){
    return board_score;
}

// fields a champion on the board   O(t`*t``)
bool Board::Field_Champion(const int &int_champion, const bool &needs_synergy){
    // checks for a cost restriction
    if(db->all_champions[int_champion]->cost > cost_restriction){
        return false;
    }
    // checks for a required synergy
    if(needs_synergy){
        bool synergy = false;
        for(auto &trait : db->all_champions[int_champion]->traits){
            if(trait->fielded != 0){
                synergy = true;
                break;
            }
        }
        if(!synergy){
            return false;
        }
    }

    // fields the traits and updates the board score
    current_board.push_back(int_champion);
    int counter, score, prev_score;
    for(auto &trait : db->all_champions[int_champion]->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded += 1;
        while(counter < (int)trait->tiers.size() && trait->fielded >= trait->tiers[counter]){
            counter++;
        }
        score = trait->tier_values[counter-1];
        trait->score = score;
        board_score += score - prev_score;
    }
    return true;
}

// unfields the last champion   O(t`*t``)
// precondition: must be at least 1 champion
void Board::Unfield_Champion(){
    int int_champion = current_board.back();
    current_board.pop_back();
    int counter, score, prev_score;
    for(auto &trait : db->all_champions[int_champion]->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded -= 1;
        while(counter < (int)trait->tiers.size() && trait->fielded >= trait->tiers[counter]){
            counter++;
        }
        score = trait->tier_values[counter-1];
        trait->score = score;
        board_score += score - prev_score;
    }
}

// shadow unfields a champion
void Board::Shadow_Unfield_Champion(const int &int_champion){
    int counter, score, prev_score;
    for(auto &trait : db->all_champions[int_champion]->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded -= 1;
        while(counter < (int)trait->tiers.size() && trait->fielded >= trait->tiers[counter]){
            counter++;
        }
        score = trait->tier_values[counter-1];
        trait->score = score;
        board_score += score - prev_score;
    }
}

// fields a trait   O(t``)
void Board::Field_Trait(const int &int_trait){
    int counter = 0;
    int score = 0;
    Trait* trait = db->all_traits[int_trait];
    int prev_score = trait->score;

    trait->fielded += 1;
    while(counter < (int)trait->tiers.size() && trait->fielded >= trait->tiers[counter]){
        counter++;
    }
    score += trait->tier_values[counter-1];
    trait->score = score;
    board_score += score - prev_score;
}


// shadow unfields a specific trait
void Board::Unfield_Trait(const int &int_trait){
    int counter = 0;
    int score = 0;
    Trait* trait = db->all_traits[int_trait];
    int prev_score = trait->score;

    trait->fielded -= 1;
    while(counter < (int)trait->tiers.size() && trait->fielded >= trait->tiers[counter]){
        counter++;
    }
    score += trait->tier_values[counter-1];
    trait->score = score;
    board_score += score - prev_score;
}

// ropes the data through   O(1)
int Board::Champions_In_Set(){
    return db->champions_in_set;
}

// ropes the data through   O(1)
int Board::Traits_In_Set(){
    return db->traits_in_set;
}

// calculates the average score increase of the first tier   O(c*t`)
float Board::Average_Score_Increase(){
    float total_increase = 0;
    for(const auto &champion : db->all_champions){
        for(const auto &trait : champion->traits){
            //total_increase += Average_Trait_Tier_Increase(trait);
            total_increase += trait->tier_values[1];
        }
    }
    return total_increase / db->champions_in_set;
}

// calculates the maximum increase   O(c*t`)
int Board::Maximum_Score_Increase(){
    int highscore = 0;
    int counter = 0;
    for(const auto &champion : db->all_champions){
        counter = 0;
        for(const auto &trait : champion->traits){
            //counter += Max_Trait_Tier_Increase(trait);
            counter += trait->tier_values[1];
        }
        if(counter > highscore){
            highscore = counter;
        }
    }
    return highscore;
}

// returns a vector of all the champions   O(c)
vector<string> Board::All_Champions(){
    vector<string> all_champs;
    for(const auto &champ : db->all_champions){
        all_champs.push_back(champ->name);
    }
    return all_champs;
}

// returns a vector of all the traits   O(t)
vector<string> Board::All_Traits(){
    vector<string> all_traits;
    for(const auto &trait : db->all_traits){
        all_traits.push_back(trait->name);
    }
    return all_traits;
}

// fields a champ without adding it to the local list of champions   O(t'*t'')
void Board::Shadow_Field_Champion(const int &int_champion){
    int counter, score, prev_score;
    for(auto &trait : db->all_champions[int_champion]->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded += 1;
        while(counter < (int)trait->tiers.size() && trait->fielded >= trait->tiers[counter]){
            counter++;
        }
        score += trait->tier_values[counter-1];
        trait->score = score;
        board_score += score - prev_score;
    }
}

// sets the cost restriction   O(1)
void Board::Set_Cost_Restriction(const int &cost){
    cost_restriction = cost;
}

// ropes the level restriction vector through   O(1)
vector<int> Board::Level_Restriction(){
    return db->level_restriction;
}

// compresses a champion   O(n)
int Board::Compress_Champion(const string &champion){
    int counter = 0;
    for(const auto &champ : db->all_champions){
        if(champ->name == champion){
            return counter;
        }
        counter++;
    }
    return counter;
}

// compresses a trait   O(n)
int Board::Compress_Trait(const string &traits){
    int counter = 0;
    for(const auto &trait : db->all_traits){
        if(trait->name == traits){
            return counter;
        }
        counter++;
    }
    return counter;
}