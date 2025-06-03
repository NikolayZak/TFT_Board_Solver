#include "../LocalDB.hpp"
#include "../newSolver.hpp"

// compile test with: g++ ../Fast_Vector.cpp ../Common.cpp ../Serialisation.cpp ../LocalDB.cpp ../newBoard.cpp ../newSolver.cpp newMain.cpp -lsqlite3 -o test
int main() {
    // Initialize the local database
    LocalDB db("database.db");
    SetData set_data = db.allocSet(11);
    Solver solver(set_data, 10); // Create a solver with heap size of 10
    vector<BoardResult> answer = solver.Solve(11);
    cout << "Optimal Board:\n";
    for(int i = 0; i < answer[0].board.size(); i++) {
        cout << answer[1].board[i] << " ";
    }
    cout << "\nScore: " << answer[0].score << "\n";
    cout << "Runtime: " << solver.Runtime() << " seconds\n";
    set_data.deallocSet(); // Clean up the allocated set data

    return 0;
}