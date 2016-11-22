# hsh
  _   _  _____ _   _                                 
| | | |/  ___| | | |                                
| |_| |\ `--.| |_| |                                
|  _  | `--. \  _  |                                
| | | |/\__/ / | | |                                
\_| |_/\____/\_| |_/                                
                                                    
                                                    
 _   _                         _____ _   _      _ _ 
| | | |                       /  ___| | | |    | | |
| |_| | ___ _ __  _ __ _   _  \ `--.| |_| | ___| | |
|  _  |/ _ \ '_ \| '__| | | |  `--. \  _  |/ _ \ | |
| | | |  __/ | | | |  | |_| | /\__/ / | | |  __/ | |
\_| |_/\___|_| |_|_|   \__, | \____/\_| |_/\___|_|_|
                        __/ |                       
                       |___/    

The MVP shell

## Files

All source files are contained within /src

Command.c: This handles commands. It deals with running subprocesses and other things such as changing directory and reassigning variables

hashTable.c: This is a simple hashtable implementation to store variables in

hsh.c: This is the main file. Sets up variables and runs the main loop

linkedlist.c: A simple linked list implementation to keep the path in

parser.c: A very basic set of file to parse lines from the profile file to see if the syntax is correct

reader.c: Helper functions for reading files and checking directory contents
