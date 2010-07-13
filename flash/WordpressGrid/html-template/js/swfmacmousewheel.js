/**
 * SWFMacMouseWheel v1.0: Mac Mouse Wheel functionality in flash - http://blog.pixelbreaker.com/
 *
 * SWFMacMouseWheel is (c) 2006 Gabriel Bucknall and is released under the MIT License:
 * http://www.opensource.org/licenses/mit-license.php
 *
 * Dependencies: 
 * SWFObject v2.0 - (c) 2006 Geoff Stearns.
 * http://blog.deconcept.com/swfobject/
 */
function SWFMacMouseWheel( swfObject )
{
	this.so = swfObject;
	var isMac = navigator.appVersion.toLowerCase().indexOf( "mac" ) != -1;
	if( isMac ) this.init();
}

SWFMacMouseWheel.prototype = {
	init: function()
	{
		SWFMacMouseWheel.instance = this;
		if (window.addEventListener)
		{
		}
		window.onmousewheel = document.onmousewheel = SWFMacMouseWheel.instance.wheel;
	},
	
	handle: function( delta )
	{
		document[ this.so.getAttribute('id') ].externalMouseEvent( delta );
	},

        var delta = 0;
        if( /AppleWebKit/.test(navigator.userAgent) ) {
        	delta /= 3;	
        }
};