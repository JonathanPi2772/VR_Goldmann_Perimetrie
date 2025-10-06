# terrain.py

import numpy as np
import OpenGL.GL as gl
import math


class Terrain:
    """
    Manages the generation and rendering of the mountain terrain mesh.
    The terrain is created using a Gaussian curve.
    """

    def __init__(self, settings):
        """
        Initializes the Terrain object and generates the mesh.

        Args:
            settings: An object containing configuration parameters.
        """
        self.color = settings.mountain_color
        self.vertices, self.normals, self.indices = self._generate_mesh(
            extent=settings.mountain_extent,
            resolution=settings.mountain_resolution,
            peak=settings.peak_amplitude,
            sigma=settings.sigma,
            summit_y=settings.summit_y
        )

    def _generate_mesh(self, extent, resolution, peak, sigma, summit_y):
        """
        Generates the vertices, normals, and indices for the mountain mesh.
        - Uses y as the vertical axis.
        - The summit is placed at the specified summit_y coordinate.
        """
        # The base level is calculated so the summit (at the center) is at summit_y
        base_y = summit_y - peak

        # Create a grid in the x,z (horizontal) plane
        xs = np.linspace(-extent, extent, resolution)
        zs = np.linspace(-extent, extent, resolution)
        dx = xs[1] - xs[0]
        dz = zs[1] - zs[0]

        heights = np.zeros((resolution, resolution), dtype=np.float32)
        for i, x in enumerate(xs):
            for j, z in enumerate(zs):
                r_squared = x * x + z * z
                # Gaussian function for the mountain shape
                g = peak * math.exp(-r_squared / (2.0 * sigma * sigma))
                heights[i, j] = base_y + g  # The y-coordinate represents height

        # Compute normals using central differences for smooth lighting
        normals = np.zeros((resolution, resolution, 3), dtype=np.float32)
        for i in range(resolution):
            for j in range(resolution):
                hL = heights[i - 1, j] if i > 0 else heights[i, j]
                hR = heights[i + 1, j] if i < resolution - 1 else heights[i, j]
                hD = heights[i, j - 1] if j > 0 else heights[i, j]
                hU = heights[i, j + 1] if j < resolution - 1 else heights[i, j]
                dhdx = (hR - hL) / (2.0 * dx)
                dhdz = (hU - hD) / (2.0 * dz)
                n = np.array([-dhdx, 1.0, -dhdz], dtype=np.float32)
                n /= np.linalg.norm(n)  # Normalize the vector
                normals[i, j] = n

        # Create flat arrays for OpenGL
        verts = []
        norms = []
        for i, x in enumerate(xs):
            for j, z in enumerate(zs):
                y = heights[i, j]
                verts.append((x, y, z))
                norms.append(tuple(normals[i, j]))

        verts = np.array(verts, dtype=np.float32)
        norms = np.array(norms, dtype=np.float32)

        # Generate indices for drawing triangles
        indices = []

        def get_index(i, j):
            return i * resolution + j

        for i in range(resolution - 1):
            for j in range(resolution - 1):
                a = get_index(i, j)
                b = get_index(i + 1, j)
                c = get_index(i + 1, j + 1)
                d = get_index(i, j + 1)
                indices.extend([a, b, c])  # First triangle
                indices.extend([a, c, d])  # Second triangle
        indices = np.array(indices, dtype=np.uint32)
        return verts, norms, indices

    def draw(self):
        """
        Renders the mountain using vertex arrays.
        """
        gl.glColor3f(*self.color)
        gl.glEnableClientState(gl.GL_VERTEX_ARRAY)
        gl.glEnableClientState(gl.GL_NORMAL_ARRAY)
        gl.glVertexPointer(3, gl.GL_FLOAT, 0, self.vertices)
        gl.glNormalPointer(gl.GL_FLOAT, 0, self.normals)
        gl.glDrawElements(gl.GL_TRIANGLES, len(self.indices), gl.GL_UNSIGNED_INT, self.indices)
        gl.glDisableClientState(gl.GL_NORMAL_ARRAY)
        gl.glDisableClientState(gl.GL_VERTEX_ARRAY)