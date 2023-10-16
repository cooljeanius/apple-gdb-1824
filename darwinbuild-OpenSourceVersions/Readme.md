~~To use with `darwinbuild`, from the top directory of this distribution, do a command like this:~~
```
sudo darwinbuild -init darwinbuild/gdb.plist -nodmg
```

Or do something like these instructions say to do:
http://reverse.put.as/2009/01/14/how-to-compile-gdb-and-other-apple-open-source-packages-in-mac-os-x/

Actually I messed up; the plist here is the one that installs as `/usr/local/OpenSourceVersions/gdb.plist`; it is ***not*** actually one to use with `darwinbuild`
