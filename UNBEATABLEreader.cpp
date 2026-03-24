//notes for me lmao
//ANSI escape codes for color

#include <iostream>
#include <print>
#include <fstream>
#include <string>

#include <filesystem>
#include <chrono>

const int kMaxNumSongs = 200;

    //function defs
    std::string getFilepath();
    void ReadAndPrintSheet(std::ifstream &ArcadeScoreFile, std::string scoresFilePath);

int main() {

    //go read config.txt; return a found filepath
    std::string scoresFilePath = getFilepath();

    //read in the arcade highscores file found in config.txt
    std::ifstream ArcadeScoreFile;
    ReadAndPrintSheet(ArcadeScoreFile, scoresFilePath);

        //code is fine, but commented for testing
    // i tried to convert these to seconds since epoch time but i suck :p; i am going to DIRECTLY COMPARE whatever these are and NOONE can stop me
    // auto timeLastModified = std::filesystem::last_write_time(scoresFilePath);
    // auto timeLastModifiedChecking = std::filesystem::last_write_time(scoresFilePath);
    // while(true) {
    //     //stuff to check if the file changed
    //     timeLastModifiedChecking = std::filesystem::last_write_time(scoresFilePath);
    //     if( !(timeLastModified==timeLastModifiedChecking) ) {
    //         timeLastModified=timeLastModifiedChecking;
    //         std::println("{}",timeLastModified); //debug
    //         //we can actually get to work now!
    //         ReadAndPrintSheet(ArcadeScoreFile, scoresFilePath);
    //
    //     }
    // }
        //

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
    int indent{};
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



    ArcadeScoreFile >> readChar;
    while(ArcadeScoreFile) {
        if(readChar=='{') {
            indent+=4;
            std::println();
            for (int i=0; i<indent; i++) {
                std::print(" ");
            }
        }
        if(readChar=='}') {
            indent-=4;
            std::println();
            for (int i=0; i<indent; i++) {
                std::print(" ");
            }
        }
        if (indent==8 && readChar=='{') { //once we're a certain depth in the json, we know we're starting with the '{"song":"' bit, discard up to the actual name
            for (int i=1; i<10; i++) { //magic number because :) lol
                ArcadeScoreFile >> readChar;
            }
            //right now, we're ready to read the name!

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

            // ArcadeScoreFile >> maxCombo[songIndex];
            // ArcadeScoreFile >> Crit[songIndex];
            // ArcadeScoreFile >> Perf[songIndex];
            // ArcadeScoreFile >> Great[songIndex];
            // ArcadeScoreFile >> Good[songIndex];
            // ArcadeScoreFile >> Ok[songIndex];
            // ArcadeScoreFile >> Barely[songIndex];
            // ArcadeScoreFile >> Miss[songIndex];
            // ArcadeScoreFile >> Level[songIndex];

            songIndex+=1;



        }
        if (indent>=8) {
            std::print("{}", readChar);
        }
        ArcadeScoreFile >> readChar;
    }


    for (int i=0; i<songIndex; i++) {
            std::println("{:21} | {:10} | {:10} | {:7} | {:.4f} | {:4} | {:4} | {:4} | {:4} | {:4} | {:4} | {:4} | {:4} | {:4}", songName[i], difficultyName[i], modName[i], score[i], accuracy[i], maxCombo[i], Crit[i], Perf[i], Great[i], Good[i], Ok[i], Barely[i], Miss[i], Level[i]);
    }





    ArcadeScoreFile.close();
    return;
}
