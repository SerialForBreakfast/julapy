////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.view
{
	import com.holler.containers.ScreenView;
	import com.holler.twoframe.events.ImagesChangeEvent;
	import com.holler.twoframe.model.ModelLocator;
	import com.holler.twoframe.view.menu.MenuView;
	import com.holler.twoframe.view.stereo.StereoView;
	
	import flash.display.Sprite;
	import flash.events.Event;

	public class ApplicationView extends ScreenView
	{
		private var loaderView:PreloaderView;
		private var menuView:MenuView;
		private var stereoView:StereoView;
		
		public function ApplicationView(sprite:Sprite=null)
		{
			super(sprite);
			
			
			loaderView = PreloaderApplication.getInstance().loaderView;

			ModelLocator.getInstance().configModel.addEventListener( ImagesChangeEvent.IMAGES_CHANGE, configModel_imagesChangeHander );
		}
		
		/**
		 *  This function is called after the initialiseVO has been set in the ConfigModel
		 *  This is the perfect place to start coding your application
		 */
		private function configModel_imagesChangeHander( event:ImagesChangeEvent ):void
		{
			ModelLocator.getInstance().configModel.removeEventListener( ImagesChangeEvent.IMAGES_CHANGE, configModel_imagesChangeHander );
			
			loaderView.remove();
			
			initMenuView();
		}
		
		//////////////////////////////////////////////////////
		//	MENU VIEW
		//////////////////////////////////////////////////////
		
		private function initMenuView ( e:*=null ):void
		{
			var menuViewAsset:Sprite;
			
			menuViewAsset	= new Sprite();
			menuView		= new MenuView( menuViewAsset );
			menuView.addEventListener( Event.SELECT, onMenuViewClick );
			
			_sprite.addChild( menuViewAsset );
		}
		
		private function killMenuView ( e:*=null ):void
		{
			menuView.removeEventListener( Event.SELECT, onMenuViewClick );
			menuView.remove();
			menuView = null;
		}
		
		private function onMenuViewClick ( e:Event ):void
		{ 
			initStereoView();
		}
		
		//////////////////////////////////////////////////////
		//	STEREO VIEW
		//////////////////////////////////////////////////////
		
		private function initStereoView ( e:*=null ):void
		{
			var stereoViewAsset:Sprite;
			
			stereoViewAsset = new Sprite();
			stereoView		= new StereoView( stereoViewAsset, menuView.selectedItemImageVO.copy() );
			stereoView.addEventListener( Event.COMPLETE, killStereoView );
			
			_sprite.addChild( stereoViewAsset );
		}
		
		private function killStereoView ( e:*=null ):void
		{
			
		}
	}
}