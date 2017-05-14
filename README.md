# CGProj2
## Task 1: Grid
 
* Draw a grid on the X-Z plane that covers the rectangle area of (-5,0,-5) to (+5,0,+5) with grid squares of size 1.0x1.0. 
* Draw the positive X axis in red, the Y axis in green and the Z axis in blue. 
* Only draw the positive portion of each axis. Each axis should be of length 5.(or 10 grid blocks).

## Task 2: Camera Rotations

  
Use Perspective projection and locate your camera so you can see the whole scene in your view. Use `glm::LookAt` function to generate the View matrix.

Select the camera using key C. ← and → keys move the camera along the blue circle parallel to the equator. ↑ and ↓ keys rotate the camera along the red circle orthogonal to the equator. The camera always points to the origin. What direction will you choose as the "up" direction?

## Task 3: Draw the haptic device
  
The haptic device consists of the following parts:

* **Base**: placed on the x-z-plane
* **Top (green cylinder)**: placed on top of the base, slightly penetrating the base.
* **Arm1**: a rectangular box coming out of Top (green cylinder) hinged at the center of Top.
* **Joint**: A cylinder of appropriate radius, centered at the other end of arm1.
* **Arm2**: a long cylindrical shaft connected to the joint at the center of the joint
* **Pen**: another shaft of same radius as arm2 connected to the other end of arm2.
* **Button**: a small cylinder (red) on the pen.

Create the objects in the .obj format, e.g. by using Blender. Make sure you also generate correct normals for later use. Your models should be triangulated. 

Use the supplied `void loadObject(char* file, glm::vec4 color, Vertex * &out_Vertices, GLushort* &out_Indices, int ObjectId)` function to load your objects. 

Then call `void createVAOs(Vertex Vertices[], unsigned short Indices[], int ObjectId)` for each object.

**Note**: For each object, use different colors. Do not use the same colors as in the example picture.

## Task 4: Keyboard interaction

Write the code to move the haptic device, rotate the top, rotate the arms and the pen, and twist the pen using the keyboard, as explained below:

* **Pen**: Select the pen using key p. The pen should rotate when the arrow keys are pressed. ←, →, ↑ and ↓ are longitude (J4) and latitude (J5) rotations. (Note that one end is always connected to arm2). shift+← and shift+→ should twist the pen around its axis (J6) (including buttons).
* **Base**: Select the base using key b. The whole model should slide on the XZ plane according to the arrow keys.
* **Top** : Select the top using key t. The top, arms and pen should rotate around Y axis when using the left and right arrow keys (J1).
* **Arm1**: Select arm1 using key 1. The arm (and the other connected arm and pen) should rotate up and down when using the arrow keys (one end is fixed at the center of the top green cylinder) (J2).
* **Arm2**: Select Arm2 using key 2. The arm (and pen) should rotate up and down when using the arrow keys (one	end is fixed at the center of the joint) (J3).
Indicate the selected part by drawing it in a brighter color.  

## Task 5: Light up the scene

* Add two lights to the scene. For each light, you need to supply position, diffuse color, ambient color and specular color. Position the lights near the camera so that one light comes from the left and another one from the right. You are free to choose any light colors and positions as long as the scene looks good.
* You will also need to set the material of the objects. Set diffuse and ambient color to the color of the object. Set specular color to one tenth of the diffuse color. 

## Task 6: Teleporting

* When j (for jump) is pressed, from the back (not the tip) of the pen, have an object of your choice emmenate in an arc (according to Newton's law) with tangent equal to the pen axis and impact point onto the grid. 
* Animate the projectile and on impact move the haptic device to the impact location.


