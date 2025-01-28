class Position:
    _MIN_POS = 0
    _MAX_POS = 7

    def __init__(self, x, y) -> None:
        assert x >= self._MIN_POS and x <= self._MAX_POS, f"Invalid argument: x should be between {self._MIN_POS} and {self._MAX_POS}, but is {x}"
        assert y >= self._MIN_POS and y <= self._MAX_POS, f"Invalid argument: y should be between {self._MIN_POS} and {self._MAX_POS}, but is {y}"

        self.x = x
        self.y = y

    def __eq__(self, value: object, /) -> bool:
        if type(value) != type(self):
            return False
        return value.x == self.x and value.y == self.y

    def __hash__(self) -> int:
        return hash((self.x, self.y))

    def __str__(self) -> str:
        return f"{self.x, self.y}"

    def __repr__(self) -> str:
        return str(self)

