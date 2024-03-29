#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxFlash.h"
#include "Btn.h"

#include "NawlzImage.h"
#include "NawlzFluid.h"
#include "NawlzBlah.h"
#include "NawlzSand.h"
#include "NawlzFishBowl.h"
#include "NawlzInfodel.h"
#include "NawlzQuestions.h"
#include "NawlzJackInBox.h"
#include "NawlzGardener.h"
#include "NawlzDandelion.h"

#define NAWLZ_FLUID_APP			0
#define NAWLZ_BLAH_APP			1
#define NAWLZ_SAND_APP			2
#define NAWLZ_FISH_BOWL_APP		3
#define NAWLZ_INFODEL_APP		4
#define NAWLZ_QUESTIONS_APP		5
#define NAWLZ_JACKINBOX_APP		6
#define NAWLZ_GARDENER_1_APP	7
#define NAWLZ_GARDENER_2_APP	8
#define NAWLZ_GARDENER_3_APP	9
#define NAWLZ_GARDENER_4_APP	10
#define NAWLZ_DANDELION_APP     11

class testApp : public ofxiPhoneApp {
	
public:
	
	void setup	();
	void update	();
	void draw	();
	void exit	();
	
	void loadApp		( int appID );
	void loadImage		( string path, NawlzImage& imageOut );
	
	void touchDown		( int x, int y, int id );
	void touchMoved		( int x, int y, int id );
	void touchUp		( int x, int y, int id );
	void touchDoubleTap	( int x, int y, int id );

	void lostFocus					();
	void gotFocus					();
	void gotMemoryWarning			();
	void deviceOrientationChanged	(int newOrientation);

	ofxFlashXFL*	xfl;
	ofxFlashStage*	stage;
	vector<Btn*>	btns;
    vector<int>     btnIDs;
    Btn*            btnSelected;
	ofBaseApp*		nawlzApp;
	int				appID;
    NawlzImage*     img;

};


