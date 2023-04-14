"""
ISPPJ1 2023
Study Case: Breakout

Author: Alejandro Mujica
alejandro.j.mujic4@gmail.com

This file contains the class to define the Play state.
"""
import random

import pygame

from gale.factory import AbstractFactory
from gale.state_machine import BaseState
from gale.input_handler import InputHandler, InputData, InputData
from gale.text import render_text
from gale.timer import Timer

import settings
import src.powerups


class PlayState(BaseState):
    def enter(self, **params: dict):
        self.level = params["level"]
        self.score = params["score"]
        self.lives = params["lives"]
        self.paddle = params["paddle"]
        self.balls = params["balls"]
        self.brickset = params["brickset"]
        self.live_factor = params["live_factor"]
        self.points_to_next_live = params["points_to_next_live"]
        self.points_to_next_grow_up = (
            self.score
            + settings.PADDLE_GROW_UP_POINTS * (self.paddle.size + 1) * self.level
        )
        self.powerups = params.get("powerups", [])

        if not params.get("resume", False):
            self.balls[0].vx = random.randint(-80, 80)
            self.balls[0].vy = random.randint(-170, -100)
            settings.SOUNDS["paddle_hit"].play()

        self.powerups_abstract_factory = AbstractFactory("src.powerups")
        self.powerups_taken = params.get("powerups_taken", [])
        self.paddle_powerups_map = params.get("paddle_powerups_map", {})
        self.bullets = params.get("bullets", [])

        # resume powerup's timer
        for powerup in self.powerups_taken:
            if (
                hasattr(powerup, "timer")
                and hasattr(powerup, "set_timer")
                and callable(powerup.set_timer)
            ):
                timer = powerup.timer
                powerup.set_timer(self, timer)

        InputHandler.register_listener(self)

    def exit(self) -> None:
        InputHandler.unregister_listener(self)
        Timer.clear()

    def update(self, dt: float) -> None:
        self.paddle.update(dt)

        for ball in self.balls:
            ball.update(dt)
            ball.solve_world_boundaries()

            # Check collision with the paddle
            if ball.collides(self.paddle):
                if "sticky_paddle" in self.paddle_powerups_map:
                    # sticky paddle effect
                    src.powerups.StickyPaddle.stick(ball, self)
                else:
                    # default
                    settings.SOUNDS["paddle_hit"].stop()
                    settings.SOUNDS["paddle_hit"].play()
                    ball.rebound(self.paddle)
                    ball.push(self.paddle)

            # reflector ward effect
            if "reflector_ward" in self.paddle_powerups_map:
                src.powerups.ReflectorWard.reflect(ball)

            # Check collision with brickset
            if not ball.collides(self.brickset):
                continue

            brick = self.brickset.get_colliding_brick(ball.get_collision_rect())

            if brick is None:
                continue

            brick.hit()
            self.score += brick.score()
            ball.rebound(brick)

            # Check earn life
            if self.score >= self.points_to_next_live:
                settings.SOUNDS["life"].play()
                self.lives = min(3, self.lives + 1)
                self.live_factor += 0.5
                self.points_to_next_live += settings.LIVE_POINTS_BASE * self.live_factor

            # Check growing up of the paddle
            if self.score >= self.points_to_next_grow_up:
                settings.SOUNDS["grow_up"].play()
                self.points_to_next_grow_up += (
                    settings.PADDLE_GROW_UP_POINTS * (self.paddle.size + 1) * self.level
                )
                self.paddle.inc_size()

            # Chance to generate two more balls
            if random.random() < 0.1:
                r = brick.get_collision_rect()
                self.powerups.append(
                    self.powerups_abstract_factory.get_factory("TwoMoreBall").create(
                        r.centerx - 8, r.centery - 8
                    )
                )

            # Chance to generate sticky paddle powerup
            if random.random() < 0.1:
                r = brick.get_collision_rect()
                self.powerups.append(
                    self.powerups_abstract_factory.get_factory("StickyPaddle").create(
                        r.centerx - 8, r.centery - 8
                    )
                )

            # Chance to generate mini gun
            if random.random() < 0.1:
                r = brick.get_collision_rect()
                self.powerups.append(
                    self.powerups_abstract_factory.get_factory("MiniGun").create(
                        r.centerx - 8, r.centery - 8
                    )
                )

            # Chance to generate reflector ward
            if random.random() < 0.1:
                r = brick.get_collision_rect()
                self.powerups.append(
                    self.powerups_abstract_factory.get_factory("ReflectorWard").create(
                        r.centerx - 8, r.centery - 8
                    )
                )

        # Removing all balls that are not in play
        self.balls = [ball for ball in self.balls if ball.in_play]

        for bullet in self.bullets:
            bullet.update(dt)
            bullet.solve_world_boundaries()

            # Check collision with brickset
            if not bullet.collides(self.brickset):
                continue

            brick = self.brickset.get_colliding_brick(bullet.get_collision_rect())

            if brick is None:
                continue

            brick.hit()
            self.score += brick.score()
            bullet.in_play = False

        # Removing all bullets that are not in play
        self.bullets = [bullet for bullet in self.bullets if bullet.in_play]

        self.brickset.update(dt)

        if not self.balls:
            self.lives -= 1
            if self.lives == 0:
                self.state_machine.change("game_over", score=self.score)
            else:
                self.paddle.dec_size()
                self.state_machine.change(
                    "serve",
                    level=self.level,
                    score=self.score,
                    lives=self.lives,
                    paddle=self.paddle,
                    brickset=self.brickset,
                    points_to_next_live=self.points_to_next_live,
                    live_factor=self.live_factor,
                )

        # Update powerups
        for powerup in self.powerups:
            powerup.update(dt)

            if powerup.collides(self.paddle):
                powerup.take(self)

        # Remove powerups that are not in play
        self.powerups = [p for p in self.powerups if p.in_play]

        # Check victory
        if self.brickset.size == 1 and next(
            (True for _, b in self.brickset.bricks.items() if b.broken), False
        ):
            self.state_machine.change(
                "victory",
                lives=self.lives,
                level=self.level,
                score=self.score,
                paddle=self.paddle,
                balls=self.balls,
                points_to_next_live=self.points_to_next_live,
                live_factor=self.live_factor,
            )

    def render(self, surface: pygame.Surface) -> None:
        heart_x = settings.VIRTUAL_WIDTH - 120

        i = 0
        # Draw filled hearts
        while i < self.lives:
            surface.blit(
                settings.TEXTURES["hearts"], (heart_x, 5), settings.FRAMES["hearts"][0]
            )
            heart_x += 11
            i += 1

        # Draw empty hearts
        while i < 3:
            surface.blit(
                settings.TEXTURES["hearts"], (heart_x, 5), settings.FRAMES["hearts"][1]
            )
            heart_x += 11
            i += 1

        render_text(
            surface,
            f"Score: {self.score}",
            settings.FONTS["tiny"],
            settings.VIRTUAL_WIDTH - 80,
            5,
            (255, 255, 255),
        )

        # draw taken powerups icons
        powerup_dict = {}
        for powerup in self.powerups_taken:
            class_name = powerup.__class__.__name__
            if class_name not in powerup_dict:
                powerup_dict[class_name] = powerup

        powerup_x = 0
        for class_name, powerup in powerup_dict.items():
            surface.blit(
                settings.TEXTURES["spritesheet"],
                (powerup_x, self.paddle.y - 22),
                settings.FRAMES["powerups"][powerup.frame],
            )
            powerup_x += 18

        self.brickset.render(surface)

        self.paddle.render(surface)

        for ball in self.balls:
            ball.render(surface)

        for powerup in self.powerups:
            powerup.render(surface)

        if "minigun" in self.paddle_powerups_map:
            src.powerups.MiniGun.render_cannons(self, surface)

        # draw fired bullets
        for bullet in self.bullets:
            bullet.render(surface)

    def on_input(self, input_id: str, input_data: InputData) -> None:
        if input_id == "move_left":
            if input_data.pressed:
                self.paddle.vx = -settings.PADDLE_SPEED
            elif input_data.released and self.paddle.vx < 0:
                self.paddle.vx = 0
        elif input_id == "move_right":
            if input_data.pressed:
                self.paddle.vx = settings.PADDLE_SPEED
            elif input_data.released and self.paddle.vx > 0:
                self.paddle.vx = 0
        elif input_id == "pause" and input_data.pressed:
            self.state_machine.change(
                "pause",
                level=self.level,
                score=self.score,
                lives=self.lives,
                paddle=self.paddle,
                balls=self.balls,
                brickset=self.brickset,
                points_to_next_live=self.points_to_next_live,
                live_factor=self.live_factor,
                powerups=self.powerups,
                powerups_taken=self.powerups_taken,
                paddle_powerups_map=self.paddle_powerups_map,
                bullets=self.bullets,
            )
        elif input_id == "release" and input_data.pressed:
            if "sticky_paddle" in self.paddle_powerups_map:
                # release sticky balls if any
                for ball in self.paddle_powerups_map["sticky_paddle"]["balls"]:
                    ball[0].vx = random.randint(-80, 80)
                    ball[0].vy = random.randint(-170, -100)
                self.paddle_powerups_map["sticky_paddle"]["balls"] = []
        elif input_id == "fire" and input_data.pressed:
            if "minigun" in self.paddle_powerups_map:
                if len(self.bullets) == 0:
                    src.powerups.MiniGun.fire(self)
