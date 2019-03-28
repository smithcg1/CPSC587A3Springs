Use buttons 1-6 to cycle through scenes
1: Single mass spring
2: Chain
3: Cube of jelly
4: Curtain
5: Flapping flag 
6: Cloth on a table





Bonus extension 1 explination:


p (velocity of particle)
v (fluid velocity)
v'(relative velocity) = v - p

alpha (coupling strenght between flow and particle)
F = alpha v'



vr (relative velocity)
vn (normal velocity)
vt (tangent velocity)

vr = vn + vt

v (flow speed)
p (density)
A (area of face)
Fn = alphaN A v vn    (normal force)
Ft = alphaT A vt    (tangental force)




average velocity























Bonus extensio 2 explination:
This scene was accomplished by first pruducing a horizontal high density sheet of masses with the same cross spring structure as needed for part 4) and left to fall. During each simulation step just after the updated velocity is calculated for each mass, it's current posision is checked against the construct of the table. This check is done by first seeing if the mass is anywhere within the table and if not, the velocity is unaffected. If the posision is within the table, the posision is then check to find which side it is on by checking if the posision is between the surface and one unit in. If the mass is found within this range, the velocity components during this simulation that would push it further in is set to zero. 

For example if the mass lands in the middle of the table it would be found to be withing the table bounds, it would be within 1 unit of the top face (this is assumed due to the slow speed of the objects and small simulation timesteps), and if the velocity of the mass has a negative y component, the y component is set to 0 instead. This stops the mass from being pushed further down, but still allows it to be moved side to side and lifted up.




