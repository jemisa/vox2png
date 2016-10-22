vox2png
=======
This program was inspired by [this post](https://www.reddit.com/r/gamedev/comments/57x7n3/really_cool_fake_3d_in_gamemaker_by_the_creator/) on /r/gamedev.  
The technique described here not only looks nice, but allows you to do voxel graphics in purely 2D engines (as long as you can do matrix transformations).  
The big downside is that it is an absolute PITA to create assets for, because you need to draw your voxel object layer-by-layer by hand. 

To make this a lot easier I decided to write this program.  
It takes a [MagicaVoxel](http://ephtracy.github.io/) .vox file, and creates a .png sprite sheet from it.
To show you what I mean by that, here are some screenshots:

Source model:
![From this](http://i.imgur.com/O9bU8nE.png)
Resulting sprite sheet:
![To this](http://i.imgur.com/eAYoJAF.png)

// TODO Add an in engine screenshot here

Format
---------
As can be seen in the screenshot it currently just puts all the Z layers next to each other on the X axis.  
I might add an option to export each Z layer as a different file.  
If you want to see a certain feature, just ask me and I'll add it. Or, if you know C, it shouldn't be hard to do it yourself.

Building and running
----------------------------
Vox2png doesn't have any dependencies except the C Standard Library. Just compile `vox2png.c` with your C compiler, like this:

	gcc vox2png.c -o vox2png

Then you just run it like this:

	./vox2png input.vox output.png

