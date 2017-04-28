# 3DTetris
This is a version of 3D Tetris written in C++ and OpenGL that I wrote for my computer graphics class in college in 2008. 

I took an introduction to computer graphics class in the Fall of 2008.  In this class, everyone had to make a final project to show off to the class.  I chose to make a 3D version of my favorite puzzle game: Tetris!

## How to do rotation: the simple way!

Choose one block of your tetris piece to be the center of the piece.

Create a temporary tetris piece that moves the center piece to the origin of the plane you are rotating on.  For example, if you are rotating on the Z axis then you will want the center to be at 0,0 on the x-y plane.

Rotate the piece along the plane.  So in the case of the x-y plane, then each blocks (x,y) becomes (y,-x) if rotating CW or (-y,x) if rotating CCW.

Now translate each block back from the origin to where it was before (i.e. if your center was originally at (0,2,19) then you originally translated each block -2 on the y axis to the origin (0,0,19).  Now translate each piece back 2 on the y axis such that the center is again at (0,2,19)).

Your rotation is done!  Repeat the process for each axis that you need to rotate along.


![image](https://cloud.githubusercontent.com/assets/690781/25533208/e17facda-2bfd-11e7-83c8-94a5e6b42e52.png)

![image](https://cloud.githubusercontent.com/assets/690781/25533219/ec52090a-2bfd-11e7-903e-9a4be6433612.png)

![image](https://cloud.githubusercontent.com/assets/690781/25533226/f256b698-2bfd-11e7-8278-af35a4466050.png)

![image](https://cloud.githubusercontent.com/assets/690781/25533234/f7a053b6-2bfd-11e7-8ed5-89db9a7e29e0.png)
