/////////////////////////////////////////////////////////////////////////////
// Name:        rppg_data_collection.hpp
// Purpose:     GUI for covid testing and data collection
// Author:      Rohith Yenumula
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#pragma once

#include "animation.hpp"

using namespace animation;

namespace rppg_data_collection
{

	// ----------------------------------------------------------------------------
	// constants
	// ----------------------------------------------------------------------------

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

	// ----------------------------------------------------------------------------
	// enums
	// ----------------------------------------------------------------------------

	enum
	{
		// file menu
		TabOrder_Quit = wxID_EXIT,
		TabOrder_About = wxID_ABOUT,
	};

	enum
	{
		StatusPane_Default,
		StatusPane_Focus,
		StatusPane_Max
	};

	enum
	{
		wxGOBACK = 1001,
		wxMAIN_TO_CONSENT = 101,
		wxCONSENT_TO_HAND = 102,
		wxRIGHT_HAND = 103,
		wxLEFT_HAND = 104,
		wxScreen1_To_Screen2 = 105,
		wxScreen2_To_Screen3 = 106
	};

	// ----------------------------------------------------------------------------
	// declarations for each wxWidgets GUI window
	// ----------------------------------------------------------------------------

	// the main application class
	class MyApp : public wxApp //MyApp is the class for the application
	{
	public:
		virtual bool OnInit();
	};

	class MyFrame : public wxFrame
	{
	public:
		MyFrame();
		void OnExit(wxCommandEvent &event);

	private:
		void OnAbout(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnNotebookChange(wxCommandEvent &event);
		wxPanel *m_panel;
		wxDECLARE_EVENT_TABLE();
	};

	class UserConsent : public wxFrame
	{
	public:
		UserConsent(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size);
		void GoBack(wxCommandEvent &event);
		void ToHand(wxCommandEvent &event);

	private:
		wxDECLARE_EVENT_TABLE();

	protected:
		wxStaticText *m_staticText1;
		wxStaticText *m_staticText2;
		wxStaticText *m_staticText3;
	};

	class ChooseHand : public wxFrame
	{
	public:
		ChooseHand(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size);
		void ToVideo1(wxCommandEvent &event);
		void GoBack(wxCommandEvent &event);

	private:
		wxDECLARE_EVENT_TABLE();

	protected:
		wxStaticText *m_staticText1;
		wxStaticText *m_staticText2;
		wxStaticText *m_staticText3;
		wxStaticText *m_staticText4;
	};

	class VideoScreen1 : public wxFrame
	{
	public:
		VideoScreen1(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size);
		void ToVideo2(wxCommandEvent &event);
		void GoBack(wxCommandEvent &event);
		bool CameraOpen();
		bool CameraClose();

	private:
		wxDECLARE_EVENT_TABLE();

	protected:
		wxStaticText *m_staticText1;
		wxStaticText *m_staticText2;
		wxStaticText *m_staticText3;
		wxStaticText *m_staticText4;
		wxStaticText *m_staticText5;
		wxStaticText *m_staticText6;
	};

	class VideoScreen2 : public wxFrame
	{
	public:
		VideoScreen2(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size);
		void ToVideoFinal(wxCommandEvent &event);
		bool BouncingBall();
		bool CountDown();
		bool CameraSave();
		bool CountDownCamera();

	private:
		wxDECLARE_EVENT_TABLE();

	protected:
		wxStaticText *m_staticText1;
		wxStaticText *m_staticText2;
		wxStaticText *m_staticText3;
		wxStaticText *m_staticText4;
		wxStaticText *m_staticText5;
		wxStaticText *m_staticText6;
		wxStaticText *m_staticText7;
		wxStaticText *m_staticText8;
	};

	class VideoScreenFinal : public wxFrame
	{
	public:
		VideoScreenFinal(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size);
		void OnQuit2(wxCommandEvent &event);

	private:
		wxDECLARE_EVENT_TABLE();
	};

	// and the panel taking up MyFrame client area
	class MyPanel : public wxPanel
	{
	public:
		MyPanel(wxWindow *parent);

	private:
		//	wxWindow (wxWindow *parent,  wxWindowID id, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxPanelNameStr)
		wxWindow *HomeScreen(wxWindow *parent);

	protected:
		wxStaticText *m_staticText1;
		wxStaticText *m_staticText2;
		wxStaticText *m_staticText3;
	};

} // namespace rppg_data_collection
