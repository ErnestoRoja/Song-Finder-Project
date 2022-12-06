#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>
#include <chrono>

class Song
{
private:
    std::string name;
    std::string artist;
    float danceability;
    float energy;
    float tempo;
    float speechiness;

    // Private functions
    void initVariables(std::string name, std::string artist, float danceability, float energy, float tempo, float speechiness);

public:
    Song();
    Song(std::string name, std::string artist,float danceability, float energy, float tempo, float speechiness);
    virtual ~Song();

    // Public accessors
    std::string getName();
    std::string getArtist();
    float getDanceability();
    float getEnergy();
    float getTempo();
    float getSpeechiness();    
};

