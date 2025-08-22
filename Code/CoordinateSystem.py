from math import sin, cos, acos, atan2, pi, tan
from typing import Self
from enum import Enum


class EuklidPoint:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def to_polar(self, sphere_center: Self, radius: float):
        return SpherePoint(
            theta=acos(self.z / radius),
            phi=atan2(self.y - sphere_center.y, self.x - sphere_center.x)
        )


class SpherePoint:
    def __init__(self, theta, phi):
        self.theta = theta  # Polarwinkel oder auch Längengrad
        self.phi = phi  # Horizontalwinkel oder auch Breitengrad

    def to_euklid(self, sphere_center: EuklidPoint, radius: float):
        return EuklidPoint(
            sphere_center.x + radius * sin(self.theta) * cos(self.phi),
            sphere_center.y + radius * sin(self.theta) * sin(self.phi),
            sphere_center.z + radius * cos(self.theta)
        )


class IntensityAlphabetical(Enum):
    a = 1
    b = 2
    c = 3
    d = 4
    e = 5


class Intensity:
    background = 10 # nits
    max_value = 315  # nits = cd/cm2

    def __init__(
            self,
            intensity_alphabetical: IntensityAlphabetical,
            intensity_numerical: int
    ):
        self.intens_alphabetical = intensity_alphabetical
        self.intens_numerical = intensity_numerical

    def get_nit_value(self):
        db = 19 - ((self.intens_numerical - 1) * 5) - (self.intens_alphabetical.value - 1)
        return self.max_value * (10 ** (db / -10))

    def get_asb_value(self):
        return self.get_nit_value() * pi


class PointSizes(Enum):
    I = 1.7   # degree
    II = 0.8  # degree
    III = 0.43  # degree
    IV = 0.2  # degree
    V = 0.1  # degree

class PointSize:

    def __init__(self, point_size: PointSizes):
        self.point_size = point_size

    def to_metrik_radius(self, distance_to_patient_cm: float = 30.0):
        return tan(self.point_size.value) * distance_to_patient_cm * 0.5



class CoordinateSystemKinetic:

    def __init__(self):
        main_axes = []
        

