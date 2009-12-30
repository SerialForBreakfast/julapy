/*
 *  Sydfest_06.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 30/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SYDFEST_06_H_
#define _SYDFEST_06_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxShapes.h"
#include "ofxBox2d.h"
#include "ofxSimpleGuiToo.h"
#include "ofxColorUtils.h"

struct Circle_06
{
	int		id;
	ofPoint p;
	int		frame;
	int		color;
	float	radius;
	bool	tick;
};

class Sydfest_06 : public ofxJulapyApp
{
	
	public :
	
	 Sydfest_06 ();
	~Sydfest_06 ();
	
	void setup		();
	void update		();
	void draw		();
	void drawDebug	();
	
	void clear		();
	void runFromExternalData ();
	void saveColors	();
	
	void writeToFile	( string filename = "sydfest_06_data" );
	void loadFromFile	( string filename = "sydfest_06_data" );
	
	void keyReleased	( int key );
	void mousePressed	( int x, int y, int button );
	
	ofxSimpleGuiToo		gui;
	
	ofxShapes			shapes;
	
	ofImage				image;
	ofRectangle			imageRect;
	float				imageAlpha;
	
	ofImage				imageColor;
	float				imageColorAlpha;
	
	bool					bDrawDebug;
	bool					bExternalData;
	int						startFrame;
	int						circleAddRate;
	
	ofxBox2d				box2d;
	vector<ofxBox2dCircle>	box2dCircles;
	vector<ofxBox2dLine>	box2dLineStrips;
	
	vector<Circle_06>		circles;
};

#endif