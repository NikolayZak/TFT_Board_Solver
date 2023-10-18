#include "TFT_Algorithm.hpp"




// creates a trait_Node (private)
trait_Node* tft_database::createTraitNode(){
    trait_Node* newNode = new trait_Node;
    newNode->fielded = 0;
    newNode->score = 0;
    return newNode;
}





// finds a trait node (private)
trait_Node* tft_database::find_Node(string &name) { 
    for (auto &current : trait_root) {
        if (current->name == name) {
            return current;
        }
    }
    
    // Return nullptr if the node is not found
    return nullptr;
}


// creates the trait datastructure (private)
vector<trait_Node*> tft_database::load_trait_reg() {
    vector<trait_Node*> trait_root;
    trait_Node* trait_current = nullptr;

    stringstream dataStream(TRAITS_CSV);
    string line;
    while (getline(dataStream, line)) {
        trait_current = createTraitNode();
        trait_current->name = line;

        getline(dataStream, line);
        stringstream tierStream(line);
        int tier;
        while (tierStream >> tier) {
            trait_current->tiers.push_back(tier);
            if (tierStream.peek() == ',') {
                tierStream.ignore();
            }
        }

        getline(dataStream, line);
        stringstream tierValueStream(line);
        int tierValue;
        while (tierValueStream >> tierValue) {
            trait_current->tier_values.push_back(tierValue);
            if (tierValueStream.peek() == ',') {
                tierValueStream.ignore();
            }
        }
        trait_current->tiers.push_back(-1);
        trait_root.push_back(trait_current);
    }

    return trait_root;
}




// creates the champion datastructure (private)
vector<champion_Node*> tft_database::load_champ_reg() {
    vector<champion_Node*> champion_root;
    champion_Node* champion_current;
    string trait_name;

    stringstream dataStream(CHAMPIONS_CSV);
    string line;
    while (getline(dataStream, line)) {
        champion_current = new champion_Node;

        stringstream champion_file_data(line);
        string field;
        getline(champion_file_data, field, ',');
        champion_current->name = field;

        getline(champion_file_data, field, ',');
        champion_current->cost = stoi(field);

        while (getline(champion_file_data, field, ',')) {
            trait_Node* tmp_trait = find_Node(field);
            champion_current->traits.push_back(tmp_trait);
        }
        champion_root.push_back(champion_current);
    }
    return champion_root;
}



// loads and returns the full database (constructor)
tft_database::tft_database(){
    trait_root = load_trait_reg();
    champion_root = load_champ_reg();
    P_champs_in_set = champion_root.size();
    P_traits_in_set = trait_root.size();
    P_score = 0;
    P_cost_limitation = 5;
    P_max_size = -1;
    P_highscore = -1;
}

tft_database::~tft_database(){
    for(auto& trait : trait_root){
        delete trait;
    }
    for(auto& champ : champion_root){
        delete champ;
    }
}



//fields a trait to the database (public)
void tft_database::feild_trait(int value){
    P_emblems_added.push_back(value);

    trait_Node* current = trait_root[value];
    current->fielded += 1;
    
    int counter = 0, score = 0;
    int prev_score = current->score;
    while(current->fielded >= current->tiers[counter] && current->tiers[counter] != -1){
        counter++;
    }
    score += current->tier_values[counter-1];
    current->score = score;
    P_score += score - prev_score;
}



// fields a champ to the database (private)
bool tft_database::feild_champ(int value){
    champion_Node* current = champion_root[value];
    int counter = 0, score = 0, prev_score = 0;

    if(P_cost_limitation < current->cost){ return false; }

    for(auto &trait : current->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded += 1;
        while(trait->fielded >= trait->tiers[counter] && trait->tiers[counter] != -1){
            counter++;
        }
        score += trait->tier_values[counter-1];
        trait->score = score;
        P_score += score - prev_score;
    }
    return true;
}

//field champ public method
void tft_database::feild_champion(int value){
    P_champs_added.push_back(value);

    champion_Node* current = champion_root[value];
    int counter = 0, score = 0, prev_score = 0;

    for(auto &trait : current->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded += 1;
        while(trait->fielded >= trait->tiers[counter] && trait->tiers[counter] != -1){
            counter++;
        }
        score += trait->tier_values[counter-1];
        trait->score = score;
        P_score += score - prev_score;
    }
}


// returns false if there is no synergy. special case of field (private)
bool tft_database::feild_champ_with_synergy(int value){
    champion_Node* current = champion_root[value];
    int counter = 0, score = 0, prev_score = 0;
    bool synergy = false;

    if(P_cost_limitation < current->cost){ return false; }
    for(auto &trait : current->traits){
        if(trait->fielded != 0){
            synergy = true;
        }
    }
    if(!synergy){ return false; }

    for(auto &trait : current->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded += 1;
        while(trait->fielded >= trait->tiers[counter] && trait->tiers[counter] != -1){
            counter++;
        }
        score += trait->tier_values[counter-1];
        trait->score = score;
        P_score += score - prev_score;
    }
    return true;
}

