#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
        
    // save temp file WITHIN app!
    CUR_VERSION = "1.1";
    ofSetWindowTitle("frameAudio - " + CUR_VERSION);
    ofSetDataPathRoot("../Resources/data/");
    ofSetEscapeQuitsApp(false);
    
    ofBackground(0,0,0);
    recording=false;
    
    //AUDIO
    bufferSize = BUFFER_SIZE;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    ofSoundStreamSetup(0, NUM_CHANNELS,this, SAMPLE_RATE,BUFFER_SIZE,4);
    tempAudio = "temp.wav";
    
    recTimer = 0;
    recTimerEnd = 0;
    recFrameRate = 40;
    passFrameRate = recFrameRate;
    recFrames = 160;
    passFrames = recFrames;
    
    
    //GUI
    gui.setup("Settings"); // most of the time you don't need a name
    gui.setPosition(10, ofGetHeight()/3);
    gui.add(frames.setup("  FRAMES TO REC", 25, 1, 100)); // 100
    gui.add(frameRate.setup("  FRAMES PER SEC", 25, 1, 100));
    gui.add(recRecord.setup("[R]ECORD"));
    gui.add(recPlay.setup("[P]LAY"));
    gui.add(recLoop.setup("[L]OOP", false));
    gui.add(recExport.setup("[S]AVE"));
    recRecord.addListener(this,&ofApp::recRecordPressed);
    recPlay.addListener(this,&ofApp::recPlayPressed);
    recExport.addListener(this,&ofApp::recExportPressed);
    //recLoop.addListener(this,&ofApp::recLoopChanged);
    
    //AUDIO
    audioAmp = 1000.00;
    soundStream.listDevices();
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    //ofHideCursor();
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    recRecord.removeListener(this,&ofApp::recRecordPressed);
    recPlay.removeListener(this,&ofApp::recPlayPressed);
    recExport.removeListener(this,&ofApp::recExportPressed);
    //recLoop.removeListener(this,&ofApp::recLoopChanged);
}

//--------------------------------------------------------------
void ofApp::recRecordPressed(){
    cout<<"Start recording\n";
    temp.stop();
    recLoop = false;
    if(ofFile::doesFileExist(tempAudio, true)){
        ofFile::removeFile(tempAudio, true);
    }
    cout << tempAudio <<"----\n";
    audioRecorder.setup(tempAudio); // (pt)
    audioRecorder.setFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
    recording=true;
}
//--------------------------------------------------------------
void ofApp::recPlayPressed(){
    temp.play();
}
//--------------------------------------------------------------
void ofApp::recLoopChanged(){
    temp.play();
}
//--------------------------------------------------------------
void ofApp::recExportPressed(){
    ofBuffer buf = ofBufferFromFile(tempAudio, true);
    string tempPath = "audio_" + ofToString(int(frames)) + "_" + ofToString(int(frameRate)) + ".wav";
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(tempPath, "Save your file");
    if (saveFileResult.bSuccess){
        bool fileWritten = ofBufferToFile(saveFileResult.filePath, buf);
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    //AUDIO
    if(useAudio){
        //lets scale the vol up to a 0-1 range
        scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
        
        //lets record the volume into an array
        volHistory.push_back( scaledVol );
        
        //if we are bigger the the size we want to record - lets drop the oldest value
        if( volHistory.size() >= 400 ){
            volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofBackground(200);
    
     if(recLoop){
         temp.setLoop(true);
         if(!temp.getIsPlaying()){
             temp.play();
         }
     }else{
         temp.setLoop(false);
     }

    
    recFrameRate = float(1/float(frameRate)*1000);
    recFrames = float(frames*recFrameRate);
    
    if(ofGetElapsedTimeMillis() > recTimer + recFrames && recording){
        cout<<"Stop recording\n";
        recording=false;
        audioRecorder.finalize();
        audioRecorder.close();
        temp.loadSound(tempAudio);
        
        
        //recPlay = true;
        temp.play();
        temp.setVolume(1.0f);
    }else if(!recording){
        recTimer = ofGetElapsedTimeMillis();
    }
    
    if(recording){
        ofSetColor(255,0,0);
    }else{
        ofSetColor(255,255,255);
    }
    
    ofPushMatrix();
    ofNoFill();
    ofSetLineWidth(2);
    ofTranslate(0, ofGetHeight()*.25);
    ofBeginShape();
    for(int i = 0; i < bufferSize; i++){
        float x = ofMap(i,0,bufferSize,0,ofGetWidth());
        //float x = left[i]*audioAmp;
        float y = left[i]*audioAmp;
        
        ofCurveVertex(x, y);
    }
    ofEndShape();
    
    ofTranslate(0, ofGetHeight()*.5);
    ofBeginShape();
    for(int i = 0; i < bufferSize; i++){
        float x = ofMap(i,0,bufferSize,0,ofGetWidth());
        //float x = left[i]*audioAmp;
        float y = right[i]*audioAmp;
        
        ofCurveVertex(x, y);
    }
    ofEndShape();
    ofPopMatrix();

    gui.draw();
    }

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='r'){
        recRecordPressed();
    }else if(key == 'p'){
        recPlayPressed();
    }else if(key == 'l'){
        if(!recLoop){
            temp.play();
            recLoop = true;
        }else{
            temp.stop();
            recLoop = false;
        }
    }else if(key == 's'){
        recExportPressed();
        
    }else if(key == 356){ // left arrow
        int tempFrames = frames;
        frames = tempFrames-1;
        if(frames < 1){
            frames = 1;
        }
    }else if(key == 358){ // right arrow
        int tempFrames = frames;
        frames = tempFrames+1;
        if(frames > 100){
            frames = 100;
        }
    }else if(key == 359){ // Down arrow
        int tempFrameRate = frameRate;
        frameRate = tempFrameRate-1;
        if(frameRate < 1){
            frameRate = 1;
        }
    }else if(key == 357){ // UP arrow
        int tempFrameRate = frameRate;
        frameRate = tempFrameRate+1;
        if(frameRate > 100){
            frameRate = 100;
        }
    }
    cout << key << endl;

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    ofShowCursor();

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //temp.play();
    ofShowCursor();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    if(recording){
        audioRecorder.addSamples(input,bufferSize);
    }
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    if(input){
        for (int i = 0; i < bufferSize; i++){
            left[i]		= input[i*2]*0.5;
            right[i]	= input[i*2+1]*0.5;
            
            curVol += left[i] * left[i];
            curVol += right[i] * right[i];
            numCounted+=2;
        }
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;

}




