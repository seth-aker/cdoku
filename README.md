# Cdoku Puzzle Generator

Generate unique and interesting sudoku puzzles quickly. 

## Usage 
It's simple: pass the target difficulty ["BEGINNER", "EASY", "MEDIUM", "HARD", "IMPOSSIBLE"] and the desired count of puzzles to generate at that difficulty as arguments. 

Example: `puzzle_generator_app EASY 10`


## Design philosophy
Puzzle generation and solving are designed with the fastest practical implementation in mind.
- All data exists on the stack
- Lookup tables are used extensively to reduce ALU processing.
- Knuth's Algorithm X used to efficiently solve puzzles when checking for uniqueness and validity.
- Logical solving algorithm used to determine the human-level difficulty of a puzzle.


#### \*AI Disclaimer\*
No AI was used to write any code within the src directory. The test suite was written with AI and clearly has a number of issues that need addressing including code-coverage.

