# audioFrames
Record audio frame samples, based on given framerate

Requires: https://github.com/angeloseme/ofxLibsndfileRecorder

Just a simple tool to solve a hard problem.. record film frame length samples based on a particular framerate (ie. 24).
Couldn't find any easy tools to make it happen for a workshop, so hopefully this one can make the magic happen.
Now if only there was a stupideasy tool for arranging/dragging+dropping these samples on a timeline below a movie file...

BUGS:

Still trying to work out issues with the tempaudio.wav location (records sample to this for playback before user can save sample to custom path).

Should really be switched to using a buffer for temp storing the recording.. but that's beyond my skills right now.
