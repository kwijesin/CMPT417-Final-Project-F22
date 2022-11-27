# CMPT417-Final-Project-F22
Final project for CMPT417 at SFU, Fall 2022


## Project

### A Genetic Approach to Canonical State Placement for True Distance Heuristics

**Abstract**: True distance heuristics are a class of memory-based heuristics that store the all pairs shortest path 
for a subset K of all N possible states. The efficacy of this heuristic is dependent on the choice of 
canonical states K. In this paper we propose a genetic algorithm solution for optimizing canonical state 
choices for a single-agent pathfinding problem, and compare the solution to other iterative algorithms for 
finding canonical states.

We will be testing choices of states K using a genetic algorithm. The population will be scored on number of nodes 
expanded, and their mutations will shift the states around. The heuristics created by the genetic algorithm are 
used for 2D grid pathfinding with A*. A* can be replaced with other algorithms such as IDA*, to see if choice of 
search algorithm changes the outcome of the genetic search significantly. The scores across generations will be 
compared to determine a calculation time / generation trade-off for efficacy gains. The performance of the genetic 
algorithm, including its efficacy and it's time to calculate, will finally be compared with a more analytical approach 
to canonical state placement.

## Code

The program is written in C++ for QT Creator 6
