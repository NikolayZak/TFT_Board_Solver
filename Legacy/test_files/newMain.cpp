#include "../LocalDB.hpp"
#include "../Solver.hpp"




// compile test with: g++ ../FastVector.cpp ../Common.cpp ../TopScoringBoards.cpp ../Serialisation.cpp ../LocalDB.cpp ../Board.cpp ../Solver.cpp newMain.cpp -lsqlite3 -o test
int main() {
    // Initialize the local database
    LocalDB db("database.db");
    SetData set_data = db.allocSet(11);
    Solver solver(set_data, 10); // Create a solver with heap size of n
    solver.UpdateData(set_data, 10, {"Dryad"}, {"Janna"});
    vector<BoardResult> answer = solver.Solve(6); // compute the target size

    // send result to crow 



    // testing
    cout << "Optimal Boards:\n";
    for (const auto& board_result : answer) {
        board_result.Print();
    }
    
    cout << "Runtime: " << solver.Runtime() << " seconds\n";
    set_data.deallocSet(); // Clean up the allocated set data

    return 0;
}