# OpenSD profile file: dualshock4
#
# !! Please see the online OpenSD users manual at <https://opensd.codeberg.page> for detailed
# !! explanations of each setting in this file. Or use the offline HTML documentation that came
# !! with this distribution.  If installed, it can usually be found in /usr/local/share/opensd
# !! An man page is also available by typing: man opensdd
#
# This is file is included with OpenSD and meant as a template.  It's recommended to make a 
# copy of this file, rather than edit it directly.


[Profile]
# Profile name
#   The profile name as it will appear in the GUI and through the CLI query. 
#   Should be unique for each profile to avoid confusion.
#
#   Format:
#       Name = <string>
#
#       string: 
#           Any unique name.  Should be enclosed in quotes to preserve spaces.
#
#   Example:
#       Name = "Retroarch Profile"
#
Name            = "Sony DualShock 4 compatibility profile"

# Profile description
#   The profile description as it will appear in the GUI and through the CLI query.
#
#   Format:
#       Description = <string>
#
#       string:
#           A brief profile description.  Should be enclosed in quotes to preserve spaces.
#
#   Example:
#       Description = "Just a basic configuration for Retroarch"
#
Description     = "This profile mimics the appearance and behaviour of the hid_sony driver."


[Features]
# Force Feedback
#   Enable haptic feedback / rumble function.
#   ** Not yet fully implemented **
#
#   Format:
#       ForceFeedback = <true | false>
#
#   Example:
#       ForceFeedback = true
#
ForceFeedback   = true

# Motion Device
#   If this is set to true, an additional input device will be created which will report motion
#   control data.  Motion axes still need to have thier bindings defined.  If this is disabled, any
#   "Motion" device bindings will be ignored.
#
#   Format:
#       MotionDevice = <true | false>
#
#   Example:
#       MotionDevice = true
#
MotionDevice    = true

# Mouse Device
#   If this is set to true, an additional input device will be created which will be used to send 
#   mouse / trackpad events.  Mouse events still need to have thier bindings defined.  If this is
#   disabled, any "Mouse" device bindings will be ignored.
#
#   Format:
#       MouseDevice = <true | false>
#
#   Example:
#       MouseDevice = true
#
MouseDevice     = true

# Lizard Mode
#   The Steam Controller and the Steam Deck both have a kind of fallback BIOS mode which emulates 
#   some keyboard and mouse events.  Valve refers to this as 'Lizard Mode'.  This mode cannot be 
#   redefined.  It sends events IN ADDITION to the gamepad events created by the OpenSD driver, so 
#   it should always be disabled. 
#
#   When OpenSD exits, Lizard Mode is re-enabled.
#
#   Format:
#       LizardMode = <true | false>
#
#   Example:
#       LizardMode = false
#
LizardMode      = false

# Stick Filtering
#   The thumbsticks on the Steam Deck have a circular range but return square-ish data, which makes 
#   it feel odd and complicated to apply radial deadzones to.  Because of this, OpenSD vectorizes the
#   stick position and returns "cleaner", round stick ranges, as well as being able to create clean 
#   deadzone rescaling.
#
#   If you disable this setting, axis ranges are still internally normalized and rescaled to the the 
#   uinput device, but no vectorization will be applied and any deadzones will be ignored.
#
#   Format:
#       StickFiltering = <true | false>
#
#   Example:
#       StickFiltering = true
#
StickFiltering  = true

# Trackpad Filtering
#   Similar to above, but matches the square shape of the trackpad.  Filtering is only applied to
#   absolute values.  This setting must be enabled to apply deadzones to the trackpad absolute axes.
#   Relative values (rel_x and rel_y) are unaffected, therefore deadzones do not affect mouse 
#   movement with the pads.
#
#   Format:
#       TrackpadFiltering = <true | false>
#
#   Example:
#       TrackpadFiltering = true
#
TrackpadFiltering  = true


