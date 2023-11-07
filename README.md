# MacMahon Game Solver

This repository contains a C++ implementation of a solver for the MacMahon Game, a puzzle that involves arranging colored tiles on a grid in a way that matches the colors at the edges of adjacent tiles. The solver utilizes various strategies, including backtracking and parallel processing, to efficiently find a solution to the game.

### Features

- **File Input**: Reads game configurations from a file, including grid size and tile colors.
- **Tile Manipulation**: Provides utility functions to convert between string representations of tiles and their character vector equivalents.
- **Color Coding**: Uses ANSI escape codes to color-code the tiles for terminal output, enhancing visual representation.
- **Printing Utilities**: Functions to print the current grid state and the final result in a color-coded format.
- **Backtracking Solver**: Implements a backtracking algorithm to find a solution to the tile placement puzzle.
- **Multithreading**: Utilizes a ThreadPool for concurrent processing, increasing efficiency on multi-core systems.
- **Parallel Recursion**: Employs parallel recursive calls to explore possible solutions concurrently.

### Building the Project

```sh
# Clone the repository
git clone https://github.com/emmanuel-aubertin/MacMahon-Solver.git

# Navigate to the repository directory
cd MacMahon-Solver

# Compile the project & run it
make
```

### Usage

To run the MacMahon Game Solver, provide a text file with the game configuration as input:

```sh
./MacMahonSolver -f ./grid/6x6.txt
```

If you need more launch option you can use `--help`.

The input file should be formatted as follows:

- The first line should contain two integers representing the number of columns and rows.
- Each subsequent line should represent a tile with four characters (one for each edge color).

```
6 6
B R G B
B B G G
R G G R
....
```

### License

This project is licensed under the GNU License - see the [LICENSE](LICENSE) file for details.

### External documentation

Inpiration & understanding threadpool in c++ see [stackoverflow](https://stackoverflow.com/questions/15752659/thread-pooling-in-c11)
Understanding why use condition_variable read polytechnique [documentation](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/thread/condition_variable.html#:~:text=The%20condition_variable%20class%20is%20a,a%20spurious%20wakeup%20occurs)
