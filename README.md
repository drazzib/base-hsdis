base-hsdis
==========

Basic disassembler plugin for HotSpot JVM

History
-------
Recent versions of HotSpot (including current builds of JDK7 and JDK6)
can load a plug-in disassembler for diagnosing code quality.

There are partial sources for of this plugin in the OpenJDK, but this is an
alternative, independently written implementation of the plugin,
demonstrating that the plugin can be easily built on top of any disassembler.
It uses the "Bastard" disassembler from SourceForge (aka libdisasm).

Why this repository ?
---------------------
This is just a "repackaging" of original hsdis based on source code from
http://kenai.com/projects/base-hsdis

I've modified build-system to don't depends on libdisasm *source* but use
existing libdisasm *system library* (so dynamic linking to it - no static link).

Build Debian package
--------------------
+ Download this version of hsdis:
```
git clone git://github.com/drazzib/base-hsdis.git
cd base-hsdis
```

+ Install build-dependencies
```
sudo apt-get install build-essential devscripts
sudo mk-build-deps -i -r
```

+ Build package
```
debuild -b -uc -us
```

+ Install package
```
sudo debi --with-depends
sudo apt-get --purge remove base-hsdis-build-deps
```

Enjoy your disassembler
-----------------------
```
JDK7=/usr/lib/jvm/java-7-openjdk-amd64
XJAVA="$JDK7/bin/java -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly"
$XJAVA -Xcomp -cp ~/Classes hello
$XJAVA -Xcomp -cp ~/Classes -XX:PrintAssemblyOptions=hsdis-print-bytes hello
$XJAVA -XX:-PrintAssembly -XX:+PrintStubCode
$XJAVA -XX:-PrintAssembly -XX:+PrintInterpreter
$XJAVA -XX:-PrintAssembly -XX:+PrintSignatureHandlers
```

License
-------
```
Copyright (C) 2012 Damien Raude-Morvan
Copyright (C) 2008 John R. Rose.

This package may be used for any purpose, provided that this copyright notice is preserved intact.
THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
```