[DeviceInfo]
# Device Information
#   This section allows you to set the name and USB identity of the individual input devices created
#   by the gamepad driver.  This can be useful to mimic the appearance of a specific controller in
#   order to get a very poorly written game to recognise and support it.
#
#   These are optional and will use defaults if undefined.  If the respective device is not first
#   enabled in the Feature section, these values will be ignored.
#
#   Format:
#       <device> = <vid> <pid> <ver> <name_string>
#
#       device:
#           Which device to define.  Can be "Gamepad" "Motion" or "Mouse"
#
#       vid:
#           Vendor ID.  16-bit hex value starting with "0x"
#
#       pid:
#           Product ID.  16-bit hex value starting with "0x"
#
#       vid:
#           Version.  16-bit hex value starting with "0x"
#
#       name_string:
#           The name string of the device
# 
#   Examples:
#       Gamepad = 0xDEAD 0xBEEF 0x001 "OpenSD Gamepad Device"
#       Motion = 0xDEAD 0xBEEF 0x001 "OpenSD Motion Control Device"
#       Mouse = 0xDEAD 0xF00D 0x001 "OpenSD Mouse Device"
#
Gamepad     = 0x054C 0x05C4 0x8111 "Sony Computer Entertainment Wireless Controller"
Motion      = 0x054C 0x05C4 0x8111 "Sony Computer Entertainment Wireless Controller Motion Sensors"
Mouse       = 0x054C 0x05C4 0x8111 "Sony Computer Entertainment Wireless Controller Touchpad"


[Deadzones]
# Axis Deadzones
#   Values are floating point and represent the percentage of the total range to ignore.  A value
#   of 0.05 would be a 5% deadzone.  Deadzones are capped at 90% (0.9).  A value of 0 is considered
#   disabled.  
#   If StickFiltering is disabled, LStick and RStick deadzones will be ignored.
#   If TrackpadFiltering is disabled, LPad and RPad deadzones will be ignored.
#
#   Format:
#       <input> = <value>
#
#       input:
#           Valid absolute inputs, which are:  
#               "LeftStick", "RightStick", "LeftPad", "RightPad", "LeftTrigg", and "RightTrigg"
#
#       value:
#           Any floating point value, from 0.0 to 0.9, representing percentage of axis center to
#           ignore.
#
#   Example:
#       LStick = 0.20
#
LStick      = 0.17
RStick      = 0.17
LPad        = 0
RPad        = 0
LTrigg      = 0.17
RTrigg      = 0.17


[GamepadAxes]
# Gamepad Axis List
#   Gamepad absolute axes must have a defined range or they will not be created. 
#   Any 'Gamepad' ABS_ events which are configured in the [Bindings] section must be defined here
#   first, or they will be ignored.
#
#   Using specific min/max values may be useful for exactly emulating certain devices. Setting these
#   to higher values than the underlying hardware will NOT make your joysticks more precise.
#
#   Format:
#       <abs_axis> = <axis_min> <axis_max>
#
#       abs_axis:
#           Any axis event code beginning with "ABS_"
#
#       axis_min:
#           The lowest value for the full axis range.
#
#       axis_max:
#           The highest value for the full axis range.
#
#       fuzz:
#           Fuzziness of axis values (OPTIONAL)
#
#       res:    
#           Resolution in units/mm or units/radian (OPTIONAL)
#
#   Examples:
#       ABS_HAT0X = -1 1
#       ABS_HAT0Y = -1 1
#       ABS_Z = 0 32767
#
#-----------------------------------------------------------
# Axis            Min       Max         Fuzz    Resolution
#-----------------------------------------------------------
ABS_HAT0X       = -1        1
ABS_HAT0Y       = -1        1
ABS_X           = -255      255
ABS_Y           = -255      255
ABS_RX          = -255      255
ABS_RY          = -255      255
ABS_Z           = 0         255
ABS_RZ          = 0         255


[MotionAxes]
# Motion Axis List
#   Motion control absolute axes must have a defined range or they will not be created. 
#   Any 'Motion' ABS_ events which are configured in the [Bindings] section must be defined here 
#   first, or they will be ignored.
#
#   Using specific min/max values may be useful for exactly emulating certain devices.
#   Setting these to higher values than the underlying hardware will NOT make your motion sensors 
#   any more precise.
#
#   Format:
#       <abs_axis> = <axis_min> <axis_max> <fuzz> <res>
#
#       abs_axis:
#           Any axis event code beginning with "ABS_"
#
#       axis_min:
#           The lowest value for the full axis range.
#
#       axis_max:
#           The highest value for the full axis range.
#
#       fuzz:
#           Fuzziness of axis values (OPTIONAL)
#
#       res:    
#           Resolution in units/mm or units/radian (OPTIONAL)
#
#   Examples:
#       ABS_X = -32767 32767
#       ABS_Y = -32767 32767
#       ABS_Z = -32767 32767
#       ABS_RZ = -2097152 2097152 16 1024
#
#-----------------------------------------------------------
# Axis            Min       Max         Fuzz    Resolution
#-----------------------------------------------------------
ABS_X           = -32768    32768       16      8192
ABS_Y           = -32768    32768       16      8192
ABS_Z           = -32768    32768       16      8192
ABS_RX          = -2097152  2097152     16      1024
ABS_RY          = -2097152  2097152     16      1024
ABS_RZ          = -2097152  2097152     16      1024


