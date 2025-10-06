# starry_sky.py

import numpy as np
import OpenGL.GL as gl
import OpenGL.GLU as glu
import math
from helper_functions import draw_sphere_object


class StarrySky:
    """
    Manages the sky sphere and the distribution of stars.
    Stars are placed on a sphere using a Fibonacci lattice for even distribution.
    """

    def __init__(self, settings):
        """
        Initializes the StarrySky and generates star positions.

        Args:
            settings: An object containing configuration parameters.
        """
        self.sky_radius = settings.sky_radius
        self.sky_color = settings.sky_color
        self.star_color = settings.star_color
        self.positions, self.sizes = self._generate_stars(
            num_stars=settings.num_stars,
            distance=settings.star_distance,
            size_variation=settings.star_size_variation,
            position_noise=settings.star_position_noise
        )
        self.quadric = glu.gluNewQuadric()

    def _generate_stars(self, num_stars, distance, size_variation, position_noise):
        """
        Generates evenly distributed star positions and sizes on a sphere's surface.
        Uses a Fibonacci lattice and adds random noise for a natural look.
        """
        positions, sizes = [], []
        golden_ratio = (1 + 5 ** 0.5) / 2

        base_angle_rad = math.radians(0.2)  # Base angle for size calculation

        for i in range(num_stars):
            # Fibonacci lattice for the base position
            theta = 2 * math.pi * i / golden_ratio
            phi = math.acos(1 - 2 * (i + 0.5) / num_stars)

            # Add small random deviations for a more natural appearance
            theta += np.random.uniform(-0.05 * size_variation, 0.05 * size_variation)
            phi += np.random.uniform(-0.05 * size_variation, 0.05 * size_variation)

            # Convert spherical to Cartesian coordinates for the position
            x = distance * math.sin(phi) * math.cos(theta)
            y = distance * math.sin(phi) * math.sin(theta)
            z = distance * math.cos(phi)
            positions.append([x, y, z])

            # Calculate star size based on a base angle and random factor
            base_radius = math.tan(base_angle_rad / 2) * distance
            star_radius = base_radius * np.random.uniform(0.7, position_noise)
            sizes.append(star_radius)

        return positions, sizes

    def draw(self):
        """
        Renders the sky sphere and all the stars.
        """
        # --- Draw the inner sphere for the sky background ---
        glu.gluQuadricDrawStyle(self.quadric, glu.GLU_FILL)
        glu.gluQuadricNormals(self.quadric, glu.GLU_SMOOTH)
        glu.gluQuadricOrientation(self.quadric, glu.GLU_INSIDE)  # Normals point inward
        gl.glDisable(gl.GL_LIGHTING)
        gl.glColor3f(*self.sky_color)
        gl.glPushMatrix()
        gl.glScalef(self.sky_radius, self.sky_radius, self.sky_radius)
        glu.gluSphere(self.quadric, 1.0, 36, 24)  # Unit sphere scaled up
        gl.glPopMatrix()
        gl.glEnable(gl.GL_LIGHTING)

        # --- Draw each star as a small sphere ---
        gl.glColor3f(*self.star_color)
        for pos, radius in zip(self.positions, self.sizes):
            gl.glPushMatrix()
            gl.glTranslatef(*pos)
            draw_sphere_object(radius, 8, 8)  # Low-poly spheres for performance
            gl.glPopMatrix()