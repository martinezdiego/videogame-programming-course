from typing import TypeVar

import pygame

from gale.timer import Timer

import settings

from src.Board import Board
from src.powerups.PowerUp import PowerUp


class FourTilesMatch(PowerUp):
    def __init__(self, i: int, j: int, color: int, variety: int) -> None:
        super().__init__(i, j, color, variety)

    def render(self, surface: pygame.Surface, offset_x: int, offset_y: int) -> None:
        pygame.draw.circle(
            self.alpha_surface,
            self.COLOR_PALETTES[self.color],
            (
                self.x + offset_x + settings.TILE_SIZE // 2,
                self.y + offset_y + settings.TILE_SIZE // 2,
            ),
            settings.TILE_SIZE // 2,
        )
        pygame.draw.rect(
            self.alpha_surface,
            (34, 32, 52, 200),
            pygame.Rect(0, 0, settings.TILE_SIZE, settings.TILE_SIZE),
            border_radius=7,
        )
        surface.blit(self.alpha_surface, (self.x + 2 + offset_x, self.y + 2 + offset_y))
        pygame.draw.circle(
            surface,
            self.COLOR_PALETTES[self.color],
            (
                self.x + offset_x + settings.TILE_SIZE // 2,
                self.y + offset_y + settings.TILE_SIZE // 2,
            ),
            settings.TILE_SIZE // 2,
        )

    def exec(self, play_state: TypeVar("PlayState")) -> None:
        board = play_state.board
        match = [self]
        directions = [(0, -1), (1, 0), (0, 1), (-1, 0)]

        for x, y in directions:
            neighbor_i = self.i + x
            neighbor_j = self.j + y
            if (
                0 <= neighbor_i < settings.BOARD_HEIGHT
                and 0 <= neighbor_j < settings.BOARD_WIDTH
            ):
                match.append(board.tiles[neighbor_i][neighbor_j])

        board.matches.append(match)
