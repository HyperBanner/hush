# Welcome to hush

## Hyper's Useless SHell.

This is my first personal project, and I gotta say, I needed to google quite a lot of stuff in order to even get started.
Definitely the most challenging thing I've done yet, and I'm super happy to get it out of the way this fast.
I haven't seen people try and build a shell as their first project, and now I get why.
I may have overestimated just how complex just adding in seemingly basic stuff is, and just establishing a good structure for your code,
so it doesn't become a nightmare to manage and write.

## Usage

If for some god forsaken reason you want to test this out, this repo already includes a build directory, where the executable file itself
resides. Just run the executable in a terminal emulator and you're good to go.

Support-wise, I'm pretty sure this only works on Linux operating systems. Going cross-platform is a bit too much for me at this point.

## Features

Has the absolute basics baked in, like "cd", "pwd", "echo", etc.
Supports running PATH executables.
Both absolute and relative paths are supported, as well as the "~" alias for /home/USERNAME/. (the current directory alias needs to be explicitly written out!)
cd also supports the "-" alias, which just switches to the previous directory.

... thats about it.
