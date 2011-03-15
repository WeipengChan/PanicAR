//
//  PanicARDemoAppDelegate.mm
//  PanicAR Demo
//
//  Created by Andreas Zeitler on 9/1/11.
//  Copyright doPanic 2011. All rights reserved.
//

#import "PanicARDemoAppDelegate.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif


@implementation PanicARDemoAppDelegate

@synthesize window;
@synthesize tabBarController;


// application delegate method
- (void)applicationDidFinishLaunching:(UIApplication *)application {	
    tabBarController.delegate = self;
    [self.window addSubview:tabBarController.view];
	[window makeKeyAndVisible];
	
	[self createAR];
	[self createMarkers];
    
	[self showAR];
}

// standard dealloc of the delegate
- (void)dealloc {
	if (m_ARController != nil) [m_ARController release];
	[window release];
	[super dealloc];
}



// create the ARController
- (void) createAR {
	//setup ARController properties
	[ARController setEnableCameraView:YES];
	[ARController setEnableRadar:YES];
	[ARController setEnableInteraction:YES];
	[ARController setEnableAccelerometer:YES];
	[ARController setEnableAutoswitchToRadar:YES];
	[ARController setEnableViewOrientationUpdate:YES];
	[ARController setFadeInAnim:UIViewAnimationTransitionCurlDown];
	[ARController setFadeOutAnim:UIViewAnimationTransitionCurlUp];
	[ARController setCameraTint:0 g:0 b:0 a:0];
	[ARController setCameraTransform:1.25 y:1.25];
	
	//create ARController
	m_ARController = [[ARController alloc] initWithDelegate:self];
	
	[[tabBarController.viewControllers objectAtIndex:0] setView:nil];
	
#if (TARGET_IPHONE_SIMULATOR)
	// returns nil if AR not available on device
	if (m_ARController) {
		// simulator testing coordinates
		m_ARController.myLocation = [[CLLocation alloc] initWithLatitude:49.009860 longitude:12.108049];
	}
#endif
}

// create a few test markers
- (void) createMarkers {
    // first: setup a new marker with title and content
    ARMarker* newMarker = [[ARMarker alloc] initWithTitle:@"New York City" contentOrNil:@"New York, United States"];
    
    // second: add the marker to the ARController using the addMarkerAtLocation method
    // pass the geolocation (latitude, longitude) that specifies where the marker should be located
    // WARNING: use double-precision coordinates whenever possible (the following coordinates are from Google Maps which only provides 8-9 digit coordinates
	[m_ARController addMarkerAtLocation: newMarker atLocation:[[[CLLocation alloc] initWithLatitude:40.708231 longitude:-74.005966] autorelease]];
    
    
    // add a second marker
    newMarker = [[ARMarker alloc] initWithTitle:@"Berlin" contentOrNil:@"Germany"];
    [m_ARController addMarkerAtLocation:newMarker atLocation:[[[CLLocation alloc] initWithLatitude:52.523402 longitude:13.41141] autorelease]];
    
    // add a third marker, this time allocation of a new marker and adding to the ARController are wrapped up in one line
	[m_ARController addMarkerAtLocation:[[ARMarker alloc] initWithTitle:@"London" contentOrNil:@"United Kingdom"] atLocation:[[[CLLocation alloc] initWithLatitude:51.500141 longitude:-0.126257] autorelease]];
}

// display the ARView in the tab bar (non-modal)
- (void) showAR {
    // on DEVICE: show error if device does not support AR functionality
    // AR is not supported if either camera or compass is not available
#if !(TARGET_IPHONE_SIMULATOR)
    if (![ARController deviceSupportsAR]) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"No AR Error Title", @"No AR Support")
                                                        message:NSLocalizedString(@"No AR Error Message"@, "This device does not support AR functionality!") 
                                                       delegate:nil 
                                              cancelButtonTitle:NSLocalizedString(@"OK Button", @"OK") 
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
#endif
    
    // check if ARController instance is valid
    if (m_ARController == nil) {
        NSLog(@"No ARController available!");
        return;
    }
    
    // show AR Controller in tab bar by assigning the ARView to the first view controller of the tab bar
    [[tabBarController.viewControllers objectAtIndex:0] setView:m_ARController.view];
    // now tell the ARController to become visbiel in a non-modal way while keeping the status bar visible
    // NOTE: the camera feed will mess with the status bar's visibility while being loaded, so far there is no way to avoid that (iOS SDK weakness)
    [m_ARController showController:NO showStatusBar:YES];
    // when showing the ARView non-modal the viewport has to be set each time it becomes visible in order to avoid positioning and resizing problems
    [m_ARController setViewport:CGRectMake(0, 0, 320, 411)];
    
    NSLog(@"ARView selected in TabBar");
}

// marker interaction delegate
// called when the AR view is tapped
// marker is the marker that was tapped, or nil if none was hit
- (void) markerTapped:(ARMarker*)marker {
	if (marker != nil) {
		marker.touchDownColorR = 1;
		marker.touchDownColorG = 0.5;
		marker.touchDownColorB = 0.5;
		marker.touchDownColorA = 1;
		marker.touchDownScale = 1.25;
		
		NSLog(@"markerClicked: %@", marker.title);
		m_ARController.infoLabel.text = [[[NSString alloc] initWithFormat:@"%@ - %@", marker.title, marker.content] autorelease];
	}
	else m_ARController.infoLabel.text = [[[NSString alloc] initWithFormat:@""] autorelease];
}



// tab bar delegate method, switches the views displayed by the app
- (void)tabBarController:(UITabBarController *)tabBar didSelectViewController:(UIViewController *)viewController {
	if ([tabBarController.viewControllers indexOfObject:viewController] == 0) {
		[self showAR];
	}
	else {
		[[tabBarController.viewControllers objectAtIndex:0] setView:nil];
		[m_ARController hideController];
	}
	
}

// about dialog weblink action
- (IBAction) webButton_click {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: @"http://www.dopanic.com/ar"]];
}

@end