//notes for me lmao
//ANSI escape codes for color

#include <iostream>
#include <print>
#include <fstream>
#include <string>

#include <filesystem>
#include <chrono>

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

//uhh on second thought dont actually do what you're doing ok thanks
void ReadAndPrintSheet(std::ifstream &ArcadeScoreFile, std::string scoresFilePath) {
    ArcadeScoreFile.open(scoresFilePath);
    if(!ArcadeScoreFile) {
        std::println("arcade-highscores.json not found; did you put the correct filepath in config.txt?");
        std::exit(1);
    }

    // char charRead{};
    // std::println();
    // while(ArcadeScoreFile && !(charRead==':')) {
    //     ArcadeScoreFile.get(charRead);
    //     std::print("{}",charRead);
    // }
    // ArcadeScoreFile.get(charRead);
    // while(ArcadeScoreFile && !(charRead=='g')) {
    //     ArcadeScoreFile.get(charRead);
    //     std::print("{}",charRead);
    // }
    // ArcadeScoreFile.get(charRead);
    // ArcadeScoreFile.get(charRead);
    //
    // std::println();









    ArcadeScoreFile.close();
    return;
}
