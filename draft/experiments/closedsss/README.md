# experiments/closedsss

An experimentation for the state space search of closed positions.

## Goals

For each closed position, we can find a corresponding state-space-search tree
starting from that position until hitting the breakthrough (and stop constructing
after getting to the breakthrough).

Now, we want to know that:

1. What is the (estimate) number of possible states reachable from the closed position?
2. What is the (average) depth of the search tree?
3. What is the (average) branching factor of the search tree?

To the end, we want to know whether we can search the whole tree and evaluate instead of
doing some heuristics or machine learning.
