/////////////////////////////////////////////////////////////////////////////
// Name:        rppg_data_collection.cpp
// Purpose:     GUI for covid testing and data collection
// Author:      Rohith Yenumula
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "rppg_data_collection.hpp"

#include "json.hpp"
#include <fstream>

namespace rppg_data_collection
{

    // ============================================================================
    // implementation
    // ============================================================================
    //
    // ----------------------------------------------------------------------------
    // MyApp
    // ----------------------------------------------------------------------------

    bool MyApp::OnInit()
    {
        if (!wxApp::OnInit())
            return false;

        MyFrame *frame = new MyFrame; //stop using pointers after 2011
        frame->Show(true);
        //new Data Collection UI screen

        //put everything in config file: json structure to encode parameters

        //parameter name:value fieldname:string for ex.

        return true;
    }

    // ----------------------------------------------------------------------------
    // MyFrame
    // ----------------------------------------------------------------------------

    BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(TabOrder_Quit, MyFrame::OnQuit) //TabOrder_Quit = wxID_EXIT,
    EVT_MENU(TabOrder_About, MyFrame::OnAbout)
    EVT_BUTTON(wxID_EXIT, MyFrame::OnQuit)
    EVT_BUTTON(wxMAIN_TO_CONSENT, MyFrame::OnNotebookChange) //distinct ID 101
    END_EVENT_TABLE()

    BEGIN_EVENT_TABLE(UserConsent, wxFrame)
    EVT_BUTTON(wxGOBACK, UserConsent::GoBack)
    EVT_BUTTON(wxCONSENT_TO_HAND, UserConsent::ToHand)
    END_EVENT_TABLE()

    BEGIN_EVENT_TABLE(ChooseHand, wxFrame)
    EVT_BUTTON(wxLEFT_HAND, ChooseHand::ToVideo1)
    EVT_BUTTON(wxRIGHT_HAND, ChooseHand::ToVideo1)
    EVT_BUTTON(wxGOBACK, ChooseHand::GoBack)
    END_EVENT_TABLE()

    BEGIN_EVENT_TABLE(VideoScreen1, wxFrame)
    EVT_BUTTON(wxGOBACK, VideoScreen1::GoBack)
    EVT_BUTTON(wxScreen1_To_Screen2, VideoScreen1::ToVideo2)
    END_EVENT_TABLE()

    BEGIN_EVENT_TABLE(VideoScreen2, wxFrame)
    EVT_BUTTON(wxScreen2_To_Screen3, VideoScreen2::ToVideoFinal)
    END_EVENT_TABLE()

    BEGIN_EVENT_TABLE(VideoScreenFinal, wxFrame)
    EVT_BUTTON(wxID_EXIT, VideoScreenFinal::OnQuit2)
    END_EVENT_TABLE()

    void MyFrame::OnNotebookChange(wxCommandEvent &WXUNUSED(event))
    {
        UserConsent *frame = new UserConsent(wxID_ANY, "User Consent Request", wxDefaultPosition, wxSize(1000, 800));
        MyFrame::~MyFrame();
        frame->Show(true);
    }

    void UserConsent::GoBack(wxCommandEvent &WXUNUSED(event))
    {
        MyFrame *frame = new MyFrame;
        UserConsent::~UserConsent();
        frame->Show(true);
    }

    void UserConsent::ToHand(wxCommandEvent &WXUNUSED(event))
    {
        ChooseHand *frame = new ChooseHand(wxID_ANY, "Choose your Hand", wxDefaultPosition, wxSize(1000, 800));
        UserConsent::~UserConsent();
        frame->Show(true);
    }

    void ChooseHand::ToVideo1(wxCommandEvent &WXUNUSED(event))
    {
        VideoScreen1 *frame = new VideoScreen1(wxID_ANY, "Video Screen 1", wxDefaultPosition, wxSize(1000, 800));
        ChooseHand::~ChooseHand();
        frame->Show(true);
    }

    void ChooseHand::GoBack(wxCommandEvent &WXUNUSED(event))
    {
        MyFrame *frame = new MyFrame;
        ChooseHand::~ChooseHand();
        frame->Show(true);
    }

