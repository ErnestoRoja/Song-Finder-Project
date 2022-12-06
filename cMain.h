#pragma once
#include "wx/wx.h"
#include "wx/image.h"
#include "Graph.h"
class cMain : public wxFrame
{
public: 
	cMain();
	~cMain();
public:
	wxButton* bfs = nullptr;
	wxButton* dfs = nullptr;
	wxButton* clear = nullptr;
	wxTextCtrl* userInput = nullptr;
	wxListBox* recommendationList = nullptr;
	wxListBox* song1 = nullptr;
	wxListBox* song2 = nullptr;
	wxListBox* song3 = nullptr;
	wxListBox* song4 = nullptr;
	wxListBox* timer = nullptr;
	wxListBox* invalid = nullptr;
	wxPanel* panel = nullptr;
	wxStaticText* staticText = nullptr;
	wxStaticText* text2 = nullptr;
	wxStaticText* text3 = nullptr;
	std::vector<std::pair<float, std::string>> recommendedSong;
	Graph graph;
	std::unordered_map<std::string, Song*> map;
	std::unordered_map<std::string, std::pair<float, float>> traversed;
	std::queue<Song*> queue;
	Song* sourceSong = nullptr;

	void BFSButtonClicked(wxCommandEvent& evt);
	void DFSButtonClicked(wxCommandEvent& evt);
	void ClearButtonClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

