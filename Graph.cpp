#include "Graph.h"
 
// Constructor
Graph::Graph()
{
    this->numVertices = 1;
}

// Inserts a weighted edge between song1 and song2
void Graph::insertEdge(Song* song1, Song* song2, float weight)
{
    this->graph[song1->getName()].push_back(std::make_pair(song2, weight));
    this->graph[song2->getName()].push_back(std::make_pair(song1, weight));

    if (graph.find(song2->getName()) == graph.end())
    {
        this->graph[song2->getName()] = {};
    }
    if (graph.find(song1->getName()) == graph.end())
    {
        this->graph[song1->getName()] = {};
    }
    numVertices++;
}

// Returns the neighbors of a song
std::vector<Song*> Graph::getNeighbors(Song* song)
{
    std::vector<Song*> neighbors;
    for (auto i : graph[song->getName()])
    {
        neighbors.push_back(i.first);
    }
    return neighbors;
}

// Clears the vector that checks if a vertex is visited
void Graph::clearVisited()
{
    this->visited.clear();
}

// Performs a BFS traversal on song "source"
std::vector<std::pair<float, std::string>> Graph::bfsTraversal(Song* source)
{
    std::vector<std::pair<float, std::string>> bestSong;
    float sourceStats = source->getDanceability() + source->getEnergy() + source->getSpeechiness() + source->getTempo();
    float similarityDifference = 0.f;
    // Creates a queue
    float minimumDifference = 1000.f;
    std::queue<Song*> queue;

    // Helps keep track of the index in the vector of best songs
    int counter = 0;
    // Set the source to visited and push onto the queue
    visited[source->getName()] = true;
    queue.push(source);

    while (!queue.empty())
    {
        // First element in the queue (current element being processed)
        Song* currentSong = queue.front();
        queue.pop();

        // Checks the neighbors of the current element and adds them to the queue and to the list of possible songs if similar to the source song
        for (auto i : graph[currentSong->getName()])
        {
            if (visited.find(i.first->getName()) == visited.end())
            {
                float neighborStats = i.first->getDanceability() + i.first->getEnergy() + i.first->getSpeechiness() + i.first->getTempo();
                similarityDifference = std::abs(sourceStats - neighborStats);

                // Maintains the vector of best possible songs with a size of 4.
                if (bestSong.size() < 4)
                {
                    bestSong.push_back(std::make_pair(similarityDifference, i.first->getName()));
                } 
                // Actively updates the best possible songs to be recommended
                else if (bestSong.size() == 4)
                {
                    float relativeMax = 0.f;
                    int temp = 0;
                    for (auto j : bestSong)
                    {      
                        if (j.first > relativeMax)
                        {
                            temp = counter;
                            relativeMax = j.first;
                        }
                        counter++;
                    }
                    if (similarityDifference < relativeMax)
                    {
                        bestSong[temp].first = similarityDifference;
                        bestSong[temp].second = i.first->getName();
                    }
                }
                counter = 0;
                visited[i.first->getName()] = true;
                queue.push(i.first);
            }
        }
    }

    return bestSong;
}

std::vector<std::pair<float, std::string>> Graph::dfsTraversal(Song* source)
{
    std::vector<std::pair<float, std::string>> bestSong;
    float sourceStats = source->getDanceability() + source->getEnergy() + source->getSpeechiness() + source->getTempo();
    float similarityDifference = 0.f;
    // Creates a stack
    float minimumDifference = INT_MAX;
    std::stack<Song*> stack;

    // Helps keep track of the index in the vector of best songs
    int counter = 0;
    // Set the source to visited and push onto the stack
    visited[source->getName()] = true;
    stack.push(source);

    while (!stack.empty())
    {
        // First element of the stack (current element being processed)
        Song* currentSong = stack.top();
        stack.pop();

        // Checks the neighbors of the current element and adds them to the stack and to the list of possible songs if similar to the source song
        for (auto i : graph[currentSong->getName()])
        {
            if (visited.find(i.first->getName()) == visited.end())
            {
                float neighborStats = i.first->getDanceability() + i.first->getEnergy() + i.first->getSpeechiness() + i.first->getTempo();
                similarityDifference = std::abs(sourceStats - neighborStats);

                // Maintains the vector of best possible songs with a size of 4.
                if (bestSong.size() < 4)
                {
                    bestSong.push_back(std::make_pair(similarityDifference, i.first->getName()));
                } 
                // Actively updates the best possible songs to be recommended
                else if (bestSong.size() == 4)
                {
                    float relativeMax = 0.f;
                    int temp = 0;
                    for (auto j : bestSong)
                    {      
                        if (j.first > relativeMax)
                        {
                            temp = counter;
                            relativeMax = j.first;
                        }
                        counter++;
                    }
                    if (similarityDifference < relativeMax)
                    {
                        bestSong[temp].first = similarityDifference;
                        bestSong[temp].second = i.first->getName();
                    }
                }
                counter = 0;
                visited[i.first->getName()] = true;
                stack.push(i.first);
            }
        }
    }
    return bestSong;
}

Graph::~Graph()
{
    graph.clear();
}