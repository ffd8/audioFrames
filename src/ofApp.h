#pragma once

#include "ofMain.h"
#include "ofxLibsndFileRecorder.h"
#include "ofxGui.h"
#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        //AUDIO
        void audioReceived(float * input, int bufferSize, int nChannels);
        bool recording;
        ofxLibsndFileRecorder audioRecorder;
        string tempAudio;
        ofSoundPlayer  temp;
        Boolean pausePlayer;
    
        //AUDIO viz
        bool useAudio;
        bool avgAudio;
        float audioAmp;

        int bufferSize;
        vector <float> left;
        vector <float> right;
        vector <float> volHistory;
        
        int 	bufferCounter;
        int 	drawCounter;
        
        float smoothedVol;
        float scaledVol;
        
        ofSoundStream soundStream;
        
        //TIMERS
        float recTimer;
        float recTimerEnd;
        float passFrameRate, recFrameRate;
        float passFrames, recFrames;
        void recDuration();
        
        //GUI
        ofxPanel gui;
        ofxIntSlider frames;
        void framesChanged(int & frames);
        ofxIntSlider frameRate;
        void frameRateChanged(int & frameRate);
        ofxButton recRecord;
        void recRecordPressed();
        ofxButton recPlay;
        ofxToggle recLoop;
        void recPlayChanged();
        void recPlayPressed();
        ofxButton recExport;
        void recExportPressed();

		
};
