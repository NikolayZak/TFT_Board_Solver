#include "../LocalDB.hpp"
#include "../Solver.hpp"




// compile test with: g++ ../FastVector.cpp ../Common.cpp ../TopScoringBoards.cpp ../Serialisation.cpp ../LocalDB.cpp ../Board.cpp ../Solver.cpp newMain.cpp -lsqlite3 -o test
int main() {
    // Initialize the local database
    LocalDB db("database.db");
    SetData set_data = db.allocSet(11);
    SetData copy;
    copy.copySet(set_data);
    set_data.deallocSet(); // Clean up the allocated set data
    
    Solver test(copy, 10);
    vector<BoardResult> ans = test.Solve(3);

    for(int i = 0; i < ans.size(); i++){
        ans[i].Print();
    }

    
    copy.deallocSet();

    return 0;
}