# SATsolver

    The practical work consists of learning to use the different types of IPCs (InterProcess communication) present in a POSIX system. 
    To do this, a system will be implemented that will distribute SAT tasks (satisfyable or not) solving tasks among several processes.
    
NOTE: SAT solving problems are very complex. Given a formula in propositional logic, automatically determine whether the formula has a variable assignment that makes it true; that is, if it is satisfyable. The fact that its resolution takes longer implies that we will have to wait for certain processes to end up leaving it blocked.

## Requirements

- Minisat
- tr

    sudo apt install -y minisat