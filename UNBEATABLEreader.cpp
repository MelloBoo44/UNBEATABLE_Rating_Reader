//notes for me lmao
//TODO: replace songnames with the one from the very cool and epic json

#include <iostream>
#include <print>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>

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
    double rating{};
};

struct config {
    std::string filename{};
    bool showUncleared{};
    bool showTop25Uncleared{};
    bool showDoubletime{};
    bool showHalfTime{};
    bool showCustom{};

    bool showStar{};
    bool showUnbeatable{};
    bool showOtherDiff{};
    std::string sort[10]{};
    int sortArraySize{0};
};

const int kMaxNumSongs = 1000;

void doConfigStuff(config &settings);
void discardUntilHashtag(std::ifstream &configfile);
void ReadSheet(std::ifstream &ArcadeScoreFile, config settings, bool &top25donedisplaying);
void discardUntilNum(std::ifstream &);
void PrintSheet(int songIndex, song songs[kMaxNumSongs], config settings, bool &top25donedisplaying);
void ColorText(std::string, int i, song songs[kMaxNumSongs]);
void ColorName(int i, song songs[kMaxNumSongs]);
void ColorText(int, int i, song songs[kMaxNumSongs]);
void ColorCrit(int, int i, song songs[kMaxNumSongs]);
void ColorText(double, int i, song songs[kMaxNumSongs]);
void ColorRating(int i, song songs[kMaxNumSongs], int &ratingSkipBy, bool &top25donedisplaying);
bool OverarchingHighlighting(int i, song songs[kMaxNumSongs]);
void ResetColorAndPrintLines();
bool songSort(song const &one, song const &two);

config settings{};

int main() {

    doConfigStuff(settings);

    //read in the arcade highscores file found in config.txt
    std::ifstream ArcadeScoreFile;
    bool top25donedisplaying = false;
    ReadSheet(ArcadeScoreFile, settings, top25donedisplaying);

    // i tried to convert these to seconds since epoch time but i suck :p; i am going to DIRECTLY COMPARE whatever these are and NOONE can stop me
    auto timeLastModified = std::filesystem::last_write_time(settings.filename);
    auto timeLastModifiedChecking = std::filesystem::last_write_time(settings.filename);
    while(true) {
        //stuff to check if the file changed
        timeLastModifiedChecking = std::filesystem::last_write_time(settings.filename);
        if( !(timeLastModified==timeLastModifiedChecking) ) {
            timeLastModified=timeLastModifiedChecking;
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // i put this here because uhh iuhmm yeah?
            //we can actually get to work now!
            ReadSheet(ArcadeScoreFile, settings, top25donedisplaying);

        }
    }

    return 0;

}

void doConfigStuff(config &settings) {
    std::ifstream configfile;
    configfile.open("config.txt");
    if(!configfile) {
        std::println("config.txt not found. Please create it and put the filepath to arcade-highscores.json inside.");
        std::exit(1);
    }
    discardUntilHashtag(configfile);
    configfile.ignore(10);
    std::getline(configfile,settings.filename);
    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showUncleared;

    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile.ignore(2); // ignore the 25 in "TOP25UNCLEARED"
    discardUntilNum(configfile); configfile >> settings.showTop25Uncleared;

    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showDoubletime;
    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showHalfTime;
    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showCustom;
    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showStar;
    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showUnbeatable;
    discardUntilHashtag(configfile); discardUntilNum(configfile); configfile >> settings.showOtherDiff;

    //im sorry
    discardUntilHashtag(configfile);
    configfile.ignore(7);
    int i=0;
    std::getline(configfile, settings.sort[i], ',');
    while((configfile.peek()==' ')) {
        i++;
        configfile.ignore();
        std::getline(configfile, settings.sort[i], ',');
        if(configfile.peek()=='\n') {
            break;
        }
    }
    settings.sort[i] = settings.sort[i].substr(0,settings.sort[i].length()-2) + '\0'; //lmao
    settings.sortArraySize = i;

    configfile.close();
}
//TODO: Color top 25 properly.
void ReadSheet(std::ifstream &ArcadeScoreFile, config settings, bool &top25donedisplaying) {
    ArcadeScoreFile.open(settings.filename);
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

                int BONUS{};
                if (songs[songIndex].accuracy >= 0.90) {
                    BONUS = 25;
                } else if (songs[songIndex].accuracy >= 0.85) {
                    BONUS = 20;
                } else if (songs[songIndex].accuracy >= 0.80) {
                    BONUS = 15;
                } else if (songs[songIndex].accuracy >= 0.70) {
                    BONUS = 12;
                } else if (songs[songIndex].accuracy >= 0.50) {
                    BONUS = 10;
                } else {
                    BONUS = 0;
                }
                songs[songIndex].rating = (static_cast<double>(songs[songIndex].level)*((std::pow(((songs[songIndex].accuracy*100.0) - 50.0),1.12))+static_cast<double>(BONUS)))/5625.0;
                if(BONUS==0 || songs[songIndex].modName == "HalfTime") {
                    songs[songIndex].rating = 0.0;
                }

                songIndex+=1;
                if (songIndex >= kMaxNumSongs) {
                    std::println("Max number of songs reached, please increase maxSongs!");
                    std::exit(1);
                }
            }
    }
    ArcadeScoreFile.close();
    PrintSheet(songIndex, songs, settings, top25donedisplaying);
    return;
}

