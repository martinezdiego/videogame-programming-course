import random

import pygame

from typing import List, TypeVar

from gale.timer import Timer
from gale.factory import Factory

# import settings
from src.powerups.PowerUp import PowerUp
from src.Bullet import Bullet

class MiniGun(PowerUp):
    TIME_FRAME: float = 8.0
    BULLET_VY = -200.0
    CANNON_WIDTH = 5
    CANNON_HEIGHT = 10
    CANNON_OFFSET = 8
    
    bullet_factory = Factory(Bullet)

    def __init__(self, x: int, y: int, frame: int = 6) -> None:
        super().__init__(x, y, frame)

    def take(self, play_state: TypeVar("PlayState")) -> None:
        # add powerup to taken powerups
        play_state.powerups_taken.append(self)
        # add powerup's effect to paddle
        if "minigun" not in play_state.paddle_powerups_map:
            play_state.paddle_powerups_map["minigun"] = {}
        # set powerup's duration
        self.set_timer(play_state, self.TIME_FRAME)
        # consume powerup
        self.in_play = False

    def set_timer(self, play_state: TypeVar("PlayState"), time_frame: float) -> None:
        self.timer = time_frame
        Timer.after(time_frame, self.on_finish(play_state))
        Timer.every(1, self.countdown_timer)

    def countdown_timer(self) -> None:
        self.timer -= 1

    def on_finish(self, play_state: TypeVar("PlayState")) -> None:
        def cb():
            count = 0
            for powerup in play_state.powerups_taken:
                if isinstance(powerup, MiniGun):
                    count += 1
            if count == 1:
                # remove powerup's effect
                play_state.paddle_powerups_map.pop("minigun")
            play_state.powerups_taken = [
                powerup for powerup in play_state.powerups_taken if powerup != self
            ]

        return cb
    
    @classmethod
    def render_cannons(cls, play_state: TypeVar("PlayState"), surface: pygame.Surface) -> None:
        # draw cannons
        cannon_x = play_state.paddle.x - cls.CANNON_OFFSET - cls.CANNON_WIDTH
        cannon_y = play_state.paddle.y + play_state.paddle.height // 2 - cls.CANNON_HEIGHT // 2
        # left cannon
        pygame.draw.rect(surface, (255, 0, 0), (cannon_x, cannon_y, cls.CANNON_WIDTH, cls.CANNON_HEIGHT))
        # right cannon
        cannon_x = play_state.paddle.x + play_state.paddle.width + cls.CANNON_OFFSET
        pygame.draw.rect(surface, (255, 0, 0), (cannon_x, cannon_y, cls.CANNON_WIDTH, cls.CANNON_HEIGHT))

    @classmethod
    def fire(cls, play_state: TypeVar("PlayState")) -> None:
        cannon_width = 5
        cannon_height = 10
        cannon_offset = 8
        bullet_cx = play_state.paddle.x - cannon_offset - cannon_width + Bullet.WIDTH // 2
        bullet_cy = play_state.paddle.y + play_state.paddle.height // 2 - cannon_height // 2 - Bullet.HEIGHT // 2
        # left cannon
        b = cls.bullet_factory.create(bullet_cx, bullet_cy)
        # settings.SOUNDS["fire"].stop()
        # settings.SOUNDS["fire"].play()
        b.vy = cls.BULLET_VY
        play_state.bullets.append(b)
        # right cannon
        bullet_cx = play_state.paddle.x + play_state.paddle.width + cannon_offset + Bullet.WIDTH // 2
        b = cls.bullet_factory.create(bullet_cx, bullet_cy)
        b.vy = cls.BULLET_VY
        play_state.bullets.append(b)


