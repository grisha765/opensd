# OpenSD changelog

## [v0.53]  2024/04/11
### Added
 - Daemon command line option to override profile when starting up
 - Daemon command line option to list all available profiles
 - Added and improved various debug messages

### Changed
 - Removed .profile "extension" from default profile filenames.  File extensions are not checked anyway, so less typing needed.
 - Simplified and improved ProgArgs class
 - Moved a few functions around
 - Moved lizard mode suppression to main driver thread to reduce complexity

### Fixed
 - Fixed bug where terminating gamepad driver before starting thread resulted in abnormal shutdown
 - Fixed some minor bugs and typos


## [v0.52]  2024/04/08
### Changed
 - Improved logging when parsing DeviceInfo of profiles
 - Updated user documentation for [Touchpads] profile section
 - Cleaned up [DeviceInfo] examples in default profile

### Fixed
 - Fixed function redefinition from my last sloppy merge on gitlab
 - Fixed error where virtual device PID was misreported

### Removed
 - Removed unused port setting in daemon.ini


## [v0.51]  2024/03/19
### Changed
  - Changed project owner / maintainer
  - Moved project from gitlab to codeberg due to cloudflare sucking
  - Updated license notice
  - Updated documentation
  - Merged opensdd.1 and opensd-files.5 man pages
  - Cleaned up man page a bit
  - Cleaned up scripts a little
  - Updated readme

### Removed
  - Removed unused opensd-files.5 manpage

### Notes
  - There were a few undesired artefacts with a full git migration, so we did an ugly one.
  - Dates and git histories have been reset.
  - There's a been a kernel driver for a while now (that Valve did NOT write or provide documentation for, but PAID to take credit for), so I doubt anyone actually cares.
  - Still more to clean up, but this should be good enough to push for now.


## [v0.50]  2023/09/11
### Added
  - Added touchpad sensitivity for relative axes.
  - Added individual scale for all relative axes.
  - Added scale parameter for all REL_ bindings in profiles.
  - Added new [Touchpads] section to profiles.
  - Added new rel axis sensitivity keys to [Touchpads] section in profiles.
  - Added new rel axis intertia keys to [Touchpads] section in profiles.
  
### Changed
  - Updated and improved documentation in default ini files.
  - Improved message logging when reading profiles.


## [v0.49]  2023/07/17
### Fixed
  - Fixed right pad incorrectly being mapped onto left pad (thanks to Ilya Kataev @TeddyReady)


## [v0.48]  2022/12/18
### Changed
  - Split man page into two parts: opensdd(1) and opensd-files(5).
  - man pages are now compressed with gzip.
  - Separated install scripts into post-install section of cmake build.
  - Modified cmake build to be able to optionally disable post-install for PKGBUILD.
  - Cleaned up post-install scripts a little.

### Fixed
  - man pages should go into the correct directory now.


## [v0.47]  2022/12/16
### Added
  - Added [DeviceInfo] section to profile files for compatibility.  See documentation.
  - Added ability to set absolute axis fuzziness and resolution.  See documentation.
  - Added support for quoted string literals in ini files (profiles are ini files, too).
  - Added new compatibility profile for Sony(R) DualSense(R) based on the hid_playstation driver.
  - Added new compatibility profile for Sony(R) DualShock 4(R) based on hid_sony driver.
  - Added a few more debug messages.
  - Updated documentation.
  - Improved the inline help in the provided default profiles.
  
### Fixed
  - Ini files can now use both '#' and ';' characters for comment lines.
  - Fixed absolute axis resolution not being initialized.


## [v0.46]  2022/12/15
### Added
  - Improved input report handling.
  - Added uinput FF event handling for WiP FF support.

 
## [v0.45]  2022/12/07
### Fixed
  - Fixed issue building on older kernels where input_event_names.hpp references codes that are not yet defined.
  - Fixed a build issue where, in some environments, pthread was not getting linked.

### Changed
  - Updated kernel requirement to 4.9 to reflect new input event code checks.


## [v0.44]  2022/12/06
### Added 
  - Created new online documentation at https://open-sd.gitlab.io/opensd-docs
  
### Changed
  - Moved all documentation source to separate repo (https://gitlab.com/open-sd/opensd-docs)
  - Changed the way offline documentation is built.
  - Changed the way manpages are built.
  - Updated offline documentation.
  - Updated the manpage.
  - Updated README with new documentation links.

### Removed
  - Removed asciidoc source from the main repo.
  - Removed hardware notes (doc/hw.html) from this project.

## [v0.43]  2022/11/19
### Added
  - Added asciidoc user manual.
  - Added html user manual (from asciidoc).
  - Added asciidoc manpage.
  - Added manpage for opensdd (from asciidoc).
  - Added documentation and manpage installation to CMakeLists.txt
  - Documentation is now installed to ${CMAKE_INSTALL_PREFIX}/share/doc/opensd/


## [v0.42]  2022/11/01
### Added
  - Added a CHANGELOG to the project.

### Changed
  - Changed file extension for profile files from `.ini` to `.profile` in order to be less confusing.
  - Missing default / standard profiles are copied to user profile directory on startup (old and modified files are not overwritten)
  - Cleaned up profile loading code a bit.


## [v0.41]  2022/10/31
### Added
  - Added new "Profile" binding.  This allows the user to create a binding that loads a new gamepad profile.
  - Added driver messaging system.
  - Added driver locking.
  - Added a 250 microsecond sleep to main driver thread.

### Changed
  - Updated documentation in default profile.
  - Gamepad driver code cleanup.


## [v0.40]  2022/10/27
### Added
  - Added touchpad button emulation.  This allows binding "press" actions for specific regions of each touchpad.
    - Added quadrant touchpad "buttons" (triangular, no overlap).
    - Added orthogonal touchpad "buttons" (dpad-like, overlapping).
    - Added 2x2 grid touchpad "buttons" (no overlap).
    - Added 3x3 grid touchpad "buttons" (no overlap).

### Changed
  - Updated default profile file with documentation on new bindings.


## [v0.38]  2022/10/26
### Added
  - Added new "Command" binding.  This allows the user to create a binding that executes a command or script.
  - Added new Ini::ValVec::FullString() index parameter.
  - Added multi-threaded process runner.

### Changed
  - Updated documentation in default profile.
  - Improved logging when loading bindings.

### Removed
  - Removed old built-in profile preset.


## [0.37]  2022/10/21
### Fixed
  - Fixed a new bug where the daemon was opening the wrong hidraw device.
  - Fixed a bug where the driver thread would not exit until an input report was received.

### Changed
  - The scanning method for hidraw devices has been re-written.
  - Added a timeout to input device polling.


## Older versions are not documented here.