[Touchpads]
# Touchpad Senstivity
#   When the touchpad is used for relative axes, the sensitivity of the X and Y axes can be adjusted
#   for each pad.  This axis movement is multiplied by the specified value, so values less than 1.0 
#   will result in a slower rate of movement.  
#   A value of 1.0 represents 100% of default sensitvity.  A value of 0.5 would be 50% slower than
#   the default.
#
#   While it is possible to set the scale of individual relative axes for each binding, that method
#   has low precision due to the nature of integer math.  Adjusting the pad sensitivity here will 
#   probably give you much finer-grain control.  The two methods can also be combined, with 
#   the REL_ axis scale being calulated after the pad sensitivity.
#
#   Format:
#       {L|R}PadSensitivity = <x_sens> <y_sens>
#
#       x_sens:   
#           A floating point value representing the sensitivity of the x axis.
#
#       y_sens:   
#           A floating point value representing the sensitivity of the y axis.
#
#   Examples:
#       LPadSensitivity = 1.0 1.0
#       RPadSensitivity = 0.75 0.75
#
#---------------------------------------
# Axis                x         y
#---------------------------------------
LPadSensitivity     = 1.0       1.0
RPadSensitivity     = 1.0       1.0


# Touchpad Interia Decay
#   When used as relative axes, the touchpad motion vectors have intertia.  This intertia decays
#   each USB frame until it becomes zero.  These settings allow you to configure the rate of 
#   inertial decay.  The gamepad USB device polls at fixed(?) 250Hz so it can fall off rather 
#   quickly even with very small values.
#
#   A value of 0 means no decay (frictionless).  A value of 1.0 will effectively disable inertia.  
#   When using a trackpad as a pointer device, a value of 0.05 (5% decay per frame) is recommended.
#   Values are clamped from 0 to 1.0.
#
#   Format:
#       {L|R}PadDecay = <x_decay> <y_decay>
#
#       x_decay:
#           A floating point value between 0 and 1.0 representing the inertial decay of the X axis.
#
#       y_decay:
#           A floating point value between 0 and 1.0 representing the inertial decay of the Y axis.
#
#   Examples:
#       LPadDecay = 1.00 1.00
#       RPadDecay = 0.05 0.05
#
#---------------------------------------
# Axis            x         y
#---------------------------------------
LPadDecay       = 0.05      0.05
RPadDecay       = 0.05      0.05


