#pragma once
#include "Song.h"

class Graph : private Song
{
private:
    std::unordered_map<std::string, std::vector<std::pair<Song*, float>>> graph;
    std::unordered_map<std::string, bool> visited;
    int numVertices;

public:
    Graph();
    void insertEdge(Song* song1, Song* song2, float weight);
    std::vector<Song*> getNeighbors(Song* song);
    void clearVisited();
    std::vector<std::pair<float, std::string>>  bfsTraversal(Song* song);
    std::vector<std::pair<float, std::string>>  dfsTraversal(Song* song);
    virtual ~Graph();
};

