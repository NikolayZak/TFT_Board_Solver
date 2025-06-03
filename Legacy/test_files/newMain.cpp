#include "../LocalDB.hpp"
#include "../newSolver.hpp"

// compile test with: g++ ../Fast_Vector.cpp ../Common.cpp ../Serialisation.cpp ../LocalDB.cpp ../newBoard.cpp ../newSolver.cpp newMain.cpp -lsqlite3 -o test
int main() {
    // Initialize the local database
    LocalDB db("database.db");
    SetData set_data = db.allocSet(11);
    //set_data.restrictSet(1, {}); // Restrict the set data for player level 1
    Solver solver(set_data, 50); // Create a solver with heap size of 10
    vector<BoardResult> answer = solver.Solve(1);
    cout << "Optimal Boards:\n";
    for (const auto& board_result : answer) {
        board_result.Print();
    }
    
    cout << "Runtime: " << solver.Runtime() << " seconds\n";
    set_data.deallocSet(); // Clean up the allocated set data

    return 0;
}