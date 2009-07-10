
#include "testApp.h"

///////////////////////////////////////////////////////////
// SETUP.
///////////////////////////////////////////////////////////

void testApp::setup()
{
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(true);
	ofSetFrameRate(60);
	
	ofxAccelerometer.setup();

	ofxMultiTouch.addListener(this);
	
	verdana.loadFont( ofToDataPath( "verdana.ttf" ), 8, true, true );
	
//	earthMap.loadImage( "EarthMap_2048x1024.jpg" );
//	earthMap.loadImage( "EarthMap_1024x512.jpg" );
	earthMap.loadImage( "EarthMap_1024x512_hFlip.jpg" );
//	earthMap.loadImage( "EarthMap_1024x512_hFlip_debug.jpg" );
	earthMapTexture = earthMap.getTextureReference();
	
	initLocations();
	initCurveHop();
	
	initTrackball();
//	initLighting();
}

void testApp :: initLocations()
{
	locationIndex	= 0;
	locationsTotal	= 14;
	locations		= new float[ locationsTotal ];	// latitude and longitude pairs.
	locations[ 0 ]	= -33.87784462833714;	// Holler Sydney
	locations[ 1 ]	= 151.2179660797119;	// Holler Sydney
	locations[ 2 ]	= 40.756054;			// New York
	locations[ 3 ]	= -73.986951;			// New York
	locations[ 4 ]	= 48.856667;			// Paris
	locations[ 5 ]	= 2.350987;				// Paris
	locations[ 6 ]	= 51.500152;			// London
	locations[ 7 ]	= -0.126236;			// London
	locations[ 8 ]	= 25.271139;			// Dubai
	locations[ 9 ]	= 55.307485;			// Dubai
	locations[ 10 ]	= 9.009697;				// Panama City
	locations[ 11 ]	= -79.603243;			// Panama City
	locations[ 12 ]	= 35.689488;			// Tokyo
	locations[ 13 ]	= 139.691706;			// Tokyo
}

void testApp :: initCurveHop ()
{
	ofxVec3f p1 = sphericalToCartesian( locations[ 0 ], locations[ 1 ], 1 );
	ofxVec3f c1 = sphericalToCartesian( locations[ 0 ], locations[ 1 ], 2 );
	ofxVec3f p2 = sphericalToCartesian( locations[ 2 ], locations[ 3 ], 1 );
	ofxVec3f c2 = sphericalToCartesian( locations[ 2 ], locations[ 3 ], 2 );
	
	float *bezierPoints	= new float[ 12 ];
	bezierPoints[ 0 ]	= p1.x;
	bezierPoints[ 1 ]	= p1.y;
	bezierPoints[ 2 ]	= p1.z;

	bezierPoints[ 3 ]	= c1.x;
	bezierPoints[ 4 ]	= c1.y;
	bezierPoints[ 5 ]	= c1.z;

	bezierPoints[ 6 ]	= c2.x;
	bezierPoints[ 7 ]	= c2.y;
	bezierPoints[ 8 ]	= c2.z;

	bezierPoints[ 9 ]	= p2.x;
	bezierPoints[ 10 ]	= p2.y;
	bezierPoints[ 11 ]	= p2.z;
	
	curveHop.set( bezierPoints );
	
	delete[] bezierPoints;
}

