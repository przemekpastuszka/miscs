import pygame, sys, os
from pygame.locals import *

initialized, failed = pygame.init()
window = pygame.display.set_mode((800, 600))
pygame.display.set_caption("Playing around")
screen = pygame.display.get_surface()

img = pygame.image.load("big.png")
pos = [0, 0]
direct = [0, 0]

def update_game(events):
    global pos
    global direct
    for event in events:
       if event.key == K_s:
           direct[1] += 1
       if event.key == K_w:
           direct[1] += -1
       if event.key == K_a:
           direct[0] += -1 
       if event.key == K_d:
           direct[0] += 1
    pos[0] += direct[0] * 0.1
    pos[1] += direct[1] * 0.1
        
def render():
    screen.blit(img, pos)
    pygame.display.flip()


while True:
    key_events = []
    for event in pygame.event.get():
        if event.type == QUIT:
            sys.exit(0)
        if event.type == KEYDOWN or event.type == KEYUP:
            key_events.append(event)
    update_game(key_events)
    render()
            
            



