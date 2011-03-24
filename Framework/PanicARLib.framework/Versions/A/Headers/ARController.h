//
//  ARController.h
//  PanicAR
//
//  Created by Andreas Zeitler on 17.08.10.
//  Copyright 2010 doPanic. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <MobileCoreServices/UTCoreTypes.h>
#import <MobileCoreServices/UTType.h>

#import "ARConstants.h"
#import "ARUtils.h"
#import "ARView.h"
#import "ARControllerDelegate.h"
#import "ARMarker.h"
#import "ARMarkerTemplate.h"

@class ARController;
@class ARMarker;
@class ARMarkerTemplate;
@class ARView;

#pragma mark -
#pragma mark Interface

/*! @class ARController
 @brief main controller of AR functionality
 
 the ARController creates and controls the CameraView, the OpenGL 3D View and the LocationManager
 it will handle all Augmented Reality calculations and the drawing of the markers
 */
@interface ARController : UIViewController <CLLocationManagerDelegate, UIAccelerometerDelegate> {
    
@private
    BOOL demoMode;
    
	BOOL isVisible;
	BOOL isBeingUpdated;
	BOOL isModalView;
	BOOL wasModalView;
    BOOL readyForRendering;
    BOOL hasBeenVisibleBefore;
	BOOL freeze;
    
	BOOL freeMemoryOnWarning;
    
    BOOL locationServicesAvailableLastTime;
    NSError* locationFailError;
	
	BOOL isSupported;
	
    BOOL hasLocation;
    BOOL needsSorting;
    BOOL needsGeneralRefresh;
	
	double directionFromAccelerometer;
	double xRoll;
	double yRoll;
	double zRoll;
	double pitch;
	double roll;
	float accelerationSign;
	float rotationSign;
	BOOL isLandscapeView;
	
	UIDeviceOrientation arOrientation;
	float osVersion;
	
	//markers
	NSMutableArray *markers;
	NSArray *sortedMarkers;
	
	//base components
	UIImagePickerController* cameraView; // live camera feed 
	CGRect viewport;
	float contentScale;
	UIView* touchView;
	UIView* arContainer;
	
	UIAccelerometer *accelerometer;
	CLLocationManager	*locationManager;
	CLLocation	*currentLocation;
	double currentHeading;
	
	BOOL hasTimer;
	NSTimer* m_timer;
	
	BOOL _markerSectors[k_Sectors][100];
	int _markerSectorsY[k_Sectors];
	
	UIView* mainView;
	UIButton *hideButton;
	UIButton *radarButton;
	UILabel *infoLabel;
    
	UIView* loadingView;
	UILabel *loadingLabel;
    
	UILabel *watermark;
}


#pragma mark -
#pragma mark Properties
//is active
/*! @property is ARView visible?  */
@property (readonly) BOOL isVisible;
@property (readonly) BOOL freeze;
@property (readonly) BOOL readyForRendering;
//camera
/*! @property camera view pointer  */
@property (readonly) UIImagePickerController* cameraView;
//location and bearing
/*! @property location manager  */
@property (readonly) CLLocationManager	*locationManager;
/*! @property current device heading  */
@property (assign) double currentHeading;
/*! @property does AR system have location?  */
@property (readonly) BOOL hasLocation;
/*! @property current device location  */
@property (nonatomic, copy) CLLocation	*currentLocation;
//accelerometer
@property (assign) double directionFromAccelerometer;
//views and viewport
@property (readonly) UIView* touchView;
@property (readonly) CGRect viewport;
@property (readonly) float contentScale;
@property (readonly) BOOL isLandscapeView;
@property (readonly) float accelerationSign;
@property (readonly) float rotationSign;
//markers and sortin
@property (assign) BOOL needsSorting;
@property (readonly) NSMutableArray	*markers;
//outlets
@property (nonatomic, retain) IBOutlet UIButton *hideButton;
@property (nonatomic, retain) IBOutlet UIButton *radarButton;
@property (nonatomic, retain) IBOutlet UILabel *infoLabel;
@property (nonatomic, retain) IBOutlet UIView* loadingView;
@property (nonatomic, retain) IBOutlet UILabel *loadingLabel;

