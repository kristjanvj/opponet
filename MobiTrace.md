Utilities for generating and converting mobility and contact traces.

# Introduction #

The MobiTrace toolbox is a collection of scripts and simple utilities for trace driven mobility support. The following utilities are provided:

  * **UrbanMobility**
  * **[rwpy](rwpy.md)**
  * **[u2tr](u2tr.md)**
  * **[mobgen](mobgen.md)**

# Installation and usage #

All the scripts presently provided are created in Python (http://www.python.org) on a Linux platform. Python is however multi-platform so the programs should run on a Windows platform with minimal changes. Python 2.4 or higher is required.

Get the latest version from the downloads area or via anonymous subversion access:
```
svn checkout http://opponet.googlecode.com/svn/trunk/ opponet-read-only
```
Unpack the code in a directory of your choice. It is convenient to add the directory to your path but not mandatory. The scripts may have to be made executable on a Linux platform, e.g.
```
  chmod +x rwpy
```
Thereafter, the scripts can be run using
```
  python rwpy test.xml
```
or simply
```
  ./rwpy test.xml
```