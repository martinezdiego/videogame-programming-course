from typing import NoReturn

import pygame

from src.Board import Board
from src.Tile import Tile


class PowerUp(Tile):
    """
    The base power-up.
    """

    COLOR_PALETTES = [
        (217, 160, 102),
        (217, 87, 99),
        (138, 111, 48),
        (217, 87, 99),
        (82, 75, 36),
        (172, 50, 50),
        (75, 105, 47),
        (102, 57, 49),
        (55, 148, 110),
        (143, 86, 59),
        (91, 110, 225),
        (223, 113, 38),
        (48, 96, 130),
        (132, 126, 135),
        (63, 63, 116),
        (105, 106, 106),
        (118, 66, 138),
        (89, 86, 82),
    ]

    def __init__(self, i: int, j: int, color: int, variety: int) -> None:
        super().__init__(i, j, color, variety)

    def render(self, surface: pygame.Surface, offset_x: int, offset_y: int) -> NoReturn:
        raise NotImplementedError

    def exec(self, board: Board) -> NoReturn:
        raise NotImplementedError
