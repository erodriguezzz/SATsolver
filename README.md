# SATsolver

The practical work consists of learning to use the different types of IPCs (InterProcess communication) present in a POSIX system. 
To do this, a system will be implemented that will distribute SAT tasks (satisfyable or not) solving tasks among several processes.
    
NOTE: SAT solving problems are very complex. Given a formula in propositional logic, automatically determine whether the formula has a variable assignment that makes it true; that is, if it is satisfyable. The fact that its resolution takes longer implies that we will have to wait for certain processes to end up leaving it blocked.

## Requirements

- Minisat
    ```sudo apt install -y minisat```
- tr
    ```sudo apt install tr```

## Running the solver

This solver implements two methods for managing the shared memory between the view and master process. 
These are System V and POSIX API's and you can run which ever you like by compiling by: 

```make rebuild_posix```

or 

```make rebuild_sysv```

## Testing

```make pvs-studio-test-posix```

```make pvs-studio-test-sysv```

```make cpp-check-test```
