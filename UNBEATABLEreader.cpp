//notes for me lmao
//ANSI escape codes for color

#include <iostream>
#include <print>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <thread>

const int kMaxNumSongs = 200;

    //function defs
    std::string getFilepath();
    void ReadAndPrintSheet(std::ifstream &ArcadeScoreFile, std::string scoresFilePath);
    void discardUntilNum(std::ifstream &ArcadeScoreFile);

int main() {

    //go read config.txt; return a found filepath
    std::string scoresFilePath = getFilepath();

    //read in the arcade highscores file found in config.txt
    std::ifstream ArcadeScoreFile;
    ReadAndPrintSheet(ArcadeScoreFile, scoresFilePath);

        //code is fine, but commented for testing
    // i tried to convert these to seconds since epoch time but i suck :p; i am going to DIRECTLY COMPARE whatever these are and NOONE can stop me
    auto timeLastModified = std::filesystem::last_write_time(scoresFilePath);
    auto timeLastModifiedChecking = std::filesystem::last_write_time(scoresFilePath);
    while(true) {
        //stuff to check if the file changed
        timeLastModifiedChecking = std::filesystem::last_write_time(scoresFilePath);
        if( !(timeLastModified==timeLastModifiedChecking) ) {
            timeLastModified=timeLastModifiedChecking;
            std::println("{}",timeLastModified); //debug
            //we can actually get to work now!
            ReadAndPrintSheet(ArcadeScoreFile, scoresFilePath);

        }
    }

    return 0;

}

std::string getFilepath() {
    std::ifstream configfile;
    configfile.open("config.txt");
    if(!configfile) {
        std::println("config.txt not found. Please create it and put the filepath to arcade-highscores.json inside.");
        std::exit(1);
    }
    std::string path{};
    std::getline(configfile,path);
    configfile.close();
    return path;
}

void ReadAndPrintSheet(std::ifstream &ArcadeScoreFile, std::string scoresFilePath) {
    ArcadeScoreFile.open(scoresFilePath);
    if(!ArcadeScoreFile) {
        std::println("arcade-highscores.json not found; did you put the correct filepath in config.txt?");
        std::exit(1);
    }

    char readChar{};
    int songIndex{};

    std::string songName[kMaxNumSongs]{};
    std::string difficultyName[kMaxNumSongs]{};
    std::string modName[kMaxNumSongs]{};
    std::string score[kMaxNumSongs]{};
    double accuracy[kMaxNumSongs]{};
    int maxCombo[kMaxNumSongs]{};
    int Crit[kMaxNumSongs]{};
    int Perf[kMaxNumSongs]{};
    int Great[kMaxNumSongs]{};
    int Good[kMaxNumSongs]{};
    int Ok[kMaxNumSongs]{};
    int Barely[kMaxNumSongs]{};
    int Miss[kMaxNumSongs]{};
    int Level[kMaxNumSongs]{};
    bool cleared[kMaxNumSongs]{};



    ArcadeScoreFile >> readChar;
    while(ArcadeScoreFile) {
            for (int i=1; i<25; i++) { //magic number because guh
                ArcadeScoreFile >> readChar;
            }
            //right now, we're ready to read the first name!
            while(ArcadeScoreFile) {
                while(ArcadeScoreFile && !(readChar=='/')) { //SONG NAME grabber
                    songName[songIndex]=songName[songIndex]+readChar;
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; // discard '/'
                while(ArcadeScoreFile && !(readChar=='\\')) { //DIFFICULTY grabber
                    difficultyName[songIndex]=difficultyName[songIndex]+readChar;
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; // discard '\'
                ArcadeScoreFile >> readChar; // discard '\' part 2
                while(ArcadeScoreFile && !(readChar=='\"')) { //MODIFIER grabber
                    modName[songIndex]=modName[songIndex]+readChar;
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; // discard until ':'
                while(ArcadeScoreFile && !(readChar==':')) {
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; //actaully discard the :
                while(ArcadeScoreFile && !(readChar==',')) { //SCORE grabber
                    score[songIndex]=score[songIndex]+readChar;
                    ArcadeScoreFile >> readChar;
                }
                while(ArcadeScoreFile && !(readChar==':')) { //discard until :
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> accuracy[songIndex]; //ACCURACY grabber

                discardUntilNum(ArcadeScoreFile); //MACCOMBO grabber
                ArcadeScoreFile >> maxCombo[songIndex];

                //Judgement grabbing
                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Crit[songIndex];

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Perf[songIndex];

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Great[songIndex];

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Good[songIndex];

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Ok[songIndex];

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Barely[songIndex];

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Miss[songIndex];


                int wedontcarewhatthisisithinklmao{};
                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> wedontcarewhatthisisithinklmao; //this number i dont care about (?)

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> Level[songIndex]; //LEVEL grabber

                while(ArcadeScoreFile && !(ArcadeScoreFile.peek()=='t' || ArcadeScoreFile.peek()=='f')) { //discard until true/false
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; //begins with 't' or 'f' for true/false
                if(readChar=='t') {
                    cleared[songIndex]=1;
                }
                //we have all the date from this song we need, setup for next loop
                discardUntilNum(ArcadeScoreFile); //goto a number we dont care about
                while(ArcadeScoreFile && !(readChar==':')) { //discard until :
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; //throw the :
                ArcadeScoreFile >> readChar; //and the : before the next song
                songIndex+=1;
            }
    }

    //will NOT work on windows!!
    system("clear");
    system("cls");


    for (int i=0; i<songIndex; i++) { ///supposed to be down
        std::println("\033[31m{:^33}\033[0m || \033[32m{:^10}\033[0m | \033[33m{:^10}\033[0m | \033[34m{:>7}\033[0m | \033[35m{:^.4f}\033[0m | {:^4} | {:^4} | {:^4} | {:^4} | {:^4} | {:^4} | {:^4} | {:^4} | {:^4} | {:^5}", songName[i], difficultyName[i], modName[i], score[i], accuracy[i], maxCombo[i], Crit[i], Perf[i], Great[i], Good[i], Ok[i], Barely[i], Miss[i], Level[i], cleared[i]);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //we printed the table. we can rest for a bit before checking if the file updated.



    ArcadeScoreFile.close();
    return;
}

void discardUntilNum(std::ifstream &ArcadeScoreFile) {
    char readChar{};
    while(ArcadeScoreFile && !(isdigit(ArcadeScoreFile.peek()))) { //discard until number
        ArcadeScoreFile >> readChar;
    }
}
