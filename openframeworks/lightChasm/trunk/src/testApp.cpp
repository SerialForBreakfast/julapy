#include "testApp.h"

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofBackground( 255, 255, 255 );
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofSetVerticalSync( true );
	ofSoundStreamSetup( 0, 2, this, 44100, 512, 4 );
	
	initAudioIn();
	initOpScope();
	initOpCheckers();
	initOpCircle();
	initOpBars();
	initOpRain();
	initVideos();
	initOsc();
}

///////////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////////

void testApp :: initAudioIn ()
{
	audioIn.init();
	audioInAvgPower.init();
}

void testApp :: initOpCheckers ()
{
	opCheckersSize		= 150;
	opCheckersSizeScale	= 0.5f;
	
	opCheckers.init( 640, 480 );
	opCheckers.setSize( opCheckersSize * opCheckersSizeScale );
}

void testApp :: initOpScope ()
{
	opScope.init( 640, 480 );
}

void testApp :: initOpCircle ()
{
	opCirlceWidth	= 640;
	opCirlceHeight	= 480;
	
	float rx = opCirlceWidth / 2.0f;
	float ry = opCirlceHeight / 2.0f;
	
	opCirlceRot			= 0.0f;
	opCirlceRotScale	= 0.0f;
	opCirlceRes			= MIN_CIRCLE_PTS;
	opCirlceResScale	= 0.5f;
	opCirlceColor		= 0.0f;
	
	opCirlce.x		= rx;
	opCirlce.y		= ry;
	opCirlce.radius	= sqrt( rx * rx + ry * ry ) * 1.5f;
	opCirlce.setCirlceResolution( opCirlceRes = (int)( opCirlceResScale * MAX_CIRCLE_PTS ) );
	opCirlce.setRotation( opCirlceRot );
}

void testApp :: initOpBars ()
{
	opBarsAudioAvgMin		= 60;
	opBarsAudioAvgMinScale	= 0.5f;
	
	opBars.init( 640, 480 );
	opBars.setNumberOfBars( audioIn.averagesTotal );
	opBars.setAudioAvgMin( opBarsAudioAvgMin * opBarsAudioAvgMinScale );
}

void testApp :: initOpRain()
{
	opRain.init( 640, 480 );
}

void testApp :: initVideos ()
{
	videosTotal				= 3;
	videoPosition			= 0;
	videoPositionOsc		= 0;
	videoPositionOverride	= false;
	
	videos = new VideoObj[ videosTotal ];
	videos[ 0 ].video.loadMovie( "flight_01.mov" );
	videos[ 1 ].video.loadMovie( "nionr_02.mov" );
	videos[ 2 ].video.loadMovie( "flight_02.mov" );
	
	for( int i=0; i<videosTotal; i++ )
	{
		videos[ i ].video.setPaused( true );
		videos[ i ].duration	= videos[ i ].video.getDuration();
		videos[ i ].startTime	= 0;
		videos[ i ].playing		= false;
		videos[ i ].oscPlaying	= false;
	}
}

void testApp :: initOsc ()
{
	oscReceiver.setup( 12345 );
	
	oscRcvr.init( &oscReceiver );
	oscRcvr.addInput( "/1/fader0", &opCheckersSizeScale );
	oscRcvr.addInput( "/1/fader1", &videoPositionOsc );
	oscRcvr.addInput( "/1/fader2", &opBarsAudioAvgMinScale );
	oscRcvr.addInput( "/1/fader3", &opCirlceResScale );
	oscRcvr.addInput( "/1/fader4", &opCirlceRotScale );
	oscRcvr.addInput( "/1/fader5", &opCirlceColor );
	
	oscRcvr.addInput( "/1/toggle1", &videos[ 0 ].oscPlaying );
	oscRcvr.addInput( "/1/toggle2", &videos[ 1 ].oscPlaying );
	oscRcvr.addInput( "/1/toggle3", &videos[ 2 ].oscPlaying );
	oscRcvr.addInput( "/1/toggle4", &videoPositionOverride );
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp::update()
{
	audioIn.update();
	float avgPowerScaled =  audioInAvgPower.getNormalisedValue( audioIn.averagePower );
//	printf( "avgPowerScaled: %4.2f \n", avgPowerScaled );
	
	updateOsc();
	
	updateVideo();
	
	opCirlce.setRotation( opCirlceRot += opCirlceRotScale * 1.0 + 0.1 );
	opCirlce.setAudioInValue( avgPowerScaled );
	opCirlce.setRgbScale( opCirlceColor, opCirlceColor, opCirlceColor );
	opCirlce.setCirlceResolution( opCirlceRes = (int)( opCirlceResScale * ( ( MAX_CIRCLE_PTS - 1 ) * 0.5 ) ) * 2 );
	opCirlce.update();

	opBars.setAudioAvgMin( opBarsAudioAvgMin * opBarsAudioAvgMinScale );
	opBars.setAudioInData( audioIn.averages );
	opBars.update();
	
	opCheckers.setSize( MAX( 3, opCheckersSize * opCheckersSizeScale ) );
	opCheckers.update();
	
	opRain.update();
}

void testApp :: updateOsc ()
{
	oscRcvr.ping();
}

void testApp :: updateVideo ()
{
	videoPosition += ( videoPositionOsc - videoPosition ) * 0.02;
	
	for( int i=0; i<videosTotal; i++ )
	{
		if( videos[ i ].playing != videos[ i ].oscPlaying )	// osc toggle button has changed.
		{
			videos[ i ].playing = videos[ i ].oscPlaying;
			
			if( videos[ i ].playing )
			{
				videos[ i ].startTime = ofGetElapsedTimef();
			}
		}
		
		if( videos[ i ].playing )
		{
			float t = ( ofGetElapsedTimef() - videos[ i ].startTime ) / videos[ i ].duration;
			if( t > 1 )
			{
				t = 0;
				videos[ i ].startTime = ofGetElapsedTimef();
			}
			
			if( videoPositionOverride )
			{
				videos[ i ].video.setPosition( videoPosition );
			}
			else
			{
				videos[ i ].video.setPosition( t );
			}
		}
	}
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp::draw()
{
	opRain.drawToFBO();
	opCirlce.drawToFBO();
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR );
	
	drawVideos();
	opCheckers.draw();
	opBars.draw();
	opCirlce.drawFBOToScreen();
	opRain.drawFBOToScreen();
	
	glDisable( GL_BLEND );

//	audioIn.draw();
	
	ofSetColor( 0xFF0000 );
	ofDrawBitmapString
	(
		"fps :: " + ofToString(ofGetFrameRate(), 2) + "\n\n",
		20,
		20
	);
}

void testApp :: drawVideos()
{
	for( int i=0; i<videosTotal; i++ )
	{
		if( videos[ i ].playing )
		{
			opScope.update( videos[ i ].video.getPixels() );
			opScope.draw();
		}
	}
}

//--------------------------------------------------------------

void testApp :: keyPressed(int key)
{
	//
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


void testApp :: audioReceived( float *input, int bufferSize, int nChannels )
{
	for( int i=0; i<bufferSize; i++)
	{
		audioIn.left[ i ]	= input[ i * 2 ];
		audioIn.right[ i ]	= input[ i * 2 + 1 ];
	}
}
