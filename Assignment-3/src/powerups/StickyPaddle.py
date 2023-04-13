import random
from typing import List, TypeVar

from gale.timer import Timer

from src.powerups.PowerUp import PowerUp
from src.Ball import Ball


class StickyPaddle(PowerUp):
    TIME_FRAME: float = 5.0

    def __init__(self, x: int, y: int, frame: int = 7) -> None:
        super().__init__(x, y, frame)

    def take(self, play_state: TypeVar("PlayState")) -> None:
        # add powerup to taken powerups
        play_state.powerups_taken.append(self)
        # add powerup's effect to paddle
        if "sticky_paddle" not in play_state.paddle_powerups_map:
            play_state.paddle_powerups_map["sticky_paddle"] = {"balls": []}
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
                if isinstance(powerup, StickyPaddle):
                    count += 1
            if count == 1:
                # release sticky balls if any
                for ball in play_state.paddle_powerups_map["sticky_paddle"]["balls"]:
                    ball[0].vx = random.randint(-80, 80)
                    ball[0].vy = random.randint(-170, -100)
                # remove powerup's effect
                play_state.paddle_powerups_map.pop("sticky_paddle")
            play_state.powerups_taken = [
                powerup for powerup in play_state.powerups_taken if powerup != self
            ]

        return cb
