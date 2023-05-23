# simple_shell

A project to explore the use of processes by implementing a simple user shell. This shell currently only supports Linux.

## Compilation

A makefile has been included to simplify compilation. Simply type the following prompt:

```
prompt> make
```

## Running

The shell supports two operating modes: interactive mode and batch mode. Interactive mode continually asks the user for a new prompt, which must be entered through stdin. Batch mode allows the user to specify a script containing multiple commands. 

To run the executable in Interactive mode:

```
prompt> ./wish
```

To run the executable in Batch mode:

```
prompt> ./wish "filepath"
```

Note, when in Interactive mode, the shell may be exited by typing:

```
prompt> exit
```

## Usage and Examples

The shell currently only supports one command per prompt/script line. For now, only standard Linux commands are supported. The command should be specified as follows:

```
<command> <arg_1> <arg_2>, ..., <arg_n>
```

A concrete example:

```
ls -l /usr/bin/
```