//actions
- (IBAction)doHideButton;
- (IBAction)doRadarButton;


#pragma mark -
#pragma mark Class Properties


-(ARView*) sharedView;

-(BOOL) enableCameraView;
-(BOOL) enableAccelerometer;
-(BOOL) enableAccelerometer;
-(BOOL) enableAutoswitchToRadar;
-(BOOL) enableInteraction;
-(BOOL) enableViewOrientationUpdate;
-(BOOL) enableMovieCapture;
-(BOOL) enableLoadingView;
-(BOOL) enableContinuousGPS;
-(BOOL) isDemoMode;
-(UIDeviceOrientation) defaultOrientation;
-(id<ARControllerDelegate>) delegate;

-(float) CameraViewTransformX;
-(float) CameraViewTransformX;

-(float) RadarPositionX;
-(float) RadarPositionY;

-(float) CameraViewTintR;
-(float) CameraViewTintG;
-(float) CameraViewTintB;
-(float) CameraViewTintA;

-(float) MinimumRange;
-(float) MaximumRange;

-(UIViewAnimationTransition) FadeInAnim;
-(UIViewAnimationTransition) FadeOutAnim;

-(ARMarkerTemplate*) defaultMarkerTemplate;
-(Mesh) defaultMarkerMesh;
-(Mesh) radarMesh;
-(uint) radarDot;
-(uint) radarBG;
-(Mesh) coordinatesMesh;
-(uint) coordinatesTexture;
-(ARMarker*) lastTappedMarker;
-(ARMarker*) tappedMarker;
-(BOOL) needsStatusBar;
-(UIStatusBarStyle) savedStatusBarStyle;
-(float) lastDeltaTime;
-(void) setLastDeltaTime:(float)value;
-(BOOL) inRadarMode;



#pragma mark -
#pragma mark Public Isntance Methods

/*!  create ARController with startup parameters
 
 initinitWithNibName: 
 @param nibNameOrNil name of Interface File (nib or xib) without extension
 @param nibBundleOrNil bundle to load Interface File from
 
 allocates and initializes a new ARController instance with previously defined settings.
 The instance will be returned and will be available in the class field 'sharedController'
 */
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil;

/*!  create ARController with startup parameters, pass a custom delegate object
 
 initWithNibName: 
 @param nibNameOrNil name of Interface File (nib or xib) without extension
 @param nibBundleOrNil bundle to load Interface File from
 @param delegate the delegate instance to receive events from the ARController
 
 allocates and initializes a new ARController instance with previously defined settings.
 The instance will be returned and will be available in the class field 'sharedController'
 */
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil delegate:(id<ARControllerDelegate>)_delegate;


/*!  suspend ARController */
- (void) suspendToBackground;
/*!  resume ARController */
- (void) resumeFromBackground;
/*!  free as much memory as possible (mainly by releasing OGL memory occupied by ARMarker textures) */
- (void) freeMemory;

- (void) failWithErrorCodeImmediately:(int)code;
- (void) failWithErrorCodeDelayed:(int)code;
- (BOOL) startLocationServicesIfNeccessary:(BOOL)displayError;
- (void) stopLocationServices:(BOOL)force;

//marker management

/*!  adds the ARMarker at the given location
 
 */
- (ARMarker*)addMarkerAtLocation:(ARMarker*)marker atLocation:(CLLocation*)cllocation;

/*!  adds the ARMarker as a virtual object (without geolocation information)
 
 */
- (ARMarker*)addMarkerAsVirtual:(ARMarker*)marker angle:(float)angle distance:(float)distance;

/*!  remove a ARMarker object
 
 */
- (void) removeMarker:(ARMarker*)marker;

/*!  remove all ARMarkers from the controller
 
 */
- (void) clearMarkers;

/*!  returns the number of ARMarkers currently added to the Controller
 
 */
- (int) numberOfMarkers;


