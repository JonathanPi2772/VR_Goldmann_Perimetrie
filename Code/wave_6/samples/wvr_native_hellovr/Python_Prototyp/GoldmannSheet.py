# GoldmannSheet.py
# from meteoroid import MeteoroideSize

class PolarPoint:
    def __init__(self, theta, phi):
        self.phi = phi
        self.theta = theta

    def __str__(self):
        return f"(theta={self.theta}, phi={self.phi})"


class GoldmannSheet:
    def __init__(self):
        self._points: list[PolarPoint] = []
        self._sizes: list = []

    def add_point(self, p: PolarPoint, size):
        self._points.append(p)
        self._sizes.append(size)

    def get_points(self):
        return self._points, self._sizes
