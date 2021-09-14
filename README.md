# SATsolver

The objective of this practical work is learning how to use the different types of IPCs (Inter-Process communication) present in a POSIX system. To do this, the implemented system will distribute SAT (satisfyable or not) solving tasks among several processes.
    
NOTE: SAT solving problems are very complex. Given a formula in propositional logic, a SAT solver automatically determines whether the formula has a variable assignment that makes it true; that is, if it is satisfyable. The fact that its resolution takes longer implies that we will have to wait for certain processes to end, and therefore it will end up being blocked.

## Requirements

In order to compile the project succesfully, the user must run certain commands beforehand:

- Minisat: 
    ```$ sudo apt install -y minisat```
- tr:
    ```$ sudo apt install tr```


## Compiling the system

This system implements two methods for managing the shared memory between the view and master process; these are System V and POSIX APIs. The user can altern between these two by compiling with either one of the following two commands:

```make rebuild_posix```

or 

```make rebuild_sysv```

## Excecuting the system

After creating both executable files ```p_view``` and ```p_master```, the system can be excecuted in different ways. One of these is by executing the following command:

```$ ./p_master <files> | ./p_view```

where ```<files>``` is the directory where your CNF files are located.

And the other way to execute the system is using two terminals. Firstly, one of them should execute the master process using the following command:

```$ ./p_master <files>```

while in the other terminal, the user executes the view process by typing in the shell the command:

```$ ./p_view <code_1> <code_2>```

where ```<code_1>``` and ```<code_2>``` are shown in stdout after executing master process with the previously mentioned command in the first terminal.

After doing all these steps, these are the possible outputs that resemble to those the user will see:

```
$ ./p_master files/*
0x12345678
0x54321012
```
```
./p_view 0x12345678 0x54321012
15095   FILENAME: files/sample1.cnf:    Number of variables: 100      Number of clauses:  160      CPU time: 0.002118 s      UNSATISFIABLE
```
or

```
$ ./p_master files/* | ./p_view
15095   FILENAME: files/sample1.cnf:    Number of variables: 100      Number of clauses:  160      CPU time: 0.002118 s      UNSATISFIABLE
```

In order for this system to work properly, is important to mention that the shared memory between the processes has a fixed size. 

```#define MAX_LENGTH 65536```

Therefore, it will process the information up to a specific amount independantly of how many files the system receives. 


## Testing

The user can run the implemented static tests by using the following commands:

```make pvs-studio-test-posix```

```make pvs-studio-test-sysv```

```make cpp-check-test```

Note that pvs-studio and cpp-check do need to be installed. 
