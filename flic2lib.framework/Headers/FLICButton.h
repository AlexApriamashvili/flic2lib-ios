//
//  FLICButton.h
//  flic2lib
//
//  Created by Anton Meier on 2019-04-11.
//  Copyright © 2019 Shortcut Labs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FLICEnums.h"

NS_ASSUME_NONNULL_BEGIN

@protocol FLICButtonDelegate;

/*!
 *  @class FLICButton
 *
 *  @discussion     An instance of this class represents a physical Flic.
 *
 */
@interface FLICButton : NSObject

/*!
 *  @property identifier
 *
 *  @discussion     This identifier is guaranteed to be the same for each Flic paired to a particular iOS device. Thus it can be used to identify a Flic within an app.
 *                  However, If you need to identify Flics cross different apps on different iOS devices, then you should have look at the either uuid, serialNumber, or bluetoothAddress.
 *
 */
@property(readonly, nonatomic, strong, nonnull) NSUUID *identifier;

/*!
 *  @property delegate
 *
 *  @discussion     The delegate that will receive events related to this particular Flic.
 *                  You can either set this delegate manually for each button, or let the manager do so automatically using the buttonDelegate as default.
 *
 */
@property(weak, nonatomic, nullable) id<FLICButtonDelegate> delegate;

/*!
 *  @property name
 *
 *  @discussion     The bluetooth advertisement name of the Flic. This will be the same name that is shown by iOS it its bluetooth settings.
 *
 */
@property(nonatomic, readonly, strong, nullable) NSString *name;

/*!
 *  @property nickname
 *
 *  @discussion     With this property you can read out the display name that the user may change in for example the Flic app. This value can also be changed from third party apps
 *                  integrating this framework. The purpose of this is to provide more human readable name that the user can use to identify its Flic's across apps.
 *                  For example "Kitchen Flic" or "Bedroom Lights".
 *
 */
@property(nonatomic, readwrite, strong, nullable) NSString *nickname;

/*!
 *  @property bluetoothAddress
 *
 *  @discussion     The bluetooth address of the Flic. This will be a string representation of a 49 bit long address. Example: "00:80:e4:da:12:34:56"
 *
 */
@property(nonatomic, readonly, strong, nonnull) NSString *bluetoothAddress;

/*!
 *  @property uuid
 *
 *  @discussion     This is a unique identifier string that best used to identify a Flic. This is for example used to identify Flics on all our API endpoints.
 *
 */
@property(nonatomic, readonly, strong, nonnull) NSString *uuid;

/*!
 *  @property serialNumber
 *
 *  @discussion     The serial number is a production identifier that is printed on the backside of the Flic inside the battery hatch.
 *                  This serves no other purpose than allowing a user to identify a button by manually looking at it. Can be useful in some cases.
 *
 */
@property(nonatomic, readonly, strong, nonnull) NSString *serialNumber;

/*!
 *  @property triggerMode
 *
 *  @discussion     Use this property to let the flic2lib know what type of click events you are interested it. By default you will get Click, Double Click and Hold events.
 *                  However, if you for example are only interested in Click events then you can set this property to FLICButtonTriggerModeClick. Doing so will allow the flic2lib to
 *                  deliver the events quicker since it can now ignore Double Click and Hold.
 *
 */
@property(nonatomic, readwrite) FLICButtonTriggerMode triggerMode;

/*!
 *  @property state
 *
 *  @discussion     Lets you know if the Flic is Connected, Disconnected, Connecting, or Disconnecting.
 *
 */
@property(nonatomic, readonly) FLICButtonState state;

/*!
 *  @property pressCount
 *
 *  @discussion     The number of times the Flic has been clicked since last time it booted.
 *
 */
@property(nonatomic, readonly) uint32_t pressCount;

/*!
 *  @property firmwareRevision
 *
 *  @discussion     The revision of the firmware currently running on the Flic.
 *
 */
@property(nonatomic, readonly) uint32_t firmwareRevision;

/*!
 *  @property isReady
 *
 *  @discussion     When a Flic connects it will go through a quick cryptographic verification to ensure that it is both a genuine Flic and that it is the correct Flic.
 *                  Once this is completed this property will be set to YES and it is not until after that that you will start receiving click events (if any). As soon as the button disconnects
 *                  this will be set to NO again.
 *
 */
@property(nonatomic, readonly) BOOL isReady;

/*!
 *  @property batteryLevel
 *
 *  @discussion     This will be the last know battery sample taken on the Flic. To convert this sample into voltage you can use the following formula:
 *                  float voltage = ((batterylevel * 3.6) / 1024);
 *                  It is a good idea to consider changing the battery once the voltage goes below 2.65V.
 *
 */
@property(nonatomic, readonly) uint32_t batterylevel;

/*!
 *  @property isUnpaired
 *
 *  @discussion     If this property is YES, then it means that this app's pairing with this specific Flic is no longer valid. This can for example occur if the Flic has been factory reset,
 *                  or if the maximum number of pairings have been reached. In this case you will need to delete the button from the manager and then scan for it again.
 *
 */