    void VideoScreen1::ToVideo2(wxCommandEvent &WXUNUSED(event))
    {
        VideoScreen2 *frame = new VideoScreen2(wxID_ANY, "Video Screen 2", wxDefaultPosition, wxSize(1000, 800));
        VideoScreen1::~VideoScreen1();
        VideoScreen1::CameraOpen();
        VideoScreen1::CameraClose();
        frame->Show(true);
    }

    void VideoScreen1::GoBack(wxCommandEvent &WXUNUSED(event))
    {
        MyFrame *frame = new MyFrame;
        VideoScreen1::~VideoScreen1();
        frame->Show(true);
    }

    void VideoScreen2::ToVideoFinal(wxCommandEvent &WXUNUSED(event))
    {
        VideoScreenFinal *frame = new VideoScreenFinal(wxID_ANY, "Thank you!", wxDefaultPosition, wxSize(1000, 800));
        VideoScreen2::~VideoScreen2();

        VideoScreen2::CountDown(); //if this is commented out, can access end screen of GUI app

        VideoScreen2::CameraSave();
        VideoScreen2::BouncingBall();

        frame->Show(true);
    }

    void VideoScreenFinal::OnQuit2(wxCommandEvent &WXUNUSED(event))
    {
        Close(true);
    }

    void MyFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
    {
        Close(true);
    }

    void MyFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
    {
        wxMessageBox(wxT("A UMD video data collection app \n(c) 2020 Rohith Yenumula"),
                     wxT("About our app"), wxOK, this);
    }

    MyFrame::MyFrame()
        : wxFrame(NULL, wxID_ANY, wxT("Video Physio App"),
                  wxDefaultPosition, wxSize(1000, 800))
    {
        //SetIcon(wxICON(sample));

        wxMenu *menuFile = new wxMenu;
        menuFile->Append(TabOrder_About);
        menuFile->AppendSeparator();
        menuFile->Append(TabOrder_Quit);

        wxMenu *menuNav = new wxMenu;

        wxMenuBar *mbar = new wxMenuBar;

        SetMenuBar(mbar);

        m_panel = new MyPanel(this);

        CreateStatusBar(StatusPane_Max);
    }

    // ----------------------------------------------------------------------------
    // MyPanel
    // ----------------------------------------------------------------------------

    MyPanel::MyPanel(wxWindow *parent)
        : wxPanel(parent, 12)
    {
        wxNotebook *notebook = new wxNotebook(this, wxID_ANY); //wxNotebook::wxNotebook ( )
        notebook->AddPage(HomeScreen(notebook), "");           //wxNotebook::AddPage(wxWindow* page,const wxString & text)

        wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
        sizerV->Add(notebook, wxSizerFlags(1).Expand());

        SetSizerAndFit(sizerV);
    }

    wxWindow *MyPanel::HomeScreen(wxWindow *parent)
    {
        wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();

        wxPanel *page = new wxPanel(parent);
        wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1 = new wxBoxSizer(wxVERTICAL); //wxBoxSizer::wxBoxSizer(int orient) is the constructor
        wxSizer *sizerPage2 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage3 = new wxBoxSizer(wxVERTICAL);

        m_staticText1 = new wxStaticText(page, 22, wxT("Welcome to the UMD video data collection"), wxPoint(140, 240), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText1->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText1, flagsBorder);

        m_staticText2 = new wxStaticText(page, wxID_ANY, wxT("app. Tap/click 'Continue' to begin."), wxPoint(140, 265), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText2->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText2, flagsBorder);

        m_staticText3 = new wxStaticText(page, wxID_ANY, wxT("To quit, tap/click 'Quit'."), wxPoint(140, 290), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText3->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText3, flagsBorder);

        //sizerPage2->Add(new wxButton(page, wxMAIN_TO_CONSENT, wxT("Continue"), wxPoint( 400,300 ), wxDefaultSize, 0), flagsBorder); //distinct ID 101

        wxButton *wxButton1 = new wxButton(page, wxMAIN_TO_CONSENT, wxT("Continue"), wxPoint(400, 335), wxDefaultSize, 0);
        sizerPage2->Add(wxButton1, flagsBorder);
        wxButton1->SetBackgroundColour(wxColour(99, 110, 114));

        wxButton *wxButton2 = new wxButton(page, wxID_EXIT, wxT("Quit"), wxPoint(490, 335), wxDefaultSize, 0);
        sizerPage2->Add(wxButton2, flagsBorder);
        wxButton2->SetBackgroundColour(wxColour(99, 110, 114));

        page->SetSizer(sizerPage);
        return page;
    }

