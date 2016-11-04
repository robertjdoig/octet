# Bullet Physics Rope Bridge

# Brief 
Start with the shapes example and add a bridge using physics constraints. 


# Introduction 

In this project there are few different physical effects happening to the objects in the scene. There are spring constraints holding the bridge planks together, there are forces and torques being added to the ball and swinging box. There is a hinge constraint allowing the box to swing. 


The implementation of the spring and hinge constraints were added to the visual scene file instead of the main header file so that the bullet physics would be abstracted from the program code. This means that if bullet updates or changes how there constraints work, only the visual scene would need to be edited. 


# Spring Constraints

Spring constraints work on all 6 of the axis. In this implementation we are only working on the X-axis. The function in Bullet physics to use a spring takes in six variables. Two rigid bodies, two pivot points and two axis definitions. 

Explanation of each variable:
Rigid body A and B are used to get the physical object that is being constrained together. These rigid bodies also have other forces acting on them. For example the weight of the balls on the bridge. 
The pivot points are the edge of the physics objects that you want to constraint. In this implementation pivot A is the right side of the object and pivot B is the left of the object. This gives a better representation of the real world as the objects are being pulled together from the sides instead of the middle which will result in the object rotating to match each other. 
The axis variables define which axis of the object they are constraining. If the axis on this example were different then it would flip on of the object around until they had the same orientation. The axis are set to the Z axis on both objects.  

The way that it is implemented is very simple, there is an array of rigid bodies which are attached to each of the objects of the bridge. There is one less spring constraint then objects and the first and last object are static so that neither gravity nor forces are applied to the banks
Image of bridge with constraints and objects numbered.  of the bridge.  







# adding a force to the ball

The ball object in the scene has a force applied to it. The force is controlled by the user pressing the arrow keys. The ball is being push around the scene instead of a translation. This means that the mesh is moving due the its rigid body moving.  

The force is a linear force which moves the ball in a straight line. The ball has two axis it can be push on. The X and Z axis via the arrows keys on the keyboard.




# adding a torque to the box



# adding a hinge constraint to the box 

