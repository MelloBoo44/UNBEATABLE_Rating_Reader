//notes for me lmao

#include <iostream>
#include <print>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <thread>
#include <algorithm>

struct song {
    std::string songName{};
    std::string difficultyName{};
    std::string modName{};
    int score{};
    double accuracy{};
    int maxCombo{};
    int Crit{};
    int Perf{};
    int Great{};
    int Good{};
    int Ok{};
    int Barely{};
    int Miss{};
    int level{};
    bool cleared{};
    //TODO: include rating
};

const int kMaxNumSongs = 1000;

//function defs
std::string getFilepath();
void ReadSheet(std::ifstream &ArcadeScoreFile, std::string scoresFilePath);
void discardUntilNum(std::ifstream &ArcadeScoreFile);
void PrintSheet(int songIndex, song songs[kMaxNumSongs]);
void ColorText(std::string, int i, song songs[kMaxNumSongs]);
void ColorName(int i, song songs[kMaxNumSongs]);
void ColorText(int, int i, song songs[kMaxNumSongs]);
void ColorCrit(int, int i, song songs[kMaxNumSongs]);
void ColorText(double, int i, song songs[kMaxNumSongs]);
bool OverarchingHighlighting(int i, song songs[kMaxNumSongs]);
void ResetColorAndPrintLines(song one, song two);
bool songSort(song const &one, song const &two);

