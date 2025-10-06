# goldmann_sizes.py
from math import pi, tan


class MeteoroideSize:
    def __init__(self, size_deg):
        self.size = size_deg * pi * (1 / 180)
        self.distance = None
        self.size_meter2 = None

    def set_distance(self, distance):
        self.distance = distance

        # A = π * r²
        radius_m = tan(self.size * 0.5) * self.distance
        self.size_meter2 = (radius_m ** 2) * pi

    def get_size_meter2(self):
        if self.size_meter2 is not None:
            return self.size_meter2
        else:
            raise Exception("Size could not be calculated yet, missing Distance.")

    def get_next_size(self):
        raise Exception("Implemented by child class.")


# --- Reference Page 217 - VISUAL FIELD DIGEST | A guide to perimetry and the Octopus perimeter ---
class Size_V(MeteoroideSize):
    def __init__(self):
        super().__init__(1.72)

    def get_next_size(self):
        return Size_IV


class Size_IV(MeteoroideSize):
    def __init__(self):
        super().__init__(0.86)

    def get_next_size(self):
        return Size_III


class Size_III(MeteoroideSize):
    def __init__(self):
        super().__init__(0.43)

    def get_next_size(self):
        return Size_II


class Size_II(MeteoroideSize):
    def __init__(self):
        super().__init__(0.22)

    def get_next_size(self):
        return Size_I


class Size_I(MeteoroideSize):
    def __init__(self):
        super().__init__(0.11)

    def get_next_size(self):
        return None
