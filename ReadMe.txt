Completed for CPSC587A3

Use buttons 1-6 to cycle through scenes
1: Single mass spring
2: Chain
3: Cube of jelly
4: Curtain
5: Flapping flag 
6: Cloth on a table





Bonus extension 1 explanation:
The wind blown flag was accomplished using the calculations below

p (velocity of triangle which is the average velocity of masses that comprise it)
v (flow velocity)
vr(relative velocity) = v - p

alpha (coupling strength between flow and triangle)

n (normal to triangle) = normalized cross-product of two vectors of the triangle
vn (normal velocity) = dot(n, normalized(vr)) * v
vt (tangent velocity) = v - vn

A (area of face of the triangle)
*for the simulation alphaN = alphaT

Fn = alphaN A v vn    (normal force)
Ft = alphaT A vt    (tangential force)

These forces were then added to the total force of each of the comprising masses.





Bonus extension 2 explanation:
This scene was accomplished by first producing a horizontal high density sheet of masses with the same cross spring structure as needed for part 4) and left to fall. During each simulation step just after the updated velocity is calculated for each mass, it's current position is checked against the construct of the table. This check is done by first seeing if the mass is anywhere within the table and if not, the velocity is unaffected. If the position is within the table, the position is then check to find which side it is on by checking if the position is between the surface and one unit in. If the mass is found within this range, the velocity components during this simulation that would push it further in is set to zero. 

For example if the mass lands in the middle of the table it would be found to be withing the table bounds, it would be within 1 unit of the top face (this is assumed due to the slow speed of the objects and small simulation time-steps), and if the velocity of the mass has a negative y component, the y component is set to 0 instead. This stops the mass from being pushed further down, but still allows it to be moved side to side and lifted up.



