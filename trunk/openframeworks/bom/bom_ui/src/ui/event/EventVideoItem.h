/*
 *  EventVideoItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventItem.h"

class EventVideoItem : public EventItem
{
	
public:
	
	 EventVideoItem() {};
	~EventVideoItem() {};
	
	//==================================================
	
	ofVideoPlayer*	eventVideo;
	
	//==================================================
	
	void setup ()
	{
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "event_bg_video" );
		eventVideo	= (ofVideoPlayer*)ofxAssets :: getInstance()->getAsset( "event.movie" );
	}
	
	void show ()
	{
		EventItem :: show();
		
		if( eventVideo != NULL )
		{
			eventVideo->setPosition( 0 );
			eventVideo->play();
		}
	}
	
	void hide ()
	{
		EventItem :: hide();
	
		if( eventVideo != NULL )
		{
			eventVideo->stop();
		}
	}
	
	void update ()
	{
		EventItem :: update();
	}
	
	void draw ()
	{
		if( !visible )
			return;
		
		ofSetColor( 0xFFFFFF );
		
		if( bg != NULL )
		{
			bg->draw( x, y );
		}
		
		if( eventVideo != NULL )
		{
			eventVideo->draw( x, y );
		}
		
		EventItem :: draw();
	}
	
};