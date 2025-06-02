# Assignment 2 Culling 

It work with some issues that will be fixed at some point, hopefully

## Controls
W-A-S-D for movement 
Mouse movement determines the direction and it is inverted

P toggles the mouse on/off to interact with the ImGui UI

ImGui lets user to manage the FOV, Near Plane and Far Plane
It also show the amount of objects in scene and how many are being rendered
also tries to show FPS but it may or may not work that much?
And the buttons are for rendering the quadtree and the bounding volumes if needed


Blue cubes the parents and green are the childs. Yes it could be better colour coded
Also not all visualitions of the bounding volumes seem to be correct but the culling works so that is another point that needs work to bedone on.

## Images
The following are some images examples of the program 
![alt-text](https://github.com/JcMN/C--Frustum-and-Space-Culling/blob/master/images/Culling%20SC.png "Culling Example Image 1")

![alt-text](https://github.com/JcMN/C--Frustum-and-Space-Culling/blob/master/images/Culling%20SC%202.png "Culling Exmaple 2")

![alt-text](https://github.com/JcMN/C--Frustum-and-Space-Culling/blob/master/images/Culling%20SC%203.png "Culling Example 3")

![alt-text](https://github.com/JcMN/C--Frustum-and-Space-Culling/blob/master/images/Culling%20SC4.png "Culling Example 4")

## Run 
To run you need CMAKE on Visual Studio Code, delete the build folder, and Configure with the Visual Studio compiler and Build. Then just run Debug with CMAKE and a window should opne with the program running