// unfields a champ from the database (public)
void tft_database::unfeild_champion(int value){
    champion_Node* current = champion_root[value];
    int counter = 0, score = 0, prev_score = 0;

    for(auto &trait : current->traits){
        counter = 0;
        score = 0;
        prev_score = trait->score;
        trait->fielded -= 1;
        while(trait->fielded >= trait->tiers[counter] && trait->tiers[counter] != -1){
            counter++;
        }
        score = trait->tier_values[counter-1];
        trait->score = score;
        P_score += score - prev_score;
    }
}


// converts a raw database board to a string board (private)
string tft_database::convert_board(vector<int> board){
    string names;
    champion_Node* current;
    sort(board.begin(), board.end());
    for(int i = 0; i < (int)board.size(); i++){
        current = champion_root[board[i]];
        names.append(current->name);
        names.append(" ");
    }
    names = names.substr(0, names.length() - 1);
    names.append("\n");
    return names;
}

// converts a vector of integer boards into a string (private)
string tft_database::convert_boards_to_string(){
    string ans;
    for(auto &board : P_computed_boards){
        ans.append(convert_board(board));
    }
    return ans;
}

// converts a vector of integer boards into a vector of string (private)
string tft_database::convert_boards_to_int_string(){
    string ans;
    for(auto &board : P_computed_boards){
        for(auto &champ : board){
            ans.append(to_string(champ));
            ans.append(" ");
        }
        ans = ans.substr(0, ans.length() - 1);
        ans.append("\n");
    }
    return ans;
}

// used to check if an item is in a vector (private)
bool tft_database::check_vec(vector<int> vec, int item){
    for(int i = 0; i < (int)vec.size(); i++){
        if(vec[i] == item){
            return false;
        }
    }
    return true;
}


// computes the optimal board (private)
void tft_database::compute_boards(vector<int> current_board){

    //base case, empty board
    if(current_board.size() == 0){
        for(int i = 0; i < P_champs_in_set; i++){
            if(check_vec(P_champs_added, i) && feild_champ(i)){
                current_board.push_back(i);
                compute_boards(current_board);
                unfeild_champion(i);
                current_board.pop_back();
            }
        }
        return;
    }

    // case final board size reached
    if((int)current_board.size() == P_max_size - (int)P_champs_added.size()){

        if(P_highscore < P_score){
            P_highscore = P_score;
            P_computed_boards.clear();
            P_computed_boards.push_back(current_board);

        }else if(P_highscore == P_score){
            P_computed_boards.push_back(current_board);
        }
        return;
    }

    // case champion on board & needs to add a champ
    int counter = current_board.back() + 1;
    int max_index = P_champs_in_set - (P_max_size - P_champs_added.size() - current_board.size());
    while(counter <= max_index){
        if(check_vec(P_champs_added, counter)){ // check if the champ is already added
            current_board.push_back(counter);

            if((int)current_board.size() + (int)P_champs_added.size() >= P_max_size - 2){ // checks if it's the last 2 champs
                if(feild_champ_with_synergy(counter)){ // checks for a required synergy
                    // check if it's within a good score
                    if(P_score + (5 * (P_max_size - (int)P_champs_added.size() + 1 - (int)current_board.size())) >= P_highscore){
                        compute_boards(current_board);
                    }
                    unfeild_champion(counter);
                }
            }else{ // It's not the last 2 champs check if it's within a good scores
                if(feild_champ(counter)){
                    if(P_score + (5 * (P_max_size - (int)P_champs_added.size() + 1 - (int)current_board.size())) >= P_highscore){
                        compute_boards(current_board);
                    }
                    unfeild_champion(counter);
                }
            }
            current_board.pop_back();
        }
        counter++;
    }
}

void tft_database::default_cost_restriction(){
    if(P_max_size > 6){
        P_cost_limitation = 5;
        return;
    }
    P_cost_limitation = level_chance[P_max_size - 1];
}


// resets the database(public)
void tft_database::reset(){
    P_score = 0;
    P_highscore = -1;
    P_cost_limitation = 5;
    P_emblems_added.clear();
    P_champs_added.clear();
    
    for(auto &current : trait_root){
        current->fielded = 0;
        current->score = 0;
    }
}

// adds the "added_champs" into the computed_boards (private)
void tft_database::add_added_champs_to_computed_boards(){
    for(auto &board : P_computed_boards){
        for(auto &champ : P_champs_added){
            board.push_back(champ);
        }
    }
}


