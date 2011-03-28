//
//  NawlzJackInBoxPart.cpp
//  emptyExample
//
//  Created by lukasz karluk on 28/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzJackInBoxPart.h"

NawlzJackInBoxPart :: NawlzJackInBoxPart ()
{
    forceMag = 0;
}

NawlzJackInBoxPart :: ~NawlzJackInBoxPart ()
{
    //
}

void NawlzJackInBoxPart :: setup( const ofPoint& p, float length, float angle )
{
    pStr            = p;
    this->length    = length;
    this->angle     = angle;
    
    float rad;
    rad = ( angle / 360 ) * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
    
    pEnd.x = pStr.x + length * cos( rad );
    pEnd.y = pStr.y + length * sin( rad );
    
    pointDir.x = pEnd.x - pStr.x;
    pointDir.y = pEnd.y - pStr.y;
    
    bounce.center( angle );
    bounce.position( angle );
}

void NawlzJackInBoxPart :: update ()
{
    bounce.update();
    
    float rad;
    rad = ( bounce.position() / 360 ) * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
    
    pEnd.x = pStr.x + length * cos( rad );
    pEnd.y = pStr.y + length * sin( rad );
    
    forceMag *= 0.999;
}

void NawlzJackInBoxPart :: updateForce ( const ofPoint& v )
{
    float x = pEnd.x - v.x;
    float y = pEnd.y - v.y;
    float m = ofDist( 0, 0, v.x, v.y );     // magnitude.

    if( m < forceMag )
        return;
    
    forcePoint.x    = x;
    forcePoint.y    = y;
    forceMag        = m;
    
    forceVec = pEnd - forcePoint;
    forceVec.normalize();
    
    float ang = pointDir.angleRad( forceVec );
    forceScale = sin( ( ang * 2 ) - PI * 0.5 ) * 0.5 + 0.5;
    forceDir    = ( ang < 0 ) ? -1 : 1;
    
    bounce.velocity( forceMag * forceDir * forceScale );
}

void NawlzJackInBoxPart :: updateForce ( int x, int y )
{
    forcePoint.x = x;
    forcePoint.y = y;
    
    forceVec = pEnd - forcePoint;
    forceVec.normalize();
    
    float ang = pointDir.angleRad( forceVec );
    forceScale = sin( ( ang * 2 ) - PI * 0.5 ) * 0.5 + 0.5;
    forceDir    = ( ang < 0 ) ? -1 : 1;
    
    forceVec *= 50 * forceScale;
}

void NawlzJackInBoxPart :: draw ()
{
    ofNoFill();
    ofSetColor( 0xFF00FF );
    ofLine( pStr.x, pStr.y, pEnd.x, pEnd.y );
    
    ofFill();
    ofSetColor( 0xFF0000 );
    ofCircle( pEnd.x, pEnd.y, 5 );

    ofNoFill();
    ofSetColor( 0x0000FF );
    ofLine( pEnd.x, pEnd.y, pEnd.x + forceVec.x, pEnd.y + forceVec.y );
}

void NawlzJackInBoxPart :: mouseMoved ( int x, int y )
{
//    updateForce( x, y );
}

void NawlzJackInBoxPart :: mouseDragged ( int x, int y, int button )
{
//    updateForce( x, y );
}

void NawlzJackInBoxPart :: mousePressed ( int x, int y, int button )
{
//    bounce.velocity( 10 * forceDir * forceScale );
}

void NawlzJackInBoxPart :: mouseReleased ( int x, int y, int button )
{
    //
}