void testApp :: initLighting()
{
	lightAmbient[ 0 ] = 0.2f;
	lightAmbient[ 1 ] = 0.3f;
	lightAmbient[ 2 ] = 0.6f;
	lightAmbient[ 3 ] = 1.0f;
	
	lightDiffuse[ 0 ] = 0.2f;
	lightDiffuse[ 1 ] = 0.3f;
	lightDiffuse[ 2 ] = 0.6f;
	lightDiffuse[ 3 ] = 1.0f;
	
	matAmbient[ 0 ] = 0.6f;
	matAmbient[ 1 ] = 0.6f;
	matAmbient[ 2 ] = 0.6f;
	matAmbient[ 3 ] = 1.0f;

	matDiffuse[ 0 ] = 0.6f;
	matDiffuse[ 1 ] = 0.6f;
	matDiffuse[ 2 ] = 0.6f;
	matDiffuse[ 3 ] = 1.0f;
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

///////////////////////////////////////////////////////////
// TRACKBALL.
///////////////////////////////////////////////////////////

void testApp :: initTrackball ()
{
	gTrackBallRotation[ 0 ] = 0.0f;
	gTrackBallRotation[ 1 ] = 0.0f;
	gTrackBallRotation[ 2 ] = 0.0f;
	gTrackBallRotation[ 3 ] = 0.0f;

	gWorldRotation[ 0 ]	= 0.0f;
	gWorldRotation[ 1 ]	= 0.0f;
	gWorldRotation[ 2 ]	= 0.0f;
	gWorldRotation[ 3 ]	= 0.0f;
	
	gTrackBall = false;
}

void testApp :: mouseTrackballDown( int x, int y )
{
	startTrackball( x, y, 0, 0, ofGetWidth(), ofGetHeight() );
	gTrackBall = true;
}

void testApp :: mouseTrackballUp( int x, int y )
{
	gTrackBall = false;
	rollToTrackball( x, y, gTrackBallRotation );
	if( gTrackBallRotation[0] != 0.0 )
	{
		addToRotationTrackball( gTrackBallRotation, gWorldRotation );
	}
	gTrackBallRotation[ 0 ] = gTrackBallRotation[ 1 ] = gTrackBallRotation[ 2 ] = gTrackBallRotation[ 3 ] = 0.0f;
}

void testApp :: mouseTrackballMove( int x, int y )
{
	if (gTrackBall)
	{
		rollToTrackball( x, y, gTrackBallRotation );
	}
}

///////////////////////////////////////////////////////////
// UPDATE.
///////////////////////////////////////////////////////////

void testApp::update()
{
	//
}

ofxVec3f testApp :: sphericalToCartesian( float lat, float lon, float radius )
{
	ofxVec3f result;
	
	lat	*= -1;		// inverse latitude.
	lat += 90;		// latitude offset to match geometry of the sphere.
	lon *= -1;		// inverse longitude.
	lon -= 90;		// longitude offset to match geometry of the sphere.
	
	lat *= DEG_TO_RAD;
	lon *= DEG_TO_RAD;
	
	result.x = radius * sin( lat ) * cos( lon );
	result.y = radius * sin( lat ) * sin( lon );
	result.z = radius * cos( lat );
	
	return result;
}

///////////////////////////////////////////////////////////
// DRAW.
///////////////////////////////////////////////////////////

void testApp::draw()
{
//	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
//	char str[ 255 ];
//	sprintf( str, "lat :: %1.2d", latDebug );
//	verdana.drawString( str, 10, 10 );
	
	glEnable( GL_DEPTH_TEST );
	
	glPushMatrix();
	glTranslatef( ofGetWidth() * 0.5, ofGetHeight() * 0.5, -ofGetHeight() );
	glScalef( 300, 300, 300 );

	glRotatef( gTrackBallRotation[ 0 ], gTrackBallRotation[ 1 ], gTrackBallRotation[ 2 ], gTrackBallRotation[ 3 ] );
	glRotatef( gWorldRotation[ 0 ], gWorldRotation[ 1 ], gWorldRotation[ 2 ], gWorldRotation[ 3 ] );

	earthMapTexture.bind();
	
	sphere.setSphereDrawStyle( 100012 );
	sphere.drawSphere( 1, 20, 20 );
	
	earthMapTexture.unbind();
	
//	sphere.setSphereDrawStyle( 100013 );
//	sphere.drawSphere( 1.01, 20, 20 );
	
	drawLines();
	
//	drawTriangle();
//	drawCube();
	
	glPopMatrix();
}

void testApp :: drawLines ()
{
	float lat		= locations[ locationIndex + 0 ];
	float lon		= locations[ locationIndex + 1 ];
	float radius	= 2;

//	printf( "longitude :: %1.3f\n", lon );
	
	ofxVec3f p = sphericalToCartesian( lat, lon, radius );
	
	GLfloat lines[] = {
		0.0f, 0.0f, 0.0f,
		p.x, p.y, p.z
	};
	
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f
	};
	
	glVertexPointer( 3, GL_FLOAT, 0, lines );
	glColorPointer( 4, GL_FLOAT, 0, colors );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glDrawArrays( GL_LINES, 0, 3 );
}

void testApp :: drawTriangle ()
{
	GLfloat triangle[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	glPushMatrix();
	glTranslatef( -0.5f, -0.5f, -1.0f );
	
	glTranslatef( 0, 0, -1 );
	
	glVertexPointer(3, GL_FLOAT, 0, triangle);
	glColorPointer(4, GL_FLOAT, 0, colors);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glPopMatrix();
}

void testApp :: drawCube ()
{
	GLfloat box[] = 
	{
		// FRONT
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		// BACK
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		// LEFT
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		// RIGHT
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		// TOP
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		// BOTTOM
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
	};
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	
	glVertexPointer(3, GL_FLOAT, 0, box);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// FRONT AND BACK
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	
	// LEFT AND RIGHT
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	
	// TOP AND BOTTOM
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	
	glPopMatrix();
}

///////////////////////////////////////////////////////////
// EXIT.
///////////////////////////////////////////////////////////

void testApp::exit()
{
	printf("exit()\n");
}

///////////////////////////////////////////////////////////
// HANDLERS.
///////////////////////////////////////////////////////////

void testApp::mouseMoved(int x, int y )
{
	printf("mouseMoved: %i, %i\n", x, y);	// this will never get called 
}

// mouse functions are there for backwards compatibility
// they are simply the first finger to touch the screen
// you can omit the mouse functions and just use touch functions
// or omit touch functions and just use mouse functions if you don't need multitouch
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	printf("mouseDragged: %i, %i %i\n", x, y, button);
}

void testApp::mousePressed(int x, int y, int button)
{
	printf("mousePressed: %i, %i %i\n", x, y, button);
}

void testApp::mouseReleased()
{
	printf("frameRate: %.3f, frameNum: %i\n", ofGetFrameRate(), ofGetFrameNum());
}

void testApp::mouseReleased(int x, int y, int button)
{
	printf("mouseReleased: %i, %i %i\n", x, y, button);
}

//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchDown: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
		mouseTrackballDown( x, y );
	}
}

void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchMoved: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
		mouseTrackballMove( x, y );
	}
}


void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchUp: %i, %i %i\n", x, y, touchId);
	
	if( touchId == 0 )
	{
		mouseTrackballUp( x, y );
	}
}

void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	printf("touchDoubleTap: %i, %i %i\n", x, y, touchId);
	ofToggleFullscreen();
}