    UserConsent::UserConsent(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();
        wxPanel *page = new wxPanel(this);
        wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1b = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage3 = new wxBoxSizer(wxVERTICAL);

        m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("This experiment will gather videos of your hand and face to estimate certain biodata. "), wxPoint(90, 240), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText1->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText1, flagsBorder);

        m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("If you consent to participating in this experiment, tap/click 'I Agree'. If you do not"), wxPoint(100, 265), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText2->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1a->Add(m_staticText2, flagsBorder);

        m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("consent, tap/click 'I Do Not Agree' to return to the home screen."), wxPoint(130, 290), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText3->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1b->Add(m_staticText3, flagsBorder);

        wxButton *wxButton3 = new wxButton(this, wxCONSENT_TO_HAND, wxT("I Agree"), wxPoint(410, 335), wxDefaultSize, 0);
        sizerPage2->Add(wxButton3, flagsBorder);
        wxButton3->SetBackgroundColour(wxColour(99, 110, 114));

        wxButton *wxButton4 = new wxButton(this, wxGOBACK, wxT("I Do Not Agree"), wxPoint(500, 335), wxDefaultSize, 0);
        sizerPage2a->Add(wxButton4, flagsBorder);
        wxButton4->SetBackgroundColour(wxColour(99, 110, 114));
    }

    ChooseHand::ChooseHand(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();
        wxPanel *page = new wxPanel(this);
        wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1b = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1c = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage3 = new wxBoxSizer(wxVERTICAL);

        m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("You will be asked to raise one of your hands for an extended period of time for data collection."), wxPoint(30, 240), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText1->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText1, flagsBorder);

        m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("The other hand will be used to make selections in the app. Please choose which hand"), wxPoint(65, 265), wxSize(800, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText2->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1a->Add(m_staticText2, flagsBorder);

        m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("hand you would like to keep raised. If you choose to raise your left hand: Tap/click 'Left Hand'."), wxPoint(30, 290), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText3->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1b->Add(m_staticText3, flagsBorder);

        m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("If you choose to raise your right hand, Tap/click 'Right Hand'."), wxPoint(135, 315), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText4->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1c->Add(m_staticText4, flagsBorder);

        wxButton *wxButton5 = new wxButton(this, wxLEFT_HAND, wxT("Left Hand"), wxPoint(410, 360), wxDefaultSize, 0);
        sizerPage2->Add(wxButton5, flagsBorder);
        wxButton5->SetBackgroundColour(wxColour(99, 110, 114));

        wxButton *wxButton6 = new wxButton(this, wxRIGHT_HAND, wxT("Right Hand"), wxPoint(500, 360), wxDefaultSize, 0);
        sizerPage2a->Add(wxButton6, flagsBorder);
        wxButton6->SetBackgroundColour(wxColour(99, 110, 114));
    }

    VideoScreen1::VideoScreen1(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();
        wxPanel *page = new wxPanel(this);
        wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1b = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1c = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1d = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1e = new wxBoxSizer(wxVERTICAL);

        wxSizer *sizerPage2 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage3 = new wxBoxSizer(wxVERTICAL);

        m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("This experiment will gather videos of your hand and face to estimate certain biodata. "), wxPoint(70, 240), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText1->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText1, flagsBorder);

        m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("If you do not consent to participating in this experiment tap/click 'I Do Not Agree' to return to the "), wxPoint(20, 265), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText2->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1a->Add(m_staticText2, flagsBorder);

        m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("main screen. If you do consent please place the palm of your hand in the red circle and keep your "), wxPoint(20, 290), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText3->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1b->Add(m_staticText3, flagsBorder);

        m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("hand as still as possible. Please face directly into the camera so that the face angle and tracking "), wxPoint(30, 315), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText4->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1c->Add(m_staticText4, flagsBorder);

        m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("quality gauges are filled. When you are ready, press the f key to begin recording. The first "), wxPoint(50, 340), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText5->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1d->Add(m_staticText5, flagsBorder);

        m_staticText6 = new wxStaticText(this, wxID_ANY, wxT(" recording stage will last 75 seconds after which you will be given a chance to rest."), wxPoint(40, 365), wxSize(900, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText6->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1e->Add(m_staticText6, flagsBorder);

        wxButton *wxButton7 = new wxButton(this, wxScreen1_To_Screen2, wxT("I Agree"), wxPoint(400, 400), wxDefaultSize, 0);
        sizerPage2->Add(wxButton7, flagsBorder);
        wxButton7->SetBackgroundColour(wxColour(99, 110, 114));

        wxButton *wxButton8 = new wxButton(this, wxGOBACK, wxT("I Do Not Agree"), wxPoint(490, 400), wxDefaultSize, 0);
        sizerPage2a->Add(wxButton8, flagsBorder);
        wxButton8->SetBackgroundColour(wxColour(99, 110, 114));
    }

    VideoScreen2::VideoScreen2(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();
        wxPanel *page = new wxPanel(this);
        wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1b = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1c = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1d = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1e = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1f = new wxBoxSizer(wxVERTICAL);

        wxSizer *sizerPage2 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage3 = new wxBoxSizer(wxVERTICAL);

        m_staticText1 = new wxStaticText(this, 22, wxT("You may now rest your hand. For the next stage of the experiment you will be asked to follow a "), wxPoint(20, 240), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText1->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1->Add(m_staticText1, flagsBorder);

        m_staticText2 = new wxStaticText(this, 22, wxT("breathing exercise. Please sync your breathing with the animated ball. Exhale (or inhale) when the"), wxPoint(20, 265), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText2->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1a->Add(m_staticText2, flagsBorder);

        m_staticText4 = new wxStaticText(this, 22, wxT("ball hits the top and inhale (or exhale) when the ball reaches the bottom. When ready, raise your "), wxPoint(20, 290), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText4->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1c->Add(m_staticText4, flagsBorder);

        m_staticText5 = new wxStaticText(this, 22, wxT("palm into the blue circle again. Move your face into the red box and fill the quality gauges. When"), wxPoint(20, 315), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText5->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1d->Add(m_staticText5, flagsBorder);

        m_staticText6 = new wxStaticText(this, 22, wxT("ready, tap/click 'I'm Ready'. Continue syncing your breathing with the ball. If you do not feel "), wxPoint(40, 340), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText6->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1e->Add(m_staticText6, flagsBorder);

        m_staticText7 = new wxStaticText(this, 22, wxT("comfortable following the breathing exercise, tap/click 'I'm Not Ready' and breathe at your"), wxPoint(30, 365), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText7->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage1f->Add(m_staticText7, flagsBorder);

        m_staticText8 = new wxStaticText(this, 22, wxT("natural breathing rate."), wxPoint(140, 390), wxSize(700, 23), wxALIGN_CENTRE_HORIZONTAL);
        m_staticText8->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        sizerPage2->Add(m_staticText8, flagsBorder);

        wxButton *wxButton9 = new wxButton(this, wxScreen2_To_Screen3, wxT("I'm Ready"), wxPoint(390, 450), wxDefaultSize, 0);
        sizerPage2->Add(wxButton9, flagsBorder);
        wxButton9->SetBackgroundColour(wxColour(99, 110, 114));

        wxButton *wxButton10 = new wxButton(this, wxScreen2_To_Screen3, wxT("I'm Not Ready"), wxPoint(480, 450), wxDefaultSize, 0);
        sizerPage2a->Add(wxButton10, flagsBorder);
        wxButton10->SetBackgroundColour(wxColour(99, 110, 114));
    }

    VideoScreenFinal::VideoScreenFinal(wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
        wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();
        wxPanel *page = new wxPanel(this);
        wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage1a = new wxBoxSizer(wxVERTICAL);

        wxSizer *sizerPage2 = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage2a = new wxBoxSizer(wxVERTICAL);
        wxSizer *sizerPage3 = new wxBoxSizer(wxVERTICAL);

        sizerPage1->Add(new wxStaticText(this, wxID_ANY, wxT("Thank you for your participation!"), wxPoint(255, 240), wxSize(1000, 800), 0), flagsBorder);
        sizerPage1a->Add(new wxStaticText(this, wxID_ANY, wxT("Your Reference ID is:"), wxPoint(255, 270), wxSize(1000, 800), 0), flagsBorder);

        wxButton *wxButton11 = new wxButton(this, wxID_EXIT, wxT("Exit"), wxPoint(470, 290), wxDefaultSize, 0);
        sizerPage2a->Add(wxButton11, flagsBorder);
        wxButton11->SetBackgroundColour(wxColour(99, 110, 114));
    }

    bool VideoScreen2::CameraSave()
    {
        using namespace nlohmann;

        std::ifstream ifs("customized.json");
        json jf = json::parse(ifs);

        std::string fName = jf.value("path", "oops");

        cv::VideoCapture cap(0);
        // if not success, exit program
        if (cap.isOpened() == false)
        {
            std::cout << "Cannot open the video camera" << std::endl;
            std::cin.get(); //wait for any key press
            return -1;
        }

        int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));   //get the width of frames of the video
        int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

        cv::Size frame_size(frame_width, frame_height);
        int frames_per_second = 10;

        //Create and initialize the VideoWriter object
        cv::VideoWriter oVideoWriter(fName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                     frames_per_second, frame_size, true);

        //VideoWriter oVideoWriter("/Users/rohith/COVID/video_outputs/MyVideo.mjpeg", VideoWriter::fourcc('M', 'J', 'P', 'G'),
        //                                                           frames_per_second, frame_size, true);

        //If the VideoWriter object is not initialized successfully, exit the program
        if (oVideoWriter.isOpened() == false)
        {
            std::cout << "Cannot save the video to a file" << std::endl;
            std::cin.get(); //wait for any key press
            return -1;
        }

        std::string window_name = "My Camera Feed 2";
        namedWindow(window_name, cv::WINDOW_NORMAL); //create a window called "My Camera Feed"

        while (true)
        {
            cv::Mat frame;
            int sys_w = 1440;
            int sys_h = 900;

            bool isSuccess = cap.read(frame); // read a new frame from video

            //Breaking the while loop if the frames cannot be captured
            if (isSuccess == false)
            {
                std::cout << "Video camera is disconnected" << std::endl;
                std::cin.get(); //Wait for any key press
                break;
            }

            /*
        Can make changes to the frame as necessary, here:
        e.g.  
         1. Change brightness/contrast of the image
         2. Smooth/Blur image
         3. Crop the image
         4. Draw shapes on the image -- blue and red squares for face and hand respectively
        */

            //write the video frame to the file
            oVideoWriter.write(frame);

            //show the frame in the created window
            imshow(window_name, frame);
            cv::moveWindow(window_name, int(sys_w * 0.5), int(sys_h * 0.2)); //sys_w = 1440, sys_h = 900
            cv::resizeWindow(window_name, 550, 310);

            //wait for for 10 ms until any key is pressed.
            //If the 'Esc' key is pressed, break the while loop.
            //If the any other key is pressed, continue the loop
            //If any key is not pressed withing 10 ms, continue the loop
            if (cv::waitKey(10) == 27)
            {
                std::cout << "Esc key is pressed by user. Stopping the video" << std::endl;
                break;
            }
        }
        //Flush and close the video file
        oVideoWriter.release();

        return 0;
    }

    bool VideoScreen2::CountDown()
    {
        //Scene textures
        LTexture gTimeTextTexture;
        LTexture gPromptTextTexture;

        //The window renderer
        //SDL_Renderer *gRenderer_countdown = NULL;

        MiscFuncs *miscfuncs = new MiscFuncs();
        miscfuncs->init_countdown();

        //MiscFuncs* miscfuncs = new MiscFuncs();
        miscfuncs->loadMediacountdown(); //initialize the Helvetica font for countdown timer

        bool quit = false;

        //Event handler
        SDL_Event e;

        //Set text color as black
        SDL_Color textColor = {0, 0, 0, 255};

        //Current time start time
        Uint32 startTime = 0;

        //In memory text stream
        std::stringstream timeText;

        //While application is running
        while (!quit)
        {
            //Handle events on queue
            while (SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
            }

            //Set text to be rendered
            timeText.str("");
            int sec = (SDL_GetTicks() * 1000 - startTime) / 1000000; //time incremented in seconds

            int sectest = (SDL_GetTicks() * 1000) / 1000000;
            int sectest2 = 10 - sectest; //change sectest2 in coundown() function as well, defined in taborder.cpp
            //int sectest2 = 75 - sectest;

            timeText << "Time remaining:      " << miscfuncs->countdown();

            //Render text for timer
            gTimeTextTexture.loadFromRenderedText_countdown(timeText.str().c_str(), textColor);

            //Clear screen
            SDL_SetRenderDrawColor(gRenderer_countdown, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer_countdown);

            //Render textures
            gPromptTextTexture.render_countdown((SCREEN_WIDTH_COUNTDOWN - gPromptTextTexture.getWidth()) / 2, 0);
            gTimeTextTexture.render_countdown((SCREEN_WIDTH_COUNTDOWN - gPromptTextTexture.getWidth()) / 2, (SCREEN_HEIGHT_COUNTDOWN - gPromptTextTexture.getHeight()) / 2);

            //Update screen
            SDL_RenderPresent(gRenderer_countdown);

            if (sectest2 == 0)
            {
                //Free resources and close SDL
                //close_countdown();
                return 0;
            }
        }

        return 0;
    }

    bool VideoScreen2::BouncingBall()
    {
        MiscFuncs *miscfuncs = new MiscFuncs();
        bool success = true;
        //const SDL_Event * event;

        TTF_Init();
        gWindow = SDL_CreateWindow("Breathing Rate", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_BALL, SCREEN_HEIGHT_BALL, SDL_WINDOW_SHOWN);
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        miscfuncs->loadMedia();
        bool quit = false;
        SDL_Event event;

        Dot dot;
        while (!quit)
        {
            //Handle events on queue
            while (SDL_PollEvent(&event) != 0)
            {
                //User requests quit
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
            //VideoScreen2::CameraSave();

            //Move the dot
            dot.move();

            //add a delay for 1/40th of a second
            usleep(25000);

            //Clear Screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //background color
            SDL_RenderClear(gRenderer);

            //Render objects
            dot.render();

            //Update screen
            SDL_RenderPresent(gRenderer);
        }

        miscfuncs->close();

        return 0;
    }

    bool VideoScreen1::CameraOpen()
    {
        cv::VideoCapture cap(0);

        if (cap.isOpened() == false)
        {
            std::cout << "Cannot open the video camera" << std::endl;
            std::cin.get(); //wait for any key press
            return -1;
        }

        std::string window_name = "My Camera Feed 1";
        namedWindow(window_name, cv::WINDOW_NORMAL); //create a window called "My Camera Feed"

        while (true)
        {
            cv::Mat frame;
            int sys_w = 1440;
            int sys_h = 900;
            bool bSuccess = cap.read(frame); // read a new frame from video

            //Breaking the while loop if the frames cannot be captured
            if (bSuccess == false)
            {
                std::cout << "Video camera is disconnected" << std::endl;
                std::cin.get(); //Wait for any key press
                break;
            }

            //show the frame in the created window
            imshow(window_name, frame);
            cv::moveWindow(window_name, int(sys_w * 0.5), int(sys_h * 0.2)); //1440, 900
            cv::resizeWindow(window_name, 550, 310);

            //wait for for 10 ms until any key is pressed.
            //If the 'Esc' key is pressed, break the while loop.
            //If the any other key is pressed, continue the loop
            //If any key is not pressed withing 10 ms, continue the loop
            if (cv::waitKey(10) == 27)
            {
                std::cout << "Esc key is pressed by user. Stopping the video" << std::endl;
                break;
            }
        }
        return true;
    }

    bool VideoScreen1::CameraClose()
    {
        return true;
    }

} // namespace rppg_data_collection