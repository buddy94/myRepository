# TiViMa
TiViMa: a Tiny Virtual Machine - A project assignment for the Operating Systems (IN.4022) course at University of Fribourg

# Project description & requirements

The goal of the project is to implement TiViMa, a small multiprogrammed virtual machine and to use
it to study the behaviour of different scheduling algorithms described during the course.

## TiViMa Internals

Here we describe how our Tiny Virtual Machine works and the API each of its instances exposes.
TiViMa is a virtual machine that can execute commands sent via TCP/UDP messages: every instance of TiViMa listens for sequences of commands on a specified port and executes them.

TiViMa can be configured by specifying:
1. the number of commands it can run in parallel;
2. the clock frequency.

The description that follows gives some specifications of the core components of the virtual machine in a rather abstract way; it is then up to you to decide which data structures to use, how to implement the specifications, and other implementation details (see also the "Implementation Decisions" section of this document). Nevertheless, we recommend to use the same names or naming conventions reported in the rest of this document (e.g., use the same command names and resource names).

### Resources

Each TiViMa instance keeps a list of named resources (typically files) it exposes. It also keeps track of which application is using which resource in which mode.

For the sake of simplicity, we assume that each instance exposes 20 resources (r0, r1,..., r19). Moreover to easily implement TiViMa, we use fictitious resources (i.e., resources can be modelled as entries in a resource table without having a real counterpart).
Resources can be acquired both in read only mode and in read/write mode.

### Commands

TiViMa commands, their execution times, and the number of resources they require are reported in the following table.

| CommandName | ExecutionTime(ms) | RequiredResources |
| :---------: | :---------------: | :---------------: |
|     c0      |         10        |        r1         |
|     c1      |        100        |      2rw, 1r      |
|     c2      |          1        |         -         |
|     c3      |          5        |        5r         |
|     c4      |        250        |        2w         |
|     c5      |         42        |         -         |
|     c6      |         87        |       2w, 1r      |
|     c7      |        447        |         -         |
|     c8      |       1000        |        3r         |
|     c9      |        364        |        10w        |


For the sake of simplicity, each command can be represented in a TiViMa instance by an executable or by a procedure that occupies CPU and resources for the amount of time specified in the previously mentioned table.

A *command entry* is a pair (`<command>, <list_of_resources>`), where `<command>` is the name of the command to be executed (cfr. the above table) and `<list_of_resources>` is a list of entries (`resource_name, mode`) specifying the resources used by the command (see the "TiViMa Resources" section for more information).

For example, (`c1, [(r0, 'w'), (r5, 'r'), (r2, 'w')]`) specifies that command c1 has to be executed on resources r0 (read/write mode), r5 (read mode), and r2 (read/write mode).

We assume that each command uses all the resources it requires during all its run time.

### Applications
A TiViMa application (or program) is a quadruple (`<program_name>, <owner>, <priority_level>, <list_of_commands>`), where `<program_name>` is the name of the program, `<owner>` identifies the owner of the program, `<priority_level>` is the priority level of the application, and `<list_of_commands>` is a list of command entries. The following is an example of application composed by three commands:

```
("program_x", “owner_y”, 100,
    [(c1, [(r0, 'w'), (r5, 'r'), (r2, 'w')]),
(c2, []),
     (c0, [(r7, 'r')])
    ]
)
```

When transmitted via TCP/UDP, applications are serialized as strings complying to the following protocol:

1. the first three lines contain, program name, owner identifier, and priority level, respectively;
2. one command is contained in each subsequent line; the components of its corresponding command entry are separated by a space.

The above application is serialized as follows:

```
program_x
owner_y
100
c1 r0 w r5 r r2 w
c2
c0 r7 r
```

The *time span* of a TiViMa application is the interval of time that starts when the execution of its first command starts and ends when the execution of its last commands ends, while the *optimal execution* time of an application is the sum of the execution time of its components, according to the "Commands" table, shown above.

### Evaluation of Scheduling Strategies with TiViMa

When an application ends or a clock interrupt is received, the TiViMa instance has to decide to which other application CPU and other resources must be assigned according to their priority, to the resources they use, etc.

During this project we use TiViMa to study how the scheduling strategies studied during the course behave on a provided workload. In particular, we evaluate:

- First Come First Served without taking into account application priorities;
- Round Robin without taking into account application priorities;
- [Priority Scheduling similar to the algorithm described in “Figure 2-27. A scheduling algorithm with four priority classes.”](tan06)
- Fair-Sharing considering application priority scores to prioritize different applications of the
same owner.

To do that, we use the workload defined in the file “workload.zip” (available in the course Web site) by measuring:

1. average, max, min, quartiles, and standard deviation of the execution time over all applications;
2. average, max, min, quartiles,and standard deviation of the overhead caused by the multiprogrammed environment. The overhead of an application is measured by computing the difference between its execution time and its optimal execution time;
3. average, min, max, quartiles,and standard deviation of the time waited by each application.

### Implementation Decisions
Everything that is not specified in this document should be discussed and decided by every group; in particular, here is a non comprehensive list of implementation decisions that should be taken:

- How to implement parallel execution?
- how to implement multiprogramming?
- how to model application priority?
- how to model commands?
- how many connections can send commands to TiViMa at a time?
- how to synchronize accounting data structure?

### Minimal Feature Set and Evaluation

In order to succeed this assignment each group has to implement the following list of features:

- two executables, one for the server, one for the clients;
- receive commands through TCP/UDP sockets;
- multiprogramming;
- handle priority of commands;
- implement and evaluate scheduling algorithms.

More precisely, the final grade is computed by following the criteria listed in the following:

|                                                       |     |
| ----------------------------------------------------- | --- |
| **Implemented Features**                              | 40% |
| --- Minimal Feature Set                               | 20% |
| --- Testing program / script                          | 10% |
| --- Additional Features                               | 10% |
| **Code Quality**                                      | 30% |
| --- Simple and Concise Code                           | 10% |
| --- Commented Code                                    | 10% |
| --- Architecture and Helperfiles (Makefile)           | 10% |
| **Documentation**                                     | 20% |
| --- Final 3 min/Person Presentation                   | 10% |
| ------ Time                                           |  5% |
| ------ Content                                        |  5% |
| --- Two Pages Documentation                           | 10% |
| ------ Usage of the application and testing           |  5% |
| ------ Architecture and decisions                     |  5% |
| **Evaluation of the Scheduling Strategies**           | 10% |
| --- Presentation of the results (tables, charts, ...) |  5% |
| --- Discussion of the results                         |  5% |

### Final Handin

Upload your answers to `diuf.unifr.ch/xi/os > Upload`, by following the explanations for the online.

### References
[tang06] - Andrew S. Tanenbaum, Albert S. Woodhull, “The MINIX book - Operating Systems: Design and Implementation", 3rd ed, Pearson / Prentice Hall 2006.