void discardUntilNum(std::ifstream &file) {
    char readChar{};
    while(file && !(isdigit(file.peek()))) { //discard until number
        file >> readChar;
    }
}

void discardUntilHashtag(std::ifstream &configfile) {
    char readChar{};
    while(configfile && !(readChar=='#')) { //discard until #
        configfile >> readChar;
    }
}

bool songSort(song const &one, song const &two) {
    //TODO: read config for sorting type
    // for (int i=0; i<settings.sortArraySize; i++) {
    //     std::println("im sorting but not");
    // }
    // return one.score > two.score;
    if (one.rating != two.rating) {
        return one.rating > two.rating;
    }
    if (one.accuracy != two.accuracy) {
        return one.accuracy > two.accuracy;
    }
    return one.accuracy > two.accuracy;
}

void PrintSheet(int songIndex, song songs[kMaxNumSongs], config settings, bool &top25donedisplaying) {
    std::sort(songs, songs+songIndex, &songSort);

    #if defined(__linux__)
    system("clear");
    #elif defined(_WIN32)
    system("cls");
    #endif
    std::println(" PLEASE NOTE: The 'top 25 rating thing' actually just applies colors to the first 25 ratings, reguardless of sorting.\n\n");
    std::println("     |             Song Name             | Difficulty |  Modifier  |  Score  |Accuracy|MaxCom| Crit | Perf |Great | Good |  Ok  |Barely| Miss |Level | Rating |Cleared");
    std::println("     |-----------------------------------|------------|------------|---------|--------|------|------|------|------|------|------|------|------|------|--------|-------");

    int ratingSkipBy{};
    for (int i=0; i<songIndex; i++) {
        // TODO: add actual custom conditions
        // TODO: implement skipping customs
        if ((songs[i].cleared == false) && top25donedisplaying && (settings.showTop25Uncleared==1) ) {
            continue;
        }
        if (songs[i].cleared == false && !(settings.showUncleared==1)) {
            continue;
        }
        if ((!(songs[i].difficultyName == "UNBEATABLE" || songs[i].difficultyName == "Star")) && !settings.showOtherDiff) {
            continue;
        }
        if (songs[i].difficultyName == "UNBEATABLE" && !settings.showUnbeatable) {
            continue;
        }
        if (songs[i].difficultyName == "Star" && !settings.showStar) {
            continue;
        }
        // if (songs[i].modName == "DoubleTime" || songs[i].modName == "HalfTime") {
        //     continue;
        // }
        if (songs[i].modName == "HalfTime" && !settings.showHalfTime) {
            continue;
        }
        if (songs[i].modName == "DoubleTime" && !settings.showDoubletime) {
            continue;
        }
        if ((songs[i].level == 0|| songs[i].songName.substr(0, 7) == "CUSTOM_" || songs[i].songName.substr(0, 10) == "__WSCUSTOM") && settings.showCustom){ //TODO: Can be verified by matching to official json
            continue;
        }

        std::print("{:>4}",i+1);

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

        //ColorRating(i,songs,ratingSkipBy,top25donedisplaying);
        ratingSkipBy=ratingSkipBy;
        ResetColorAndPrintLines();
        std::print("\033[90m "); //TODO: do better..

        std::print("{:^.4f}",songs[i].rating);

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

void ColorRating(int i, song songs[kMaxNumSongs], int &ratingSkipBy, bool &top25donedisplaying) {
    ResetColorAndPrintLines();
    if (songs[i].cleared == false && top25donedisplaying) {
        std::print("\033[31m ");
        return;
    }
    if (1+i-ratingSkipBy>=25) { //i love bad code its my favorite
            top25donedisplaying = true;
    }
    if (songs[i].modName == "HalfTime") {
        std::print("\033[90m ");
        ratingSkipBy+=1;
        return;
    }
    if (i-ratingSkipBy<5) {
        std::print("\033[46m ");
        return;
    }
    if (i-ratingSkipBy<10) {
        std::print("\033[45m ");
        return;
    }
    if (i-ratingSkipBy<15) {
        std::print("\033[46m ");
        return;
    }
    if (i-ratingSkipBy<20) {
        std::print("\033[45m ");
        return;
    }
    if (i-ratingSkipBy<25) {
        std::print("\033[46m ");
        return;
    }

    std::print("\033[90m ");
    return;
}
