# meteoroid.py

import numpy as np
from math import cos as cos
from math import sin as sin
import random
import OpenGL.GL as gl
import math
import time

from gl_implementation.GoldmannSheet import GoldmannSheet, PolarPoint
from gl_implementation.settings import Settings
from helper_functions import draw_sphere_object, calc_rotation_matrix
from gl_implementation.goldmann_sizes import MeteoroideSize, Size_V, Size_I


def get_polar_coordinates(x, y, z):
    """
    Calculate the spherical coordinates (theta, phi) from Cartesian coordinates (x, y, z).

    This function computes the angular coordinates theta (polar angle) and phi
    (azimuthal angle) for a point in 3D Cartesian space. The input Cartesian
    coordinates are used to derive these angles in the spherical coordinate system.

    Args:
        x (float): The x-coordinate in Cartesian space.
        y (float): The y-coordinate in Cartesian space.
        z (float): The z-coordinate in Cartesian space.

    Returns:
        tuple[float, float]: A tuple containing:
            - theta: The polar angle (in radians) from the positive z-axis.
            - phi: The azimuthal angle (in radians) in the xy-plane from the
              positive x-axis.
    """
    r = np.sqrt(x ** 2 + y ** 2 + z ** 2)
    theta = math.acos(z / r) if r != 0 else 0
    phi = np.arctan2(y, x)
    return theta, phi