[Bindings]
# Gamepad Input Bindings
#   This should be a list of all the physical gamepad buttons/sticks/pads/motion inputs you want to
#   bind to a virtual input event or command.  Anything not specified here will be considered 
#   'unbound' and not register any event.
#
#   General binding format:   
#       <input> = <bindtype> [args]
#
#       input:
#           Input names reflect the action of the user.  Like pressing a button or pushing a 
#           thumbstick to the left.  Button bindings are pretty self-explanatory.  While axes are
#           broken into directional actions (i.e. LeftStickUp). This way you can make an axis send a
#           keypress if you wanted to, and the inverse is also possible.
#
#       bindtype:
#           There are three categories of bindings: "Event", "Command" and "Profile" bindings.
#
#       args:
#           Arguments specific to each type of binding / event.
#
#
# Event Bindings:
#   Since OpenSD manages up to 3 separate virtual input devices, you will need to specify which
#   device will send a particular event.  These devices are "Gamepad", "Motion and "Mouse".
#
#   Gamepad device:
#       This  device is for buttons and axes input.  This device is always enabled.
#
#   Motion device: 
#       This is a separate input device specifically for motion-sensors like accelerometers and 
#       gyroscopes.  This device must be enabled in the [Features] section.
#
#   Mouse device: 
#       This is a virtual mouse device which uses relative axes (REL) and buttons / keys. 
#       This device must be enabled in the [Features] section.
#
#   Format:
#       <input> = <device> <event_code> [args]
#
#       input:
#           Any valid input binding name.
#
#       device:
#           One of the virtual devices created by OpenSD which will emit the event.
#           Can be either "Gamepad", "Motion" or "Mouse"
#
#   Each EVENT binding must be mapped to a Linux input event code.  A complete list can be found in
#   <linux/input-event-codes.h> and the supported codes can be found in the OpenSD documentation.
#   Most KEY_*, BTN_*, ABS_* and REL_* codes should be usable.
#
#   Event type is derived from the code prefix.  For example: KEY_* and BTN_* are key events, 
#   ABS_* is an absolute axis event, and REL_* is a relative axis event.
#
#   There are also some standard meanings for event codes with regard to device types and it is
#   possible to configure this section which can cause very strange behaviour.  It's also possible 
#   to bind event types to these devices that no software will probably ever recognise, so try to 
#   keep it sane ;)
#
#   !! Please see the documentation for a more detailed explanation about event bindings !!
#
#   --------------------------------------
#   Binding keys / buttons:
#       Keys and buttons share the same underlying event type.
#
#       Format:
#           <input> = <device> <key_event_code>
#
#           input:
#               Any valid input binding name.
#
#           device:
#               Any device which supports button / key events.
#               Gamepad should be used for gampad/joystick buttons.
#               Mouse should be used for KEY_* and mouse buttons.
#
#           key_event_code:
#               Any input event code beginning with "KEY_" or "BTN_"
#
#       Examples:
#           Menu = Gamepad BTN_START
#           DPadRight = Mouse KEY_RIGHT
#           RPadPress = Mouse BTN_LEFT
#
#   --------------------------------------
#   Binding absolute axes:
#       Absolute axis event bindings MUST specify the direction of the axis.  This allows buttons to
#       send axis events or allows an axis to be inverted at a driver level.
#
#       Format:
#           <input> = <device> <abs_event_code> <direction>
#
#           input:
#               Any valid input binding name.
#
#           device:
#               A device which supports absolute axes, so either "Gamepad" or "Motion"
#
#           abs_event_code:
#               Any event code beginning with "ABS_"
#
#           direction:  
#               Either "+" indicating up/left, or "-" indicating "down/right"
#
#       Examples:  
#           DPadUp = Gamepad ABS_HAT0Y -
#           DPadDown = Gamepad ABS_HAT0Y +
#           LStickLeft = Gamepad ABS_X -
#           LStickRight = Gamepad ABS_X +
#           
#   --------------------------------------
#   Binding relative axes:
#       Relative axis events are motion vectors representing relative changes in speed/direction.
#       These are probably most useful with the Mouse device, since most software will never use
#       relative axes from a gamepad/joystick even if they're present.
#
#       Format:
#           <input> = <device> <rel_event_code> [scale]
#
#           input:
#               Any valid input binding name.
#
#           device:
#               A device which supports absolute axes, generally just "Mouse"
#
#           rel_event_code:
#               Any event code beginning with "REL_"
#
#           scale:
#               An optional value which the relative axis will be multiplied by.
#               If omitted the default value of 1.0 (100%) will be used.
#
#       Examples:
#           RPadRelX = Mouse REL_X
#           RPadRelY = Mouse REL_Y
#           LPadRelY = Mouse REL_WHEEL 0.5
#
# --------------------------------------
# Command Bindings:
#       The "Command" binding allows you to execute external programs or scripts by forking them off
#       as a child process.
#
#       Format:  
#           <input> = Command <wait_for_exit> <repeat_delay_ms> <command_to_execute>
#
#           input:
#               Any valid input binding name.
#
#           wait_for_exit: 
#               <true | false> value which specifies if the command should complete before the
#               binding can be triggered again.
#
#           repeat_delay_ms: 
#               The amount of time in milliseconds that must elapse before the binding can be 
#               triggered again.  The timer starts when the binding is successfully triggered.
#
#       Example:  
#           QuickAccess = Command true 0 rofi -show run
#     
# --------------------------------------
# Profile Bindings:
#       This binding type allows you to switch to a different profile using just
#       the gamepad input.  Profiles are loaded from the user profile directory.
#
#       Profile cycling can easily be achieved by binding the same button in each of the profiles
#       so the last profile links back to the first.
#
#       Format:
#           <input> = Profile <profile_name>
#
#           input:
#               Any valid input binding name.
#
#           profile_name:
#               Filename of the profile ini you want to load.  Path is fixed to the user profile
#               directory, so only specify the filename itself.
#
#       Example:
#           L5 = Profile left_hand_mouse.profile
#       
#-----------------------------------------------------------
# Input             BindType    Mapping     +/-/scale
#-----------------------------------------------------------
# Directional Pad
DpadUp              = Gamepad   ABS_HAT0Y   -
DpadDown            = Gamepad   ABS_HAT0Y   +
DpadLeft            = Gamepad   ABS_HAT0X   -
DpadRight           = Gamepad   ABS_HAT0X   +
# Buttons
A                   = Gamepad   BTN_SOUTH
B                   = Gamepad   BTN_EAST
X                   = Gamepad   BTN_WEST     
Y                   = Gamepad   BTN_NORTH
L1                  = Gamepad   BTN_TL
R1                  = Gamepad   BTN_TR
L2                  = Gamepad   BTN_TL2
R2                  = Gamepad   BTN_TR2
L3                  = Gamepad   BTN_THUMBL
R3                  = Gamepad   BTN_THUMBR
L4                  = None
R4                  = None
L5                  = None
R5                  = None
Menu                = Gamepad   BTN_START
Options             = Gamepad   BTN_SELECT
Steam               = Gamepad   BTN_MODE
QuickAccess         = None
# Triggers
LTrigg              = Gamepad   ABS_Z       +
RTrigg              = Gamepad   ABS_RZ      +
# Left Stick
LStickUp            = Gamepad   ABS_Y       -
LStickDown          = Gamepad   ABS_Y       +
LStickLeft          = Gamepad   ABS_X       -
LStickRight         = Gamepad   ABS_X       +
LStickTouch         = None
LStickForce         = None
# Right Stick
RStickUp            = Gamepad   ABS_RY      -
RStickDown          = Gamepad   ABS_RY      +
RStickLeft          = Gamepad   ABS_RX      -
RStickRight         = Gamepad   ABS_RX      +
RStickTouch         = None
RStickForce         = None
# Left Trackpad
LPadUp              = None
LPadDown            = None
LPadLeft            = None
LPadRight           = None
LPadTouch           = None
LPadRelX            = Mouse     REL_X
LPadRelY            = Mouse     REL_Y
LPadTouch           = Mouse     BTN_TOUCH
LPadPress           = Mouse     BTN_LEFT
LPadForce           = None
LPadPressQuadUp     = None
LPadPressQuadDown   = None
LPadPressQuadLeft   = None
LPadPressQuadRight  = None
LPadPressOrthUp     = None
LPadPressOrthDown   = None
LPadPressOrthLeft   = None
LPadPressOrthRight  = None
LPadPressGrid2x2_1  = None
LPadPressGrid2x2_2  = None
LPadPressGrid2x2_3  = None
LPadPressGrid2x2_4  = None
LPadPressGrid3x3_1  = None
LPadPressGrid3x3_2  = None
LPadPressGrid3x3_3  = None
LPadPressGrid3x3_4  = None
LPadPressGrid3x3_5  = None
LPadPressGrid3x3_6  = None
LPadPressGrid3x3_7  = None
LPadPressGrid3x3_8  = None
LPadPressGrid3x3_9  = None
# Right Trackpad
RPadUp              = None
RPadDown            = None
RPadLeft            = None
RPadRight           = None
RPadTouch           = None
RPadRelX            = Mouse     REL_X
RPadRelY            = Mouse     REL_Y
RPadTouch           = Mouse     BTN_TOUCH
RPadPress           = Mouse     BTN_LEFT
RPadForce           = None
RPadPressQuadUp     = None
RPadPressQuadDown   = None
RPadPressQuadLeft   = None
RPadPressQuadRight  = None
RPadPressOrthUp     = None
RPadPressOrthDown   = None
RPadPressOrthLeft   = None
RPadPressOrthRight  = None
RPadPressGrid2x2_1  = None
RPadPressGrid2x2_2  = None
RPadPressGrid2x2_3  = None
RPadPressGrid2x2_4  = None
RPadPressGrid3x3_1  = None
RPadPressGrid3x3_2  = None
RPadPressGrid3x3_3  = None
RPadPressGrid3x3_4  = None
RPadPressGrid3x3_5  = None
RPadPressGrid3x3_6  = None
RPadPressGrid3x3_7  = None
RPadPressGrid3x3_8  = None
RPadPressGrid3x3_9  = None
# Accelerometers
AccelXPlus          = Motion    ABS_RX      +
AccelXMinus         = Motion    ABS_RX      -
AccelYPlus          = Motion    ABS_RY      +
AccelYMinus         = Motion    ABS_RY      -
AccelZPlus          = Motion    ABS_RZ      +
AccelZMinus         = Motion    ABS_RZ      -
# Gyro / Attitude
RollPlus            = Motion    ABS_X       +
RollMinus           = Motion    ABS_X       -
PitchPlus           = Motion    ABS_Y       +
PitchMinus          = Motion    ABS_Y       -
YawPlus             = Motion    ABS_Z       +
YawMinus            = Motion    ABS_Z       -