//internal use only
- (BOOL) markerSector:(int)y sector:(int)i;
- (int) markerSector:(int)i;
- (void) occupyMarkerSector:(int)y sector:(int)i;
- (void) sortMarkers;

//view management

/*!  set the viewport area of the Augmented Reality view
 @param rect new viewport area
 */
- (void) setViewport:(CGRect)rect;

/*!  show the ARController view
 @param asModal show as modal view?
 @param showStatusBar show statusbar?
 */
- (BOOL) showController:(BOOL)asModal showStatusBar:(BOOL)showStatusBar;
/*!  show the ARController view
 @param asModal show as modal view?
 @param showStatusBar show statusbar?
 @param displayError show alert for errors that occur?
 
 @return YES if controller can be shown, NO if error occured (e.g. location not available)
 */
- (BOOL) showController:(BOOL)asModal showStatusBar:(BOOL)showStatusBar;

/*!  hide the ARController view */
- (void) hideController;

/*!  show the radar view within the ARController view */
- (void) showRadar;
/*!  hide the radar view within the ARController view */
- (void) hideRadar;

//interaction
- (void) doTouchEvent:(CGPoint)point;




#pragma mark -
#pragma mark Class Propertie Methods


/*! 
  CONFIGURATION: set API key
 
 the Framework will run in DEMO Mode unless it receives a valid API key
 can only be set before ARController is initialized
 
 contact sales@dopanic.com for a valid API key
 
 default: ""
 */
+ (void)setAPIKey:(NSString*)key;


/*! 
  CONFIGURATION: use movie capture mode of camera for better image (but with ipod playback interruption while ARView is visible)
 @param state set to YES or NO
 
 can only be set before ARController is initialized
 
 disabling will also disable EnableAutoswitchToRadar and EnableViewOrientationUpdate
 
 default: NO
 */
+ (void)setEnableAccelerometer:(BOOL)state;


/*! 
  CONFIGURATION: use live camera view
 @param state set to YES or NO
 
 can be disabled for special use cases
 
 can only be set before ARController is initialized
 
 disabling will also disable EnableMovieCapture
 
 default: YES
 */
+ (void)setEnableCameraView:(BOOL)state;


/*! 
  CONFIGURATION: use movie capture mode of UIImagePickerController
 @param state set to YES or NO
 
 needs EnableCameraView to be YES
 
 can only be set before ARController is initialized
 
 if enabled, UIImagePickerController will use movie capture mode which will
 create a better preview image of the camera, but will cause iPod playback to be interrupted
 as soon as the ARController is shown
 
 default: NO
 */
+ (void)setEnableMovieCapture:(BOOL)state;


/*! 
  CONFIGURATION: use the top-down radar-style rendering of the camera view
 @param state set to YES or NO
 
 can only be set before ARController is initialized
 
 default: NO
 */
+ (void)setEnableRadar:(BOOL)state;


/*! 
  CONFIGURATION: switch to radar based on device orientation (OrientationFaceUp)
 @param state set to YES or NO
 
 can only be set before ARController is initialized
 
 default: NO
 */
+ (void)setEnableAutoswitchToRadar:(BOOL)state;


/*! 
  CONFIGURATION: update ARController's view orientation to device' orientation
 @param state set to YES or NO
 
 needs EnableAccelerometer to be YES
 
 will only change the content of the OpenGL-rendered view, not the orientation of any view in the controller or of the controller
 itself
 
 default: NO
 */
+ (void)setEnableViewOrientationUpdate:(BOOL)state;


/*! 
  CONFIGURATION: enable touch events on Markers and scrolling through marker-stacks
 @param state set to YES or NO
 
 default: YES
 */
+ (void)setEnableInteraction:(BOOL)state;


/*! 
  CONFIGURATION: enable screens to cover up AR view transitions (e.g. iris animation of UIImagePickerController)
 @param state set to YES or NO
 
 default: YES
 */
+ (void)setEnableLoadingView:(BOOL)state;