class Meteoroid:
    def __init__(self, settings: Settings):
        self.current_longitude_start_time = None
        self.radius = settings.meteoroid_distance
        self.random_longitude = settings.meteoroid_random
        self.longitudes_original = settings.meteoroid_longitudes_deg
        self.longitudes = self.longitudes_original.copy()
        self.test_sizes = settings.meteoroid_sizes
        self.current_longitude_index = None
        self.meteoroid_speed = settings.meteoroid_speed
        self.sec_per_longitude = 90 / self.meteoroid_speed
        self.passed_seconds = time.time()  # Variable for easy access (Point detection function)
        self.current_size = None
        self.goldmann_sheet = GoldmannSheet()
        self.perimetry_status = "ready"

        self.meteoroid_color = settings.meteoroid_color
        self.general_thales_point = settings.general_thales_point

        self.R = calc_rotation_matrix(
            self.general_thales_point, np.array([0, 0, -self.radius], dtype=float)
        )
        self.paused_star_position = np.array([0, 0, -self.radius])
        self.paused_star_size = Size_I
        self.paused_passed_seconds = 0.0
        self.paused_star_p = None

    @staticmethod
    def _theta_from_regler(theta_regler):
        return theta_regler * np.pi / 2 + (np.pi / 2)


    def _min_max_normalization(self, value):
        return 2*((value - (-self.radius)) / (self.radius - -self.radius)) - 1

    def _get_coordinates(self, longitude, theta_regler):
        theta = self._theta_from_regler(theta_regler)
        phi = np.deg2rad(longitude)
        x = sin(theta) * cos(phi) * self.radius
        y = sin(theta) * sin(phi) * self.radius
        z = cos(theta) * self.radius
        light_point_raw = np.array([x, y, z], dtype=float)
        p_vec = light_point_raw - self.general_thales_point
        theta_p = self._min_max_normalization(p_vec[1]) * 90
        phi_t = self._min_max_normalization(p_vec[0]) * 90
        p = PolarPoint(theta_p, phi_t)
        # print(f"\r{p}", end="")
        light_point = self.R @ light_point_raw
        return light_point, p

    def _get_longitude_line(self, longitude):
        theta = np.linspace(np.pi / 2, np.pi, 100)
        phi = np.deg2rad(longitude)
        x = np.sin(theta) * np.cos(phi)
        y = np.sin(theta) * np.sin(phi)
        z = np.cos(theta)
        line_raw = np.array([x, y, z], dtype=float)
        line = self.R @ line_raw
        return line

    def start_animation(self):
        if self.random_longitude:
            random.shuffle(self.longitudes)
        self.current_longitude_start_time = time.time()
        self.current_longitude_index = 0
        self.current_size = Size_V
        use_size = self.test_sizes[self.current_size]
        while use_size is False:
            self.current_size = self.current_size().get_next_size()
            use_size = self.test_sizes[self.current_size]
        self.perimetry_status = "running"
        if self.current_size is None:
            raise Exception("Perimetry can not be started with no Size in settings set to true")

    def pause_animation(self):
        if self.perimetry_status == "running":
            self.paused_star_position, self.paused_star_size, self.paused_star_p = self.get_current_point()
            longitude_index = self.current_longitude_index + int(self.passed_seconds // self.sec_per_longitude)
            sec_in_longitude = self.passed_seconds % self.sec_per_longitude
            self.current_longitude_index = longitude_index
            self.current_longitude_start_time = None
            self.paused_passed_seconds = sec_in_longitude
            self.perimetry_status = "paused"
        elif self.perimetry_status == "paused":
            return
        else:
            raise Exception("Perimetry can not be paused at the moment")

    def resume_animation(self):
        if self.perimetry_status == "paused":
            self.current_longitude_start_time = time.time() - self.paused_passed_seconds
            self.paused_star_position = np.array([0, 0, -self.radius])
            self.paused_star_size = Size_I
            self.paused_star_p = None
            self.perimetry_status = "running"
        else:
            raise Exception("Perimetry can not be paused at the moment")

    def _reset_animation(self):
        self.current_longitude_start_time = None
        self.current_longitude_index = None
        self.passed_seconds = 0.0

    def star_position_changed(self, star_position:list):
        self.R = calc_rotation_matrix(
            self.general_thales_point, np.array(star_position, dtype=float)
        )

    def get_current_point(self):
        if self.current_longitude_start_time is None:
            raise Exception("Perimetry not started yet.")
        if self.perimetry_status != "running":
            if self.perimetry_status == "paused":
                return self.paused_star_position, self.paused_star_size
            return self.perimetry_status, None

        self.passed_seconds = time.time() - self.current_longitude_start_time
        longitude_index = self.current_longitude_index + int(self.passed_seconds // self.sec_per_longitude)
        if longitude_index < len(self.longitudes) - 1:
            sec_in_longitude = self.passed_seconds % self.sec_per_longitude
            deg = sec_in_longitude * self.meteoroid_speed
            theta_regler = (deg / 90)
            light_point, p = self._get_coordinates(
                self.longitudes[longitude_index], theta_regler
            )
            return light_point, self.current_size, p
        else:  # Min. One Size Done
            # get next point size
            next_size = self.current_size().get_next_size()
            if next_size is not None:
                self.current_size = next_size
                use_size = self.test_sizes[self.current_size]
                if use_size is True: # continue with the next size
                    next_longitudes = self.longitudes_original.copy()
                    if self.random_longitude:
                        random.shuffle(next_longitudes)
                    self.longitudes.extend(next_longitudes)
                    return self.get_current_point()
                else:  # use size is false
                    return self.get_current_point()
            else:  # All defined Sizes done
                self._reset_animation()
                self.perimetry_status = "done"
                return "done", None, None


    def point_detected(self):
        """
        If the patient "presses" button, because he/she sees the moving point
        """
        self.pause_animation()  # Pause animation so no access of the relevant variables, while detecting point
        longitude_index = self.current_longitude_index + int(self.passed_seconds // self.sec_per_longitude)
        # sec_in_longitude = self.passed_seconds % self.sec_per_longitude
        p = self.paused_star_p
        self.goldmann_sheet.add_point(
            p=self.paused_star_p, size=self.paused_star_size
        )
        print(f"Detected Point at Longitude {self.longitudes[longitude_index]}° is {p}")
        self.resume_animation()
        self.current_longitude_index = longitude_index + 1
        self.current_longitude_start_time = time.time()

    def draw(self):
        if self.perimetry_status == "ready":
            print("Perimetry not started yet.")
            return False
        elif self.perimetry_status == "done":
            print("Perimetry done, reset to ready to start again")
            return False
        elif self.perimetry_status == "paused":
            point, current_size = self.paused_star_position, self.paused_star_size
        elif self.perimetry_status == "running":
            point, current_size, p = self.get_current_point()
        else:
            raise Exception("Perimetry status not recognized")

        # if phi % 0.2:
        # print(f"\r phi={phi} theta={theta}", end="")
        gl.glPushMatrix()
        gl.glTranslatef(*point)
        gl.glColor3f(*self.meteoroid_color)
        gl.glDisable(gl.GL_LIGHTING)
        size: MeteoroideSize = self.current_size()
        size.set_distance(
            self.radius
        )
        draw_sphere_object(size.get_size_meter2(), 30, 30)
        gl.glEnable(gl.GL_LIGHTING)
        gl.glPopMatrix()
        return True

    def draw_meteoroid_line(self):
        for i, lg in enumerate(self.longitudes_original):
            if i == 0:
                gl.glColor3f(0, 1, 0)  # green
            else:
                gl.glColor3f(1, 1, 1)  # white
            x, y, z = self._get_longitude_line(lg)
            gl.glBegin(gl.GL_LINE_STRIP)
            for xi, yi, zi in zip(x, y, z):
                gl.glVertex3f(xi, yi, zi)
            gl.glEnd()