@property(nonatomic, readonly) BOOL isUnpaired;

/*!
 *  @method connect
 *
 *  @discussion     Attempts to connect the Flic. If the Flic is not available, due to either being out of range or not advertising, then it will be connected once it becomes
 *                  available as this call does not time out. This is often called a pending connection. It can be canceled by calling disconnect.
 *
 */
- (void)connect;

/*!
 *  @method disconnect
 *
 *  @discussion     Disconnect a currently connected Flic or cancel a pending connection.
 *
 */
- (void)disconnect;

@end

/*!
 *  @protocol FLICButtonDelegate
 *
 *  @discussion     The delegate of a FLICButton instance must adopt the FLICButtonDelegate protocol. All calls to the delegate methods will be on the main dispatch queue.
 *
 */
@protocol FLICButtonDelegate <NSObject>

/*!
 *  @method buttonDidConnect:
 *
 *  @param button   The FLICButton instance that the event originated from.
 *
 *  @discussion     This method is called every time the Flic establishes a new bluetooth connection. Keep in mind that you also have to wait for the buttonIsReady: before
 *                  the Flic is ready to be used.
 *
 */
- (void)buttonDidConnect:(FLICButton *)button;

/*!
 *  @method buttonIsReady:
 *
 *  @param button   The FLICButton instance that the event originated from.
 *
 *  @discussion     This method is called after each connection once the Flic has been cryptographically verified. You will not receive any click events before this is called.
 *
 */
- (void)buttonIsReady:(FLICButton *)button;

/*!
 *  @method button:didDisconnectWithError:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param error         This error lets you know the reason for the disconnect. An error does not necessarily mean that something went wrong.
 *
 *  @discussion     This method is called every time the bluetooth link with the Flic is lost. This can occur for several different reasons. The most common would be that
 *                  the iOS device and the Flic is no longer within range of each other.
 *
 */
- (void)button:(FLICButton *)button didDisconnectWithError:(NSError * _Nullable)error;

/*!
 *  @method button:didFailToConnectWithError:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param error         This error lets you know why the connection attempt failed.
 *
 *  @discussion     This method is called when a connection attempt to a button fails. This indicates that something has gone wrong and that the pending connection will not be reset.
 *
 */
- (void)button:(FLICButton *)button didFailToConnectWithError:(NSError * _Nullable)error;

@optional

/*!
 *  @method button:didReceiveButtonDown:age:
 *
 *  @param button        The FLICButton instance that the event originated from.
 *  @param queued        Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
 *  @param age               If the event was queued, then this will let you know the age of the event rounded to the nearest second.
 *
 *  @discussion     The Flic registered a button down event.
 *
 */
- (void)button:(FLICButton *)button didReceiveButtonDown:(BOOL)queued age:(NSInteger)age;

/*!
 *  @method button:didReceiveButtonUp:age:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param queued       Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
 *  @param age              If the event was queued, then this will let you know the age of the event rounded to the nearest second.
 *
 *  @discussion     The Flic registered a button up event.
 *
 */
- (void)button:(FLICButton *)button didReceiveButtonUp:(BOOL)queued age:(NSInteger)age;

/*!
 *  @method button:didReceiveButtonClick:age:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param queued       Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
 *  @param age              If the event was queued, then this will let you know the age of the event rounded to the nearest second.
 *
 *  @discussion     The Flic registered a button click event.
 *
 */
- (void)button:(FLICButton *)button didReceiveButtonClick:(BOOL)queued age:(NSInteger)age;

/*!
 *  @method button:didReceiveButtonDoubleClick:age:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param queued       Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
 *  @param age              If the event was queued, then this will let you know the age of the event rounded to the nearest second.
 *
 *  @discussion     The Flic registered a double click event.
 *
 */
- (void)button:(FLICButton *)button didReceiveButtonDoubleClick:(BOOL)queued age:(NSInteger)age;

/*!
 *  @method button:didReceiveButtonHold:age:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param queued       Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
 *  @param age              If the event was queued, then this will let you know the age of the event rounded to the nearest second.
 *
 *  @discussion     The Flic registered a button hold event.
 *
 */
- (void)button:(FLICButton *)button didReceiveButtonHold:(BOOL)queued age:(NSInteger)age;

/*!
 *  @method button:didUnpairWithError:
 *
 *  @param button       The FLICButton instance that the event originated from.
 *  @param error         This will always be nil at this time.
 *
 *  @discussion     The app no longer has a valid pairing with the Flic button. The isUnpaired property will now be YES and all connection
 *                  attempts made will immediately fail. To fix this you need to delete the button from the manager and then re-scan it again.
 *
 */
- (void)button:(FLICButton *)button didUnpairWithError:(NSError * _Nullable)error;

@end

NS_ASSUME_NONNULL_END