bool tft_database::check_hashed(){
    string line, feild;
    ifstream file;
    vector<int> board;
    file.open(HASHED_BOARDS);

    while(getline(file, line)){
        if(line == P_barcode){
            P_computed_boards.clear();
            getline(file, line);
            while(line != "|"){
                stringstream linestream(line);
                while(getline(linestream, feild, ' ')){
                    board.push_back(stoi(feild));
                }
                P_computed_boards.push_back(board);
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

void tft_database::print_custom_settings(){
    cout << "CHAMPIONS ADDED: ";
    for(auto& champ : P_champs_added){
        cout << champ_i_to_s(champ) << " ";
    }
    cout << endl << "EMBLEMS ADDED: ";
    for(auto& trait : P_emblems_added){
        cout << trait_i_to_s(trait) << " ";
    }
    cout << endl << "COST LIMITATION: "; // needs to be printed in main
}

void tft_database::create_barcode(){
    // make sure everything is sorted
    sort(P_champs_added.begin(), P_champs_added.end());
    sort(P_emblems_added.begin(), P_emblems_added.end());
    string barcode;

    barcode.append(to_string(P_cost_limitation));

    barcode.append(to_string(P_max_size));



    for(int digit : P_champs_added){
        barcode.append(to_string(digit));
        barcode.append(",");
    }

    barcode.append("|");

    for(int digit : P_emblems_added){
        barcode.append(to_string(digit));
        barcode.append(",");
    }


    P_barcode = barcode;
}

void tft_database::save_board(){
    ofstream file;
    file.open(HASHED_BOARDS, ios::app);
    file << P_barcode << endl << P_hashed << "|" << endl;
    file.close();
}


// Function to calculate the Levenshtein distance between two strings (private)
int tft_database::calculateLevenshteinDistance(const string& str1, const string& str2) {
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
            dp[i][j] = min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
        }
    }

    return dp[len1][len2];
}


// Function to find the closest match in the dataset (private)
string tft_database::find_closest_champ(string& misspelled) {
    int minDistance = 10000;
    string closestMatch;

    for(auto& current : champion_root) {
        int distance = calculateLevenshteinDistance(misspelled, current->name);
        if (distance < minDistance) {
            minDistance = distance;
            closestMatch = current->name;
        }
    }

    return closestMatch;
}

// Function to find the closest match in the dataset (private)
string tft_database::find_closest_trait(string& misspelled) {
    int minDistance = 10000;
    string closestMatch;

    for(auto& current : trait_root) {
        int distance = calculateLevenshteinDistance(misspelled, current->name);
        if (distance < minDistance) {
            minDistance = distance;
            closestMatch = current->name;
        }
    }

    return closestMatch;
}


// prints the champions (private)
void tft_database::print_champions() {
    const int itemsPerRow = 6;
    const int itemWidth = 14;
    const int numberWidth = 2;

    int counter = 1;

    for(auto &tmp_champ : champion_root) {
        cout << left << setw(numberWidth) << counter << ") " << setw(itemWidth) << left << tmp_champ->name;
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

//prints the traits (private)
void tft_database::print_traits() {
    const int itemsPerRow = 6;
    const int itemWidth = 14;
    const int numberWidth = 2;

    int counter = 1;

    for (auto &tmp_trait : trait_root) {
        cout << left << setw(numberWidth) << counter << ") " << setw(itemWidth) << left << tmp_trait->name;
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

// prints champions and gets the champion input (public)
int tft_database::get_champ_input(){
    print_champions();
    int i_input = -2;
    string s_input, corrected;
    while(i_input < -1 || i_input >= P_champs_in_set){
        cout << "Option: ";
        getline(cin, s_input);
        try{
            i_input = stoi(s_input);
            i_input -= 1;
        } catch(const exception& e){
            i_input = 0;
            corrected = find_closest_champ(s_input);
            while(champion_root[i_input]->name != corrected){ i_input++;}
        }
        cout << separator;
    }
    return i_input;
}

// prints traits and gets the trait input (public)
int tft_database::get_trait_input(){
    print_traits();
    int i_input = -2;
    string s_input, corrected;
    while(i_input < -1 || i_input >= P_traits_in_set){
        cout << "Option: ";
        getline(cin, s_input);
        try{
            i_input = stoi(s_input);
            i_input -= 1;
        } catch(const exception& e){
            i_input = 0;
            corrected = find_closest_trait(s_input);
            while(trait_root[i_input]->name != corrected){ i_input++;}
        }
        cout << separator;
    }
    return i_input;
}



// uses private methods and variables to neatly return the answer (public)
string tft_database::return_optimal_boards(){
    auto start_time = chrono::high_resolution_clock::now();
    string ans;
    create_barcode();
    if(check_hashed()){
        ans = convert_boards_to_string();
        return ans;
    }

    P_highscore = 0;
    P_score = 0;
    P_computed_boards.clear();
    compute_boards(P_current_board);
    add_added_champs_to_computed_boards();
    ans = convert_boards_to_string();
    P_hashed = convert_boards_to_int_string();

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end_time - start_time;
    cout << "Execution Time: " << time.count() << " Seconds" << endl;

    // hashes only if it took longer then 6 seconds
    if((int)time.count() > 5){
        save_board();
    }
    return ans;
}