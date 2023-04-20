from typing import TypeVar

import pygame

from gale.timer import Timer

import settings

from src.Board import Board
from src.powerups.PowerUp import PowerUp


class FiveTilesMatch(PowerUp):
    def __init__(self, i: int, j: int, color: int, variety: int) -> None:
        super().__init__(i, j, color, variety)

    def render(self, surface: pygame.Surface, offset_x: int, offset_y: int) -> None:
        pygame.draw.polygon(
            self.alpha_surface,
            self.COLOR_PALETTES[self.color],
            [
                (
                    self.x + offset_x + settings.TILE_SIZE // 2,
                    self.y + offset_y,
                ),
                (self.x + offset_x, self.y + offset_y + settings.TILE_SIZE),
                (
                    self.x + offset_x + settings.TILE_SIZE,
                    self.y + offset_y + settings.TILE_SIZE,
                ),
            ]
        )
        surface.blit(self.alpha_surface, (self.x + 2 + offset_x, self.y + 2 + offset_y))
        pygame.draw.polygon(
            surface,
            self.COLOR_PALETTES[self.color],
            [
                (
                    self.x + offset_x + settings.TILE_SIZE // 2,
                    self.y + offset_y,
                ),
                (self.x + offset_x, self.y + offset_y + settings.TILE_SIZE),
                (
                    self.x + offset_x + settings.TILE_SIZE,
                    self.y + offset_y + settings.TILE_SIZE,
                ),
            ]
        )

    def exec(self, play_state: TypeVar("PlayState")) -> None:
        board = play_state.board
        match = [self]
        
        for i in range(settings.BOARD_HEIGHT):
            for j in range(settings.BOARD_WIDTH):
                tile = board.tiles[i][j]
                if (tile.color == self.color):
                    match.append(tile)

        board.matches.append(match)

        settings.SOUNDS["powerup_2"].stop()
        settings.SOUNDS["powerup_2"].play()
