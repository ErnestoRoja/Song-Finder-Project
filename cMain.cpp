#include "cMain.h"
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
    EVT_BUTTON(10002, DFSButtonClicked)
	EVT_BUTTON(10001, BFSButtonClicked)
	EVT_BUTTON(10000, ClearButtonClicked)
wxEND_EVENT_TABLE()

// Concatenates the characteristics of a song to 3 decimal places.
float updateValue(std::string s)
{
    int pos = s.find('.');
    std::string leading = s.substr(0, pos + 1);
    std::string decimals = s.substr(pos + 1, s.length() - pos);

    if (decimals.length() >= 3)
    {
        std::string appended = decimals.substr(0, 3);
        return std::stof(leading.append(appended));
    }
    return std::stof(s);
}

// Calculates the difference in characteristics between two songs.
float calculateWeight(Song* song1, Song* song2)
{
    float song1Sum = song1->getDanceability() + song1->getEnergy() + song1->getSpeechiness() + song1->getTempo();
    float song2Sum = song2->getDanceability() + song2->getEnergy() + song2->getSpeechiness() + song2->getTempo();
    return std::abs(song1Sum - song2Sum);
}

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Song Finder by: THE MUSIC MAGICIANS", wxPoint(30, 30), wxSize(1400, 800))
{
    // Initiates all the GUI elements
	panel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(1400, 800));
	panel->SetBackgroundColour(wxColor(186, 132, 84));
	staticText = new wxStaticText(panel , wxID_ANY, "In the box below, Enter the song you like.", wxPoint(500, 50));
    text3 = new wxStaticText(panel, wxID_ANY, "Execution Time", wxPoint(650, 407));
    text3->SetFont(text3->GetFont().Scale(1.25));
	text2 = new wxStaticText(panel, wxID_ANY, "Songs we recommend based on the song you like:", wxPoint(500, 190));
    text2->SetFont(text2->GetFont().Scale(1.4));
	bfs = new wxButton(panel, 10001, "BFS", wxPoint(810, 70), wxSize(100, 45));
	dfs = new wxButton(panel, 10002, "DFS", wxPoint(810, 120), wxSize(100, 45));
	clear = new wxButton(panel, 10000, "Clear", wxPoint(720, 49), wxSize(80, 20));
	userInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(500, 70), wxSize(300, 95));
    userInput->SetFont(userInput->GetFont().Scale(1.5));
	recommendationList = new wxListBox(panel, wxID_ANY, wxPoint(500, 220), wxSize(410, 180)); 
    recommendationList->SetFont(recommendationList->GetFont().Scale(1.5));
    song1 = new wxListBox(panel, wxID_ANY, wxPoint(435, 220), wxSize(60, 30));
    song2 = new wxListBox(panel, wxID_ANY, wxPoint(435, 270), wxSize(60, 30));
    song3 = new wxListBox(panel, wxID_ANY, wxPoint(435, 320), wxSize(60, 30));
    song4 = new wxListBox(panel, wxID_ANY, wxPoint(435, 370), wxSize(60, 30));
    timer = new wxListBox(panel, wxID_ANY, wxPoint(600, 430), wxSize(200, 50));
    timer->SetFont(timer->GetFont().Scale(1.80));
    invalid = new wxListBox(panel, wxID_ANY, wxPoint(390, 70), wxSize(100,40));
    invalid->SetFont(invalid->GetFont().Scale(1.75));

    // Beginning of the CSV file processing algorithm.
    std::srand((unsigned)time(0));
    bool invalidData = false;
    // Keeps track of the commas in each line of the CSV file
    int counter = 0;
    int songsAdded = 0;
    std::ifstream fileIn("test1.csv", std::ios_base::binary);

    std::string name,artist;
    float danceability, energy, tempo, speechiness;

    if (fileIn.is_open())
    {
        std::string line;
        const char delim = ',';
        while (std::getline(fileIn, line))
        {
            std::stringstream ss(line);
            std::string s;
            while (std::getline(ss, s, delim))
            {
                if (s[0] == '"')
                {
                    invalidData = true;
                    break;
                }
                // Stores the information at the required commas.
                switch (++counter)
                {
                case 2:
                    name = s;
                    break;
                case 5:
                    artist = s;
                    break;
                case 10:
                    danceability = updateValue(s);
                    break;
                case 11:
                    energy = updateValue(s);
                    break;
                case 15:
                    speechiness = updateValue(s);
                    break;
                case 20:
                    tempo = updateValue(s);
                    break;
                default:
                    break;
                }
            }
            bool unique = true;
            if (!invalidData)
            {
                // Checks if the song to be added has already been added
                if (map.find(name) != map.end())
                    unique = false;
                // If the song is unique, we add it to the map using the previously stored values
                if (unique)
                {
                    Song* songToAdd = new Song(name, artist, danceability, energy, tempo, speechiness);
                    map[name] = songToAdd;

                    // Pushes a song (V) to be added to a queue and adds three neighbors to (V)
                    queue.push(songToAdd);

                    if (queue.size() > 1)
                    {
                        float weight = calculateWeight(queue.front(), songToAdd);

                        if (graph.getNeighbors(queue.front()).size() < 3)     
                            graph.insertEdge(queue.front(), songToAdd, weight);                      
                        else
                        {
                            queue.pop();
                            graph.insertEdge(queue.front(), songToAdd, weight);
                        }
                    }
                    invalidData = false;
                }
            }
            else
                invalidData = false;

            counter = 0;
        }
        fileIn.close();
    }
}

