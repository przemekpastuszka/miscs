import sys
#import and init pygame
import pygame
import random
import math

pygame.init() 

(x_width, y_width) = (1024, 768)
iterations = 80
limit = 4

#create the screen
window = pygame.display.set_mode((x_width, y_width)) 

def blend(alpha, left_color, right_color):
    return tuple([int((1 - alpha) * i + alpha * j) for i, j in zip(left_color, right_color)])

colors = []

def add_gradient(left_color, right_color, amount):
    for i in range(0, amount):
        alpha = float(i) / amount
        colors.append(blend(alpha, left_color, right_color))
        
left = (0, 0, 200)
mid = (200, 200, 0)
right = (255, 255, 255)

add_gradient(left, mid,  iterations / 2)
add_gradient(mid, right, iterations / 2)

class BoundedFractal:
    def pre_condition(self, x0, y0):
        return False
    def next_step(self, z, c):
        pass
        
    def position(self, x, y):
        pass
        
    def render(self, iterations, limit):
        for x in range(0, x_width):
            for y in range(0, y_width):
                (x0, y0) = self.position(float(x) / x_width, float(y) / y_width)
                c = complex(x0, y0)
                z = complex(0, 0)
                
                if self.pre_condition(x0, y0):
                    continue
                
                for k in range(0, iterations):
                    if z.real * z.real + z.imag * z.imag > limit:
                        window.set_at((x, y),  colors[k])
                    z = self.next_step(z, c)
                
            pygame.display.flip()
            
            

class Mandelbrot(BoundedFractal):
    def position(self, x, y):
        return (x * 3.5 - 2.5, y * 2 - 1)
        
    def pre_condition(self, x0, y0):
        if (x0 + 1) ** 2 + y0 ** 2 < 1.0 / 16:
            return True
        p = math.sqrt((x0 - 0.25) ** 2 + y0 ** 2)
        if x0 < p - 2 * p * p  + 0.25:
            return True
        return False
        
    def next_step(self, z, c):
        return z * z + c
        
class BurningShip(BoundedFractal):
    def position(self, x, y):
        return (x * 3.5 - 2, y * 2 - 1.5)
         
    def next_step(self, z, c):
        tmp = complex(abs(z.real), abs(z.imag))
        return tmp * tmp + c

Mandelbrot().render(iterations, limit)

#input handling (somewhat boilerplate code):
while True: 
   for event in pygame.event.get(): 
      if event.type == pygame.QUIT: 
          sys.exit(0) 
      else: 
          print event
   
   
