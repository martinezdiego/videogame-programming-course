import random
from typing import Any

import pygame

class Bullet:
    WIDTH = 2
    HEIGHT = 2

    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

        self.vy = 0
        self.in_play = True

    def get_collision_rect(self) -> pygame.Rect:
        return pygame.Rect(self.x, self.y, self.WIDTH, self.HEIGHT)

    def solve_world_boundaries(self) -> None:
        r = self.get_collision_rect()

        if r.top < 0:
            self.y = 0
            self.in_play = False

    def collides(self, another: Any) -> bool:
        return self.get_collision_rect().colliderect(another.get_collision_rect())

    def update(self, dt: float) -> None:
        self.y += self.vy * dt

    def render(self, surface):
        pygame.draw.circle(surface, (226, 88, 34), (self.x, self.y), 1.0)