/*! 
  CONFIGURATION: by default locationManager does not do updates when ARView not shown, set this setting to YES to update the locationManager continuosly
 @param state set to YES or NO
 
 default: NO
 */
+ (void)setEnableContinuousGPS:(BOOL)state;


/*! 
  CONFIGURATION: the default orientation of the ARController's view
 @param orientation the default device orientation
 
 set to Landscape if default view is landscape, otherwise set to portrait
 
 default: UIDeviceOrientationPortrait
 */
+ (void)setDefaultOrientation:(UIDeviceOrientation)orientation;

/*! 
  CONFIGURATION: set the delegate object to handle all ARController events
 @param _delegate the delegate you want to use
 */
+ (void)setDelegate:(id<ARControllerDelegate>)_delegate;

/*! 
  CONFIGURATION: set the fade in animation transition used when displaying the ARController as a modal view
 @param anim the animation style used for showing the AR View
 
 default: UIViewAnimationTransitionFlipFromLeft
 */
+ (void)setFadeInAnim:(UIViewAnimationTransition)anim;

/*! 
  CONFIGURATION: set the fade out animation transition used when displaying the ARController as a modal view
 @param anim the animation style used for hiding the AR View
 
 default: UIViewAnimationTransitionFlipFromRight
 */
+ (void)setFadeOutAnim:(UIViewAnimationTransition)anim;

/*! 
  CONFIGURATION: set the camera's live view transformation (to achieve full screen camera view)
 @param x the width transformation of the camera's live view
 @param y the height transformation of the camera's live view
 
 default for iPhone4: (1.25,1.25)
 */
+ (void)setCameraTransform:(float)x y:(float)y;

/*! 
  CONFIGURATION: set a background color for the OpenGL view (rendered above the Camera's live feed)
 @param r red component of background color
 @param g green component of background color
 @param b blue component of background color
 @param a alpha component of background color
 
 can be used to increased contrast (when set to black with low alpha-value
 
 default: (0,0,0,0)
 */
+ (void)setCameraTint:(float)r g:(float)g b:(float)b a:(float)a;

/*! 
  CONFIGURATION: set the center of the radar on screen
 @param x the sideways offset of the radar screen
 @param y the up-down offset of the radar screen
 
 default: (0,0)
 */
+ (void)setRadarPosition:(float)x y:(float)y;

/*! 
  CONFIGURATION: set the min range and max range for markers to be displayed
 @param min markers closer to the device than min are not shown on screen
 @param max markers farther away than max are not shown on screen
 
 set to -1 to remove limit
 
 default: (5,20000)
 */
+ (void)setRange:(float)min Maximum:(float)max;

#pragma mark -
#pragma mark Public Class Methods

/*! 
 checks if the device supports Augmented Reality functionality
 
 Returns YES if GPS, Compass and Camera are available
 i.e. if the hardware model is iPhone 3Gs or later
 */
+ (BOOL)deviceSupportsAR;

/*! 
 checks if location services are available/allowed for the device/user
 
 only checks if location services are turned on and the app is authorized to use them
 will not take into account if Airplane Mode is turned on
 
 use deviceSupportsAR to check if device supports camera and location services in general
 
 Returns YES if location services are available to app
 */
+ (BOOL)locationServicesAvailable:(BOOL)displayError;

/*! 
  load a mesh for use in the AR view (pass mesh as pointer to Mesh struct)
 
 supports loading of OBJ-files, specs: triangulated, single UV set, need to have material
 please note that popular 3d model tools may export OBJ without UVs when using materials without textures
 
 make sure mesh file is added to XCode first
 
 @return YES if mesh successfully loaded, otherwise NO
 */
+ (BOOL) loadMesh:(NSString*)meshFilename mesh:(Mesh&)mesh;

/*! 
  load a texture into OpenGL for use in the AR view
 
 loads all image formats supported by the SDK and creates a OpenGL texture name for it
 
 @return the OpenGL texture name (uint)
 */
+ (uint) loadTexture:(NSString*)textureFilename;


/*! 
  returns the current AR controller
 */
+ (ARController*) sharedController;

@end