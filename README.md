# UNBEATABLE_Rating_Reader

Can read an UNBEATABLE arcade-highscores.json file and print it; it automatically updates upon new data hitting the file.

note: sorry for the bad code lol

## Usage:

Windows: Get all the assosiated DLLs

All: Create a config.txt containing the filepath to arcade-highscores.json



## Compile:

Compile the .cpp with the following command:

> g++ -std=c++23 -Wall -Werror -Wextra UNBEATABLEreader.cpp -o UNBEATABLE-Reader-Linux

Or compile for Windows (from Linux):

> x86_64-w64-mingw32-g++ -std=c++23 UNBEATABLEreader.cpp -o UNBEATABLE-Reader-Windows.exe -lstdc++exp
