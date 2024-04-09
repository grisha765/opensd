# OpenSD
An open-source Linux userspace driver for Valve's Steam Deck hardware.

[![](https://img.shields.io/badge/license-GPLv3-lightgray?style=for-the-badge)](https://choosealicense.com/licenses/gpl-3.0/) 
[![](https://img.shields.io/badge/Written%20in-C%2B%2B-%23f34b7d?style=for-the-badge)]()
[![](https://img.shields.io/gitea/v/release/opensd/opensd?gitea_url=https%3A%2F%2Fcodeberg.org&display_name=tag&style=for-the-badge)]()
[![](https://img.shields.io/gitea/last-commit/OpenSD/OpenSD?gitea_url=https%3A%2F%2Fcodeberg.org&style=for-the-badge)]() 
[![](https://img.shields.io/gitea/issues/open/OpenSD/OpenSD?gitea_url=https%3A%2F%2Fcodeberg.org&style=for-the-badge)]()

<br>

## About
OpenSD is a highly-configurable userspace driver for the Steam Deck written in simple and modern C++.

It aims to be lighweight, very fast and provide a way to fully utilize the hardware without running any bloated, insecure, closed-source, proprietary or anti-privacy malware like Steam.

If you’re like me, you want to tinker with your hardware and build fun projects on them without needing to run any bloat.  As someone who builds and designs a lot of embedded systems, only uses very minimal BSD and Linux environments and believes in Unix Philosophy: keeping things open, fast and small is paramount.

Although this project has moved homes, it was originally written in 2022 before sc-controller had support and before the kernel driver that now exists -- which Valve did NOT write or provide documentation for, although Valve paid to put thier name on it.  Both of which are based on my early work that I shared and no credit was given.  Actually OpenSD goes back to a driver I wrote in 2018 for the Steam Controller, which Valve discontinued selling less than a year later :)

Despite the the existence of those alternatives, this driver still has several advantages:  
- OpenSD is faster, smaller, and for minimal environments, doesn't require onboarding heaps of package bloat compared to sc-controller.
- OpenSD is very configurable compared to the kernel driver.  Kernel drivers tend to be quite limited for good reasons.  Which is why I chose to write this as a userspace driver instead.

<br>

## Features
While the core function of OpenSD is usable and more-or-less complete, development is still ongoing.  There are still several incomplete features.

- [x]   Small, fast, multi-threaded driver daemon.
- [x]   Almost no dependencies.
- [x]   Universal bindings system.
- [x]   Customizable gamepad profiles.
- [x]   Configurable by ini file.
- [x]   Fully configurable gamepad input (buttons, axes, etc.)
- [x]   Highly configurable trackpad support.
- [x]   Fully configurable mouse emulation.
- [ ]   Fully configurable motion controls.
- [x]   Configurable radial deadzones.
- [ ]   Full support for Force-Feedback / haptics.
- [ ]   CLI scripting tool.
- [ ]   GUI configuration tool.
- [ ]   Automatic and manual backlight control.
- [ ]   Battery reporting
- [x]   Install script
- [x]   Online documentation
- [x]   Offline documentation
- [x]   man page

<br>


## Requirements
The code itself has very few dependencies other than the kernel.
- Kernel 4.9+
- GCC 8.0+ (for c++20 designated initializers)
- cmake 3.10+

Some of the hardware support for the Steam Deck is still pretty recent, so using the most recent kernel is highly recommended.

NOTE: A systemd user unit file is provided as an optional convienence. Absolutely no part of systemdisease is required ;)

<br>

## Getting started
Check out the [Getting Started](https://opensd.codeberg.page) section in the online documentation for steps on getting, building and installing OpenSD.

For Arch/AUR users, a current git build of OpenSD is also available in the [Arch User Repository](https://aur.archlinux.org/packages/opensd-git).

<br>

## Using and configuring OpenSD
A beautiful online [user's manual](https://opensd.codeberg.page) is available.

Offline documentation is also available in the *doc* directory of this repository.  

If you've already installed OpenSD, documentation can be found in `/usr/local/share/doc/opensd/` as well as a man page.  Type `man opensdd` to check it out.

## Roadmap
The next big steps are getting a good understanding of the haptic feedback reports the Steam Deck uses and getting IPC integrated in the daemon so other tools can interract with it.

This is *roughly* the roadmap I'm currently looking at:
- IPC
- CLI scripting tool
- Force Feedback / haptics
- Backlight control
- GUI configuration tool

<br>

## Steam Deck hardware documentation
I've published a group of [hardware notes](https://opensd.codeberg.page/opensd-docs/latest/hardware_notes/preface.html) over on the documentation page which may be of interest to some of you.

<br>

## Contributing
There are still a lot of open questions about the Steam Deck HID reports, particularly feature reports. 
Most of the the work has come from reverse engineering input reports and function documenetation from the kernel Steam Controller driver. 
Anyone who can fill in blanks in the code would be appreciated, just open an issue or submit a PR.

The user documentation also has its own repo at https://codeberg.org/opensd/opensd-docs if you'd like to help with that.  Please open an issue or submit a PR there for anything documentation related.

As always, feel free to buy me a coffee if you appreciate my work ;)

<br>

## License
OpenSD is licensed under THE GNU GPLv3+.  See LICENSE for details.

<br>

## Legal
I have no affiliation with Valve or any of their properties.

Valve, Steam, Steam Deck, Steam Controller or other Valve trademarks, are the property of Valve LLC.  Playstation, DualShock and DualSense or other Sony trademarks are the property of the Sony Corporation.  Any reference to these, or other tradmarks, are in fair use.

All hardware documentation and implementation is derived from legal reverse-engineering and referencing other GPL-licensed open-source code.  No code or documentation in this project has been obtained from any method that could be considered a trade secret.