cMain::~cMain()
{
	delete this->bfs;
	delete this->dfs;
	delete this->clear;
	delete this->userInput;
	delete this->recommendationList;
}

void cMain::BFSButtonClicked(wxCommandEvent& evt)
{
    song1->Clear();
    song2->Clear();
    song3->Clear();
    song4->Clear();
    timer->Clear();
    recommendationList->Clear();
    invalid->Clear();

    std::string input = std::string(userInput->GetValue().mb_str());

    // Checks if the song inputed exists in the dataset.
    if (map.find(input) == map.end())
        invalid->AppendString("Invalid");
    else
    {
        invalid->AppendString("Valid");
        sourceSong = map[input];

        // Calcuates the execution time of the BFS traversal
        auto BFS_start = std::chrono::high_resolution_clock::now();
        recommendedSong = graph.bfsTraversal(sourceSong);
        auto BFS_end = std::chrono::high_resolution_clock::now();
        auto BFS_duration = std::chrono::duration_cast<std::chrono::milliseconds>(BFS_end - BFS_start);

        wxString time;
        time << BFS_duration.count();
        timer->AppendString(time + " milliseconds");

        // Outputs the best possible songs in ascending order of similarity.
        std::sort(recommendedSong.begin(), recommendedSong.end());
        for (int i = 0; i < recommendedSong.size(); i++)
        {
            wxString recArtist(map[recommendedSong[i].second]->getArtist().c_str(), wxConvUTF8);
            wxString recSong(recommendedSong[i].second.c_str(), wxConvUTF8);

            recommendationList->AppendString(recSong + " -> " + recArtist);
            recommendationList->AppendString("");

            wxString similiarity;
            similiarity << recommendedSong[i].first;

            switch(i)
            {
            case 0:
                song1->AppendString(similiarity);
                break;
            case 1:
                song2->AppendString(similiarity);
                break;
            case 2:
                song3->AppendString(similiarity);
                break;
            case 3:
                song4->AppendString(similiarity);
                break;
            default:
                break;
            }
        }
    }
    // Clear the data
    graph.clearVisited();
    recommendedSong.clear();
	evt.Skip();
}

void cMain::DFSButtonClicked(wxCommandEvent& evt)
{
    song1->Clear();
    song2->Clear();
    song3->Clear();
    song4->Clear();
    timer->Clear();
    recommendationList->Clear();
    invalid->Clear();

    std::string input = std::string(userInput->GetValue().mb_str());

    // Checks if the song inputed exists in the dataset.
    if (map.find(input) == map.end())
        invalid->AppendString("Invalid"); 
    else
    {
        invalid->AppendString("Valid");
        sourceSong = map[input];

        // Calcuates the execution time of the BFS traversal
        auto DFS_start = std::chrono::high_resolution_clock::now();
        recommendedSong = graph.dfsTraversal(sourceSong);
        auto DFS_end = std::chrono::high_resolution_clock::now();
        auto DFS_duration = std::chrono::duration_cast<std::chrono::milliseconds>(DFS_end - DFS_start);
        
        wxString time;
        time << DFS_duration.count();
        timer->AppendString(time + " milliseconds");

        // Outputs the best possible songs in ascending order of similarity.
        std::sort(recommendedSong.begin(), recommendedSong.end());
        for (int i = 0; i < recommendedSong.size(); i++)
        {
            wxString recArtist(map[recommendedSong[i].second]->getArtist().c_str(), wxConvUTF8);
            wxString recSong(recommendedSong[i].second.c_str(), wxConvUTF8);

            recommendationList->AppendString(recSong + " -> " + recArtist);
            recommendationList->AppendString("");

            wxString similiarity;
            similiarity << recommendedSong[i].first;

            switch(i)
            {
            case 0:
                song1->AppendString(similiarity);
                break;
            case 1:
                song2->AppendString(similiarity);
                break;
            case 2:
                song3->AppendString(similiarity);
                break;
            case 3:
                song4->AppendString(similiarity);
                break;
            default:
                break;
            }
        }
    }
    // Clear the data
    graph.clearVisited();
    recommendedSong.clear();
    evt.Skip();
}

void cMain::ClearButtonClicked(wxCommandEvent& evt)
{
	userInput->Clear();
    recommendationList->Clear();
    song1->Clear();
    song2->Clear();
    song3->Clear();
    song4->Clear();
    timer->Clear();
    invalid->Clear();
	evt.Skip();
}
