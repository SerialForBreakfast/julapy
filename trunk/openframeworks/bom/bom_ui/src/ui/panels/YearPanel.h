/*
 *  YearPanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 22/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

class YearPanel : public ofxSprite
{

public:
	
	YearPanel()
	{
		setup();
	}
	
	~YearPanel() {};
	
	//==================================================
	
	ofImage*		image;
	ofTrueTypeFont	rockwell_50;
	int				year;
	
	//==================================================
	
	void setup ()
	{
		x = 1;
		y = 140;
		
		image = (ofImage*)ofxAssets :: getInstance()->getAsset( "year_panel" );
		
		rockwell_50.loadFont( "assets/fonts/Rockwell.ttf", 36 );
		
		//-- model.
		
		Model* model;
		model = Model :: getInstance();
		
		year = model->getYear();
		
		ofAddListener( model->yearChangeEvent, this, &YearPanel :: yearChanged );
	}
	
	void yearChanged ( int& year )
	{
		this->year = year;
	}
	
	void update ()
	{
		//
	}
	
	void draw ()
	{
		ofEnableAlphaBlending();
		
		image->draw( x, y );
		
		string yearStr;
		yearStr = ofToString( year, 0 );
		
		rockwell_50.drawString( yearStr.substr( 0, 1 ),  34, 206 );
		rockwell_50.drawString( yearStr.substr( 1, 1 ),  73, 206 );
		rockwell_50.drawString( yearStr.substr( 2, 1 ), 112, 206 );
		rockwell_50.drawString( yearStr.substr( 3, 1 ), 151, 206 );
		
		ofDisableAlphaBlending();
	}
};