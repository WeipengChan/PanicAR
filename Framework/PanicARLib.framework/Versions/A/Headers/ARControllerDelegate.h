//
//  ARControllerDelegate.h
//  PanicAR
//
//  Created by Andreas Zeitler on 01.09.10.
//  Copyright 2010 doPanic. All rights reserved.
//



@class ARMarker;

/*! 
 @protocol ARControllerDelegate
 @brief protocol that handles ARController callbacks
 
 the ARControllerDelegate enables any class implementing it
 to receive events from the ARController instance (like the touch event of an ARMarker)
 */
@protocol ARControllerDelegate


/** Marker Interaction Callback 
 sent to the delegate when a marker is tapped
 */
- (void) markerTapped:(ARMarker*)marker;


/** AR System Error Callback 
 sent to the delegate when an error occured
 this will most likely be caused by an error in the locationManager
 
 error types:
 kCLErrorLocationUnknown
 kCLErrorDenied
 kCLErrorNetwork
 */
- (void) didFailWithErrorCode:(int)code;


/** Info Update Callback
 sent to delegate when location or heading changes
 use this to change the output in the infoLabel or to perform other output functions
 */
- (void) infoLabelUpdate;

@end