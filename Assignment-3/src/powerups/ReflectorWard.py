from typing import TypeVar

from gale.timer import Timer

import settings
from src.powerups import PowerUp
from src.Ball import Ball

class ReflectorWard(PowerUp):
    TIME_FRAME: float = 6.0

    def __init__(self, x: int, y: int, frame: int = 4) -> None:
        super().__init__(x, y, frame)

    def take(self, play_state: TypeVar("PlayState")) -> None:
        # add powerup to taken powerups
        play_state.powerups_taken.append(self)
        # add powerup's effect to paddle
        if "reflector_ward" not in play_state.paddle_powerups_map:
            play_state.paddle_powerups_map["reflector_ward"] = {}
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
                if isinstance(powerup, ReflectorWard):
                    count += 1
            if count == 1:
                # remove powerup's effect
                play_state.paddle_powerups_map.pop("reflector_ward")
            play_state.powerups_taken = [
                powerup for powerup in play_state.powerups_taken if powerup != self
            ]

        return cb
    
    @staticmethod
    def reflect(ball: Ball) -> None:
        r = ball.get_collision_rect()
        if r.top > settings.VIRTUAL_HEIGHT:
            settings.SOUNDS["hurt"].stop()
            settings.SOUNDS["wall_hit"].stop()
            settings.SOUNDS["wall_hit"].play()
            ball.in_play = True
            ball.vy *= -1