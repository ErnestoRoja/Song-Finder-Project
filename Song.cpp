#include "Song.h"

// Initializes the song object with respective characteristics
void Song::initVariables(std::string name, std::string artist, float danceability, float energy, float tempo, float speechiness)
{
    this->name = name;
    this->artist = artist;
    this->danceability = danceability;
    this->energy = energy;
    this->tempo = tempo;
    this->speechiness = speechiness;
}

Song::Song()
{

}

// Constructor
Song::Song(std::string name, std::string artist, float danceability, float energy, float tempo, float speechiness)
{
    this->initVariables(name, artist, danceability, energy, tempo, speechiness);
}

std::string Song::getName()
{
    return this->name;
}

std::string Song::getArtist()
{
    return this->artist;
}

float Song::getDanceability()
{
    return this->danceability;
}

float Song::getEnergy()
{
    return this->energy;
}

float Song::getTempo()
{
    return this->tempo;
}

float Song::getSpeechiness()
{
    return this->speechiness;
}

Song::~Song()
{
    
}