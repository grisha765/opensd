# OpenSD Documentation

[![](https://img.shields.io/gitea/last-commit/OpenSD/opensd-docs?gitea_url=https%3A%2F%2Fcodeberg.org&style=for-the-badge)]() [![](https://img.shields.io/gitea/issues/open/OpenSD/opensd-docs?gitea_url=https%3A%2F%2Fcodeberg.org&style=for-the-badge)]()



## About
This is the official source repository for [OpenSD](https://codeberg.org/opensd/opensd) documentation.

An online version of the HTML documentation can be found at https://opensd.codeberg.page

Pre-built offline versions of the HTML documentation and man pages are included with the OpenSD distribution.

<br>


## Building
You can also build the man pages and HTML documentation yourself, if you prefer.

Requirements:
 - Asciidoctor
 - gzip

There is a simple build script located in the root directory of this repo.  Just run that and the compiled documentation will be placed in the *build* subdirectory.

<br>

## License
This repo contains the documentation for OpenSD as well as modified Antora user interface files, both of which are separately licensed.

The documentation is licensed under *Creative Commons (BY-NC-SA 4.0)*.  See LICENSE for details.

The Antora UI files are licensed as required under *Mozilla Public License 2.0*.  See the LICENSE file in the *antora* directory for details.

<br>

## Contributing
If you spot something incorrect in any of the online documentation, or wish to help improve it, please open an issue here or send a PR.

All documentation is in asciidoc format.  Static HTML documentation is generated using a modified [Antora](https://antora.org/) theme.  Man pages are also generated from asciidoc.

<br>

## Disclaimer
I have no affiliation with Valve or any of their properties.

Valve, Steam, Steam Deck, Steam Controller or other Valve trademarks, are the property of Valve LLC.  Any reference to these, or other tradmarks, are in fair use.

All hardware documentation is derived from legal reverse-engineering, freely available and publically licensed code, and other openly published information.  No documentation in this project has been obtained from any method that could be considered a trade secret.
