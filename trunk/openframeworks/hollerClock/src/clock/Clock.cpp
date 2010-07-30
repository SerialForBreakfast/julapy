/*
 *  Clock.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Clock.h"

Clock :: Clock ()
{
	box2d		= NULL;
	font		= NULL;
	softBody	= NULL;
	
	secTwoSound	= NULL;
	secOneSound	= NULL;
	
	clockMode = CLOCK_MODE_1;
	
	hrsOneTotal = 2;
	hrsTwoTotal = 9;
	minOneTotal = 5;
	minTwoTotal = 9;
	secOneTotal = 5;
	secTwoTotal = 9;
	
	hrsOneX = hrsOneM1X	= 150;
	hrsTwoX = hrsTwoM1X	= 220;
	minOneX = minOneM1X	= 590;
	minTwoX = minTwoM1X	= 660;
	secOneX = secOneM1X	= 1000;
	secTwoX = secTwoM1X	= 1070;
	
	hrsOneM2X = 100;
	hrsTwoM2X = 270;
	minOneM2X = 540;
	minTwoM2X = 710;
	secOneM2X = 950;
	secTwoM2X = 1120;
	
	forceCenterPull	= 30;
	forceCenterPush = 30;
	rayBlobPad		= 0.07;
	rayBlobEase		= 0.4;
	
	setSize( ofGetWidth(), ofGetHeight() );
	setGravity( 0, 0 );
	setForceScale( 1.0 );
}

Clock :: ~Clock()
{
	//
}

///////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////

void Clock :: setup ()
{
	if( box2d == NULL )
		return;
	
	createBounds();
	createCircles();
//	createSoftBody();
}

///////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////

void Clock :: setBox2d ( ofxBox2d *box2d )
{
	this->box2d = box2d;
}

void Clock :: setSize ( ofRectangle &size )
{
	setSize( size.width, size.height );
}

void Clock :: setSize ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
	screenMinLength	= MIN( w, h );
	screenMaxLength = MAX( w, h );
	screenTotal		= w * h;
	
	screenCenter.x	= screenWidth * 0.5;
	screenCenter.y	= screenHeight * 0.5;
	
	//-- update circles.
	
	for( int i=0; i<circlesAll.size(); i++ )
	{
		circlesAll[ i ]->setSize( w, h );
	}
}

void Clock :: setTimeFont ( ofTrueTypeFont *font )
{
	this->font = font;
}

#ifdef TARGET_OF_IPHONE	
void Clock :: setSound ( ofxALSoundPlayer* secTwoSound, ofxALSoundPlayer* secOneSound )
{
	this->secTwoSound = secTwoSound;
	this->secOneSound = secOneSound;
}
#else
void Clock :: setSound ( ofSoundPlayer* secTwoSound, ofSoundPlayer* secOneSound )
{
	this->secTwoSound = secTwoSound;
	this->secOneSound = secOneSound;
}
#endif

void Clock :: setGravity( float x, float y )
{
	if( clockMode == CLOCK_MODE_2 )
	{
		gravity.x += ( x - gravity.x ) * 0.6;
		gravity.y += ( y - gravity.y ) * 0.6;
	}
}

void Clock :: setForceScale ( float f )
{
	forceScale = f;
}

void Clock :: createBounds ()
{
	b2BodyDef bd;
	bd.position.Set( 0, 0 );
	ground = box2d->ground = box2d->world->CreateBody( &bd );
	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	sd.density		= 0.0f;
	sd.restitution	= 0.0f;
	sd.friction		= 0.6;
	
	float thick		= 0.02 * screenHeight;
	
	int w = screenWidth;
	int h = screenHeight;
	
	sd.SetAsBox		//-- right		( float32 hx, float32 hy, const b2Vec2& center, float32 angle )
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2,
		b2Vec2( ( w + thick ) / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape( &sd );
	
	sd.SetAsBox		//-- left
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2, 
		b2Vec2( -thick / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- top
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, -thick /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- bottom
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, ( h + thick ) /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
}

void Clock :: createCircles ()
{
	float area	= 0.015;		// biggest area as start.
	float dec	= 0.5;		// decrease in area for the next batch of circles.
	
	createCircle( hrsOne, hrsOneTotal, areaToRadius( area ),		0x6b007e, 0.10 );
	createCircle( hrsTwo, hrsTwoTotal, areaToRadius( area *= dec ),	0xf51d2a, 0.23 );
	createCircle( minOne, minOneTotal, areaToRadius( area *= dec ),	0xf6009d, 0.44 );
	createCircle( minTwo, minTwoTotal, areaToRadius( area *= dec ),	0x8c162f, 0.57 );
	createCircle( secOne, secOneTotal, areaToRadius( area *= dec ),	0xc96dfd, 0.76 );
	createCircle( secTwo, secTwoTotal, areaToRadius( area *= dec ),	0xf7719a, 0.88 );
}

void Clock  :: createCircle ( vector<ClockCircle*> &circlesVec, int numOfCircle, float radius, int color, float lx )
{
	int t = numOfCircle;
	
	for( int i=0; i<t; i++ )
	{
		ClockCircle* circle;
		circle = new ClockCircle( radius, color );
		
		//-- define circles physics.
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		
//		circle->enableGravity( false );
		circle->setPhysics( mass, bounce, friction );
		circle->setup( box2d->getWorld(), ofRandom( 0, screenWidth ), ofRandom( 0, screenHeight ), radius, false );
		circle->setRotationFriction( 1.0 );
		circle->setDamping( 1.0 );
		
		//-- define line up point.
		
		float rx = radius / (int)screenWidth;
		float ry = radius / (int)screenHeight;
		
		float lineX;
		lineX = lx;
		lineX += ( i > 4 ) ? rx * 2.2 : 0;

		int j = i % 5;
		
		float lineY;
		lineY = 1.0;
		lineY -= 0.14;
		lineY -= ry;
		lineY -= j * ry * 2;
		lineY -= j * ( ry * 0.2 );
		
		circle->lineUpPoint.set( lineX, lineY );
		circle->setSize( screenWidth, screenHeight );
		circle->init();
		
		//-- add to vectors.
		
		circlesVec.push_back( circle );
		circlesAll.push_back( circle );
		
//		gravity->AddBody( circle->body );
	}
}

float Clock :: areaToRadius ( float area )
{
	float r;
	r = sqrt( area * screenTotal );
	r *= 0.5;
	
	return r;
}

void Clock :: createSoftBody ()
{
	float mass		= 3.0;
	float bounce	= 0.53;
	float friction	= 0.1;
	
	softBody = new ofxBox2dSoftBody();
	softBody->setPhysics( mass, bounce, friction );
	softBody->setup( box2d->getWorld(), ofRandom( 0, screenWidth ), ofRandom( 0, screenHeight ) );
}

///////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////

void Clock :: update ( int hrs, int min, int sec )
{
	int hrsOneNew = hrs / 10;
	int hrsTwoNew = hrs % 10;
	
	int minOneNew = min / 10;
	int minTwoNew = min % 10;
	
	int secOneNew = sec / 10;
	int secTwoNew = sec % 10;
	
	if( secTwoCount != secTwoNew )
	{
		if( secTwoNew == 0 )
		{
			playSecOneSound();
		}
		else 
		{
			playSecTwoSound();
		}

	}
	
	hrsOneCount = hrsOneNew;
	hrsTwoCount = hrsTwoNew;
	
	minOneCount = minOneNew;
	minTwoCount = minTwoNew;
	
	secOneCount = secOneNew;
	secTwoCount = secTwoNew;

	//--
	
	updateText();
	updateForces();
	
	if( clockMode == CLOCK_MODE_1 )
	{
//		updateRayBlob();
	}
	
	updateConvexBlob();
	
	box2d->update();
	
	for( int i=0; i<circlesAll.size(); i++ )
	{
		circlesAll[ i ]->update();
	}
}

void Clock :: playSecTwoSound ()
{
	if( secTwoSound != NULL )
		secTwoSound->play();
}

void Clock :: playSecOneSound ()
{
	if( secOneSound != NULL )
		secOneSound->play();
}

void Clock :: updateText ()
{
	float ease = 0.3;
	
	if( clockMode == CLOCK_MODE_1 )
	{
		hrsOneX += ( hrsOneM1X - hrsOneX ) * ease;
		hrsTwoX += ( hrsTwoM1X - hrsTwoX ) * ease;
		minOneX += ( minOneM1X - minOneX ) * ease;
		minTwoX += ( minTwoM1X - minTwoX ) * ease;
		secOneX += ( secOneM1X - secOneX ) * ease;
		secTwoX += ( secTwoM1X - secTwoX ) * ease;
	}
	else if ( clockMode == CLOCK_MODE_2 )
	{
		hrsOneX += ( hrsOneM2X - hrsOneX ) * ease;
		hrsTwoX += ( hrsTwoM2X - hrsTwoX ) * ease;
		minOneX += ( minOneM2X - minOneX ) * ease;
		minTwoX += ( minTwoM2X - minTwoX ) * ease;
		secOneX += ( secOneM2X - secOneX ) * ease;
		secTwoX += ( secTwoM2X - secTwoX ) * ease;
	}
}

///////////////////////////////////////////////
//	PREPARE CIRCLES.
///////////////////////////////////////////////

void Clock :: initModeOne ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		circle.destroyJoint();						// destroy all joints.
	}
}

void Clock :: initModeTwo ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		circle.destroyJoint();						// destroy all joints.
	}
}

///////////////////////////////////////////////
//	CLOCK MODE 1.
///////////////////////////////////////////////

void Clock :: updateForces ()
{
	circlesActive.clear();
	circlesInactive.clear();
	
	updateForcesVec( hrsOne, hrsOneCount );
	updateForcesVec( hrsTwo, hrsTwoCount );
	updateForcesVec( minOne, minOneCount );
	updateForcesVec( minTwo, minTwoCount );
	updateForcesVec( secOne, secOneCount );
	updateForcesVec( secTwo, secTwoCount );
}

void Clock :: updateForcesVec ( vector<ClockCircle*> &circlesVec, int count )
{
	for( int i=0; i<circlesVec.size(); i++ )
	{
		ClockCircle& circle	= *circlesVec[ i ];
		
		if( i < count )
		{
			if( clockMode == CLOCK_MODE_1 )
			{
				if( !circle.hasCenterJoint() )
					circle.createCenterJoint();
				
				addCenterForce( circle );		// spin force.
			}
			else if( clockMode == CLOCK_MODE_2 )
			{
				if( !circle.hasLineupJoint() )
					circle.createLineupJoint();
			}

			circle.active = true;
			circlesActive.push_back( &circle );
		}
		else 
		{
			if( clockMode == CLOCK_MODE_1 )
			{
				if( !circle.hasOuterJoint() )
					circle.createOuterJoint();
				
//				pushFromCenter( circle );
			}
			else if( clockMode == CLOCK_MODE_2 )
			{
				if( circle.hasLineupJoint() )
					circle.destroyJoint();
				
				floatUp( circle );
			}
			
			circle.active = false;
			circlesInactive.push_back( &circle );
		}
		
		circle.update();
	}
}

//-- CM1 FORCES.

void Clock :: addCenterForce ( ClockCircle& circle )
{
	ofxVec2f c;
	ofxVec2f p;
	ofxVec2f v;
	
	c.set( ofGetWidth() * 0.5, ofGetHeight() * 0.5 );
	p.set( circle.getPosition() );
	v = c - p;
	
//	float d;
//	d = v.length() / (float)screenMaxLength;
//	
//	float s;
//	s = circle.getRadius() / 20.0;			// the bigger the circle, the stronger the pull towards center.
	
	ofxVec2f perp;							// spinning force.
	perp = v.getPerpendicular();
	perp *= circle.spinFrc;
	perp *= forceScale;
	
//	v.normalize();		// old pull to center code.
//	v *= d;
//	v *= s;
//	v *= forceCenterPull;
//	v *= forceScale;
//	v += perp;
	
	v = perp;			// now only adding perp force to make circles spin.
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

void Clock :: pushFromCenter ( ClockCircle& circle )
{
	ofxVec2f v;
	v.set( circle.getPosition() );
	v -= screenCenter;
	
	//-- distance to keep from center, follow the shape of the inner blob.

	float d;
	d = 1 - ( v.length() / ( screenMinLength * 0.4 ) );
	
//	float ang;
//	ang = v.angle( ofxVec2f( 0, -1 ) );
//	ang *= -1;
//	ang += 180;
//	ang /= 360.0;
//
//	int ri = (int)( ang * RAY_BLOB_LO_RES );
//	ri = MAX( MIN( ri, RAY_BLOB_LO_RES - 1 ), 0 );
//
//	ofxVec2f rb;
//	rb.set( rayBlob[ ri ] );
//	rb -= screenCenter;
//	
//	ofxVec2f rv;
//	rv.set( 0, -1 );
//	rv.rotateRad( TWO_PI - v.angleRad( ofxVec2f( 0, -1 ) ) );
//	rv *= rb.length();
//	rv *= 1.5;
//	
//	rayPoints.push_back( rv + screenCenter );
//	
//	d = 1 - ( v.length() / ( rv.length() ) );

	//-- perpendicular force to center.
	
	ofxVec2f perp;							// spinning force.
	perp = v.getPerpendicular();
	perp *= circle.spinFrc;
	perp *= circle.spinDir;
	perp *= 3;
	
	//-- add forces.
	
	v.normalize();
	v *= d;
	v *= forceCenterPull;
//	v += perp;
	v *= forceScale;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

//-- CM2 FORCES.

void Clock :: floatUp ( ClockCircle& circle )
{
	float r;
	r = circle.getRadius();
	
	float s;
	s = ( circle.getPosition().y - r ) / ( screenHeight - ( r * 2 ) );
	
	float gx = gravity.x * 2;
	float gy = -30 * s;
	
	b2Vec2 up = b2Vec2( gx, gy );
	up *= forceScale;
	
	circle.body->ApplyImpulse( up, circle.body->GetWorldCenter() );
}

void Clock :: lineUp ( ClockCircle& circle )
{
	ofxVec2f p1;
	ofxVec2f p2;
	ofxVec2f v;
	
	p1.set( circle.lineUpPoint.x * screenWidth, circle.lineUpPoint.y * screenHeight );
	p2.set( circle.getPosition() );
	v = p1 - p2;
	
	float d;
	d = v.length() / (float)screenMaxLength;
	
	v.normalize();
	v *= d;
	v *= 30;
	v *= forceScale;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

//-- RAY BLOB.

void Clock :: updateRayBlob ()
{
	int h = RAY_BLOB_HI_RES;	// high res.
	int l = RAY_BLOB_LO_RES;	// low res - down sample.
	int s = h / l;				// sample.
	
	bool  bVerbose	= false;
	float blobDist	= screenMaxLength * 0.5;
	
	ofxVec2f p1;
	ofxVec2f p2;
	ofxVec2f hitPoint;
	ofxVec2f rayPoint;
	
	p2.set( screenWidth * 0.5, screenHeight * 0.5 );
	
	for( int i=0; i<h; i+=s )
	{
		if( bVerbose && ofGetFrameNum() == 0 )
			cout << "new loop" << endl;
		
		int count			= 0;
		float length		= 0;
		float lengthMax		= 0;
		float lengthTotal	= 0;
		
		for( int j=0; j<s; j++ )
		{
			//-------						// works out the index.
			
			int k = i + j;
			int m = k - (int)( s * 0.5 );
			
			if( m < 0 )
				m += h;
			
			if( m > h - 1 )
				break;
			
			//-------						// prints index for debugging.
			
			if( bVerbose && ofGetFrameNum() == 0 )
				cout << m << endl;
			
			//-------						// works out average length of samples.
			
			float p;
			p = m / (float)h;
			
			p1.set( 0, -1 );
			p1.rotate( p * TWO_PI * RAD_TO_DEG );
			p1 *= blobDist;
			p1 += p2;
			
			hitPoint.set( 0, 0 );
			
			for( int c=0; c<circlesActive.size(); c++ )
			{
				bool bHit;
				
				ClockCircle& circle = *circlesActive[ c ];
				bHit = circle.raycast( p1, p2, &rayPoint );
				
				if( bHit )
				{
					rayPoint -= p2;
					
					if( rayPoint.length() > hitPoint.length() )
					{
						hitPoint.set( rayPoint );
					}
				}
			}
			
			hitPoint += hitPoint.getNormalized() * ( rayBlobPad * screenHeight );
			
			length		= hitPoint.length();
			lengthMax	= ( length > lengthMax ) ? length : lengthMax;
			lengthTotal	+= length;
			count		+= 1;
		}
		
		//-------							// calc average.
		
		float lengthAvg;
		lengthAvg = length / (float)count;

		float p;
		p = i / (float)h;
		
		p1.set( 0, -1 );
		p1.rotate( p * TWO_PI * RAD_TO_DEG );
//		p1 *= lengthAvg;
		p1 *= lengthMax;
		p1 += p2;
		
		int n = i / s;
		ofPoint& rayPoint = rayBlob[ n ];
		
		rayPoint.x += ( p1.x - rayPoint.x ) * rayBlobEase;
		rayPoint.y += ( p1.y - rayPoint.y ) * rayBlobEase;
	}
}

void Clock :: updateConvexBlob()
{
	convexBlobInner.clear();
	convexBlobOuter.clear();

	vector<ClockCircle*>* circles;
	
	//-- inner blob.
	
	circles = &circlesActive;
	
	for( int i=0; i<circles->size(); i++ )
	{
		ClockCircle& circle = *circles->at( i );
		
		ofPoint p1 = circle.getPosition();
		
		for( int j=0; j<circle.pointsTotal; j++ )
		{
			const ofPoint& p2 = circle.points1[ j ];
			
			convexBlobInner.push_back( ofPoint() );
			ofPoint& p3 = convexBlobInner.back();
			
			p3.x = p1.x + p2.x;
			p3.y = p1.y + p2.y;
		}
	}
	
	contourUtil.convexHull( convexBlobInner );
	
	//-- outer blob.
	
	if( clockMode == CLOCK_MODE_1 )
	{
		circles = &circlesAll;
	}
	else
	{
		circles = &circlesInactive;
	}


	for( int i=0; i<circles->size(); i++ )
	{
		ClockCircle& circle = *circles->at( i );
		
		ofPoint p1 = circle.getPosition();
		
		for( int j=0; j<circle.pointsTotal; j++ )
		{
			ofPoint p2;
			
			if( clockMode == CLOCK_MODE_1 )
			{
				p2 = circle.points2[ j ];
			}
			else
			{
				p2 = circle.points1[ j ];
			}

			
			convexBlobOuter.push_back( ofPoint() );
			ofPoint& p3 = convexBlobOuter.back();
			
			p3.x = p1.x + p2.x;
			p3.y = p1.y + p2.y;
		}
	}

	contourUtil.convexHull( convexBlobOuter );
}

//-- CLOCK MODE.

void Clock :: toggleClockMode ()
{
	if( clockMode == CLOCK_MODE_1 )
	{
		clockMode = CLOCK_MODE_2;
		
		initModeTwo();
	}
	else if( clockMode == CLOCK_MODE_2 )
	{
		clockMode = CLOCK_MODE_1;
		
		initModeOne();
	}
}

///////////////////////////////////////////////
//	CONTACT HANDLER.
///////////////////////////////////////////////

void Clock :: box2dContactEventHandler ( const b2ContactPoint* p )
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		
		for( b2Shape* s=circle.body->GetShapeList(); s; s=s->GetNext() )
		{
			if( p->shape1 == s || p->shape2 == s )
			{
				float vel;								// weight colour change by velocity.
				vel = p->velocity.Length();
				vel /= 10;
				vel = ofClamp( vel, 0, 1 );
				
				if( vel > 0.5 )
				{
//					circle.colorCurr.r = circle.colorTrgt.r * ( 1 - vel );
//					circle.colorCurr.g = circle.colorTrgt.g * ( 1 - vel );
//					circle.colorCurr.b = circle.colorTrgt.b * ( 1 - vel );
					
					circle.colorCurr.r = 255;
					circle.colorCurr.g = 255;
					circle.colorCurr.b = 255;
				}
			}
		}
	}
}

///////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////

void Clock :: draw ()
{
//	ofBackground( 249, 246, 229 );		// faint yellow.
	ofBackground( 30, 30, 30 );

	ofSetColor( 255, 255, 255, 25 );
	drawConvexBlob( convexBlobOuter );
	ofSetColor( 255, 255, 255, 25 );
	drawConvexBlob( convexBlobInner );

	drawCircles( circlesInactive );
	if( clockMode == CLOCK_MODE_1 )
		drawCircleLines( circlesInactive );
	
	drawCircles( circlesActive );
//	if( clockMode == CLOCK_MODE_1 )
//		drawCircleLines( circlesActive );
	
	if( softBody != NULL )
		softBody->draw();
	
	drawTime();
}

void Clock :: drawCircles ( vector<ClockCircle*>& circles )
{
	for( int i=0; i<circles.size(); i++ )
	{
		drawCircle( *circles[ i ] );
	}
}

void Clock :: drawCircle ( ClockCircle &circle )
{
	ofFill();
	ofSetColor( circle.colorCurr.r, circle.colorCurr.g, circle.colorCurr.b );
	
	circle.draw();
	
	ofEnableSmoothing();
	
	ofNoFill();
	ofSetColor( circle.colorCurr.r, circle.colorCurr.g, circle.colorCurr.b );
	
	circle.draw();
	
	ofDisableSmoothing();
}

void Clock :: drawCircleLines ( vector<ClockCircle*>& circles )
{
	for( int i=0; i<circles.size(); i++ )
	{
		drawCircleLine( *circles[ i ] );
	}
}

void Clock :: drawCircleLine ( ClockCircle &circle )
{
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	ofSetColor( 255, 255, 255, 30 );
	ofLine( screenCenter.x, screenCenter.y, circle.eye.x, circle.eye.y );
	
	ofDisableSmoothing();
	ofDisableAlphaBlending();
}

void Clock :: drawTime ()
{
	if( font == NULL )
		return;
	
	int blackH = 80;
	ofFill();
	ofEnableAlphaBlending();
	ofSetColor( 0, 0, 0, 120 );
	ofRect( 0, screenHeight - blackH, screenWidth, blackH );
	ofSetColor( 255, 255, 255, 120 );
	ofRect( 0, screenHeight - blackH - 1, screenWidth, 1 );
	ofDisableAlphaBlending();
	
	ofSetColor( 0xFFFFFF );
	
	int fontY = screenHeight - 30;
	font->drawString( ofToString( hrsOneCount, 0 ), hrsOneX, fontY );
	font->drawString( ofToString( hrsTwoCount, 0 ), hrsTwoX, fontY );
	font->drawString( ofToString( minOneCount, 0 ), minOneX, fontY );
	font->drawString( ofToString( minTwoCount, 0 ), minTwoX, fontY );
	font->drawString( ofToString( secOneCount, 0 ), secOneX, fontY );
	font->drawString( ofToString( secTwoCount, 0 ), secTwoX, fontY );
}

void Clock :: drawRayCasts ()
{
	int t = RAY_BLOB_LO_RES;
	
	ofEnableAlphaBlending();

	for( int i=0; i<t; i++ )
	{
		float p;
		p = i / (float)t;
		
		if( p > 1.0 )	// just to test its being drawn correctly.
			return;
		
		ofxVec2f v;
		v.set( 0, -1 );
		v.rotate( p * TWO_PI * RAD_TO_DEG );
		v *= screenMinLength * 0.4;
		
		ofxVec2f cp;
		cp.set( screenWidth * 0.5, screenHeight * 0.5 );

		ofNoFill();
		ofSetColor( 255, 0, 0, 100 );
		ofLine( cp.x, cp.y, cp.x + v.x, cp.y + v.y );
		
		ofPoint& rayPoint = rayBlob[ i ];
		
		ofFill();
		ofSetColor( 0, 255, 0, 100 );
		ofCircle( rayPoint.x, rayPoint.y, 4 );
	}
	
	ofDisableAlphaBlending();
}

void Clock :: drawRayBlob ()
{
	int t = RAY_BLOB_LO_RES;

	ofNoFill();
	ofSetColor( 255, 255, 255, 100 );
	ofSetLineWidth( 2 );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	ofBeginShape();
	
	for( int i=0; i<t+3; i++ )
	{
		int j = i % t;
		
		ofPoint& rayPoint = rayBlob[ j ];
		
		ofCurveVertex( rayPoint.x, rayPoint.y );
	}
	
	ofEndShape( true );
	
	ofSetLineWidth( 1 );
	ofDisableSmoothing();
	ofDisableAlphaBlending();
}

void Clock :: drawConvexBlob ( const vector<ofPoint>& points )
{
	ofFill();
	ofSetLineWidth( 2 );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	ofBeginShape();
	
	bool bCurve = false;

	int t = points.size();
	
	if( bCurve )
		t += 3;
	
	for( int i=0; i<t; i++ )
	{
		int j = i % t;
		
		if( bCurve )
		{
			ofCurveVertex( points[ j ].x, points[ j ].y );
		}
		else
		{
			ofVertex( points[ j ].x, points[ j ].y );
		}
	}
	
	ofEndShape( true );
	
	ofSetLineWidth( 1 );
	ofDisableSmoothing();
	ofDisableAlphaBlending();
}