int main() {

    //go read config.txt; return a found filepath
    //TODO: replace with proper confing reading function
    std::string scoresFilePath = getFilepath();

    //read in the arcade highscores file found in config.txt
    std::ifstream ArcadeScoreFile;
    ReadSheet(ArcadeScoreFile, scoresFilePath);

    // i tried to convert these to seconds since epoch time but i suck :p; i am going to DIRECTLY COMPARE whatever these are and NOONE can stop me
    auto timeLastModified = std::filesystem::last_write_time(scoresFilePath);
    auto timeLastModifiedChecking = std::filesystem::last_write_time(scoresFilePath);
    while(true) {
        //stuff to check if the file changed
        timeLastModifiedChecking = std::filesystem::last_write_time(scoresFilePath);
        if( !(timeLastModified==timeLastModifiedChecking) ) {
            timeLastModified=timeLastModifiedChecking;
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // i put this here because uhh iuhmm yeah?
            //we can actually get to work now!
            ReadSheet(ArcadeScoreFile, scoresFilePath);

        }
        // TODO: new thread?
        // std::string command{};
        // std::getline(std::cin, command);
        // std::println("cmd:{}",command);
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

void ReadSheet(std::ifstream &ArcadeScoreFile, std::string scoresFilePath) {
    ArcadeScoreFile.open(scoresFilePath);
    if(!ArcadeScoreFile) {
        std::println("arcade-highscores.json not found; did you put the correct filepath in config.txt?");
        std::exit(1);
    }

    char readChar{};
    int songIndex{};

    song songs[kMaxNumSongs];

    ArcadeScoreFile >> readChar;
    while(ArcadeScoreFile && songIndex<=kMaxNumSongs) {
            for (int i=1; i<25; i++) { //magic number because guh
                ArcadeScoreFile >> readChar;
            }
            //right now, we're ready to read the first name!
            while(ArcadeScoreFile) {
                while(ArcadeScoreFile && !(readChar=='/')) { //SONG NAME grabber
                    songs[songIndex].songName=songs[songIndex].songName+readChar;
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; // discard '/'
                while(ArcadeScoreFile && !(readChar=='\\')) { //DIFFICULTY grabber
                    songs[songIndex].difficultyName=songs[songIndex].difficultyName+readChar;
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; // discard '\'
                ArcadeScoreFile >> readChar; // discard '\' part 2
                while(ArcadeScoreFile && !(readChar=='\"')) { //MODIFIER grabber
                    songs[songIndex].modName=songs[songIndex].modName+readChar;
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; // discard until ':'
                while(ArcadeScoreFile && !(readChar==':')) {
                    ArcadeScoreFile >> readChar;
                }

                // while(ArcadeScoreFile && !(readChar==',')) { //SCORE grabber
                //     songs[songIndex].score=songs[songIndex].score+readChar;
                //     ArcadeScoreFile >> readChar;
                // }
                ArcadeScoreFile >> songs[songIndex].score;
                ArcadeScoreFile >> readChar; //actaully discard the :
                while(ArcadeScoreFile && !(readChar==':')) { //discard until :
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> songs[songIndex].accuracy; //ACCURACY grabber

                discardUntilNum(ArcadeScoreFile); //MACCOMBO grabber
                ArcadeScoreFile >> songs[songIndex].maxCombo;

                //Judgement grabbing
                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Crit;

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Perf;

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Great;

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Good;

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Ok;

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Barely;

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].Miss;


                int wedontcarewhatthisisithinklmao{};
                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> wedontcarewhatthisisithinklmao; //this number i dont care about (?)

                discardUntilNum(ArcadeScoreFile);
                ArcadeScoreFile >> songs[songIndex].level; //LEVEL grabber

                while(ArcadeScoreFile && !(ArcadeScoreFile.peek()=='t' || ArcadeScoreFile.peek()=='f')) { //discard until true/false
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; //begins with 't' or 'f' for true/false
                if(readChar=='t') {
                    songs[songIndex].cleared=1;
                }
                //we have all the date from this song we need, setup for next loop
                discardUntilNum(ArcadeScoreFile); //goto a number we dont care about
                while(ArcadeScoreFile && !(readChar==':')) { //discard until :
                    ArcadeScoreFile >> readChar;
                }
                ArcadeScoreFile >> readChar; //throw the :
                ArcadeScoreFile >> readChar; //and the : before the next song
                songIndex+=1;
                if (songIndex >= kMaxNumSongs) {
                    std::println("Max number of songs reached, please increase maxSongs!");
                    std::exit(1);
                }
            }
    }
    ArcadeScoreFile.close();
    PrintSheet(songIndex, songs);
    return;
}

void discardUntilNum(std::ifstream &ArcadeScoreFile) {
    char readChar{};
    while(ArcadeScoreFile && !(isdigit(ArcadeScoreFile.peek()))) { //discard until number
        ArcadeScoreFile >> readChar;
    }
}

bool songSort(song const &one, song const &two) {
    //TODO: read config for sorting type
    if (one.accuracy != two.accuracy) {
        return one.accuracy > two.accuracy;
    }
    if (one.level != two.level) {
        return one.level > two.level;
    }
    return one.accuracy > two.accuracy;
}

void PrintSheet(int songIndex, song songs[kMaxNumSongs]) {
    std::sort(songs, songs+songIndex, &songSort);

    #if defined(__linux__)
    system("clear");
    #elif defined(_WIN32)
    system("cls");
    #endif
    std::println(" |             Song Name             | Difficulty |  Modifier  |  Score  |Accuracy|MaxCom| Crit | Perf |Great | Good |  Ok  |Barely| Miss |Level |Cleared");
    std::println(" |-----------------------------------|------------|------------|---------|--------|------|------|------|------|------|------|------|------|------|-------");

    for (int i=0; i<songIndex; i++) {
        // TODO: add actual custom conditions
        if (songs[i].cleared == false) {
            continue;
        }
        if (!(songs[i].difficultyName == "UNBEATABLE" || songs[i].difficultyName == "Star")) {
            continue;
        }
        if (songs[i].modName == "DoubleTime" || songs[i].modName == "HalfTime") {
            continue;
        }
        if (songs[i].level == 0) {
            continue;
        }

        ColorName(i,songs);
        std::print("{:^33}",songs[i].songName.substr(0, 33));

        ColorText(songs[i].difficultyName,i,songs);
        std::print("{:^10}",songs[i].difficultyName);

        ColorText(songs[i].modName,i,songs);
        std::print("{:^10}",songs[i].modName);

        ColorText(songs[i].score,i,songs);
        std::print("{:>7}",songs[i].score);

        ColorText(songs[i].accuracy,i,songs);
        std::print("{:^.4f}",songs[i].accuracy);

        ColorText(songs[i].maxCombo,i,songs);
        std::print("{:^4}",songs[i].maxCombo);

        ColorCrit(songs[i].Crit,i,songs);
        std::print("{:^4}",songs[i].Crit);

        ColorText(songs[i].Perf,i,songs);
        std::print("{:^4}",songs[i].Perf);

        ColorText(songs[i].Great,i,songs);
        std::print("{:^4}",songs[i].Great);

        ColorText(songs[i].Good,i,songs);
        std::print("{:^4}",songs[i].Good);

        ColorText(songs[i].Ok,i,songs);
        std::print("{:^4}",songs[i].Ok);

        ColorText(songs[i].Barely,i,songs);
        std::print("{:^4}",songs[i].Barely);

        ColorText(songs[i].Miss,i,songs);
        std::print("{:^4}",songs[i].Miss);

        ColorText(songs[i].level,i,songs);
        std::print("{:^4}",songs[i].level);

        if(songs[i].cleared == false) {
        ColorText(songs[i].cleared,i,songs);
        std::print("uncleared",songs[i].cleared);
        } else {
            std::print(" \033[0m|");
        }
        std::println("\033[0m");
    }

    // color debug
    // std::println("DEBUG");
    // for (int i=0; i<108; i++) {
    //     std::println("\033[{}mi:{}",i,i);
    // }
    // std::println("\033[0mlol");
}

void ResetColorAndPrintLines() {
    //std::print(" \033[0m|\033[4m");
    std::print(" \033[0m|");
    return;
}

bool OverarchingHighlighting(int i, song songs[kMaxNumSongs]) {
    if (songs[i].cleared == false) {
        std::print("\033[31m ");
        return true;
    }
    if(songs[i].Perf + songs[i].Great + songs[i].Good + songs[i].Ok + songs[i].Barely + songs[i].Miss == 0) {
        std::print("\033[102m ");
        return true;
    }
    if(songs[i].Great + songs[i].Good + songs[i].Ok + songs[i].Barely + songs[i].Miss == 0) {
        std::print("\033[43m ");
        return true;
    }
    return false;
}

void ColorName(int i, song songs[kMaxNumSongs]) {
    ResetColorAndPrintLines();
    if (songs[i].cleared == false) {
        std::print("\033[31m ");
        return;
    }
    std::print("\033[105m ");
}

void ColorText(std::string str, int i, song songs[kMaxNumSongs]) {
    ResetColorAndPrintLines();
    if (songs[i].cleared == false) {
        std::print("\033[31m ");
        return;
    }
    if (str == "UNBEATABLE") {
        std::print("\033[33m ");
        return;
    }
    if (str == "Star") {
        std::print("\033[93m ");
        return;
    }
    if (str == "DoubleTime") {
        std::print("\033[41m ");
        return;
    }
    if (str == "HalfTime") {
        std::print("\033[42m ");
        return;
    }
    if (str == "Classic") {
        std::print("\033[34m ");
        return;
    }
    std::print("\033[90m ");
    return;
}

void ColorText(int num, int i, song songs[kMaxNumSongs]) {
    ResetColorAndPrintLines();
    bool skip = OverarchingHighlighting(i, songs);
    if (skip) {
        return;
    }
    if(songs[i].Miss == 0) {
        std::print("\033[93m ");
        return;
    }
    if (num != 0) {
        std::print("\033[90m ");
        return;
    }
    if (num == 0) {
        std::print("\033[93m ");
        return;
    }
    std::print("\033[90m ");
    return;
}

void ColorCrit(int num, int i, song songs[kMaxNumSongs]) {
    ResetColorAndPrintLines();
    bool skip = false;
    if (num!=0) {
    skip = OverarchingHighlighting(i, songs);
    }
    if (skip) {
        if (num != 0 && (songs[i].Perf + songs[i].Great + songs[i].Good + songs[i].Ok + songs[i].Barely + songs[i].Miss != 0)) {
            std::print("\033[92m");
            return;
        }
        return;
    }
    if (songs[i].cleared == false) {
        std::print("\033[31m ");
        return;
    }
    if (num == 0) {
        std::print("\033[90m ");
        return;
    }
    if (num != 0) {
        std::print("\033[92m ");
        return;
    }
}

void ColorText(double num, int i, song songs[kMaxNumSongs]) {
    ResetColorAndPrintLines();
    if (songs[i].cleared == false) {
        std::print("\033[31m ");
        return;
    }
    if (num == 1.0) {
        std::print("\033[103m ");
        return;
    }
    if (num >= 0.95) {
        std::print("\033[93m ");
        return;
    }
    if (num >= 0.85) {
        std::print("\033[92m ");
        return;
    }
    if (num >= 0.75) {
        std::print("\033[94m ");
        return;
    }
    std::print("\033[90m ");
    return;
}
