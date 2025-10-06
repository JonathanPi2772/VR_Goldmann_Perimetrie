import OpenGL.GL as gl
import math
import numpy as np


def draw_sphere_object(radius, slices=20, stacks=20):
    """
    Draws a 3D sphere object using OpenGL methods.

    The function renders a sphere in a 3D space by calculating its vertices and
    normals based on the given radius, number of slices, and stacks. The sphere is
    generated through a series of quad strips, controlled by latitude and
    longitude divisions. Precision of the rendering is determined by the slices
    and stacks parameters, affecting the visual smoothness of the output.

    Parameters:
        radius: float
            The radius of the sphere to be drawn.
        slices: int, optional
            The number of longitudinal subdivisions. Higher values result in a
            smoother sphere. Defaults to 20.
        stacks: int, optional
            The number of latitudinal subdivisions. Higher values result in a
            smoother sphere. Defaults to 20.

    Raises:
        None
    """
    for i in range(stacks):
        lat0 = math.pi * (-0.5 + float(i) / stacks)
        z0, zr0 = math.sin(lat0) * radius, math.cos(lat0) * radius
        lat1 = math.pi * (-0.5 + float(i + 1) / stacks)
        z1, zr1 = math.sin(lat1) * radius, math.cos(lat1) * radius
        gl.glBegin(gl.GL_QUAD_STRIP)
        for j in range(slices + 1):
            lng = 2 * math.pi * float(j) / slices
            x, y = math.cos(lng), math.sin(lng)
            gl.glNormal3f(x * zr0, y * zr0, z0)
            gl.glVertex3f(x * zr0, y * zr0, z0)
            gl.glNormal3f(x * zr1, y * zr1, z1)
            gl.glVertex3f(x * zr1, y * zr1, z1)
        gl.glEnd()

def calc_rotation_matrix(a, b, world_up=np.array([0, 1, 0])):
    """
    Berechnet die Rotationsmatrix, die a auf b dreht und dabei eine
    stabile "Oben"-Ausrichtung beibehält, um einen Roll-Effekt zu vermeiden.

    Args:
        a (np.ndarray): Der 3D-Ausgangsvektor.
        b (np.ndarray): Der 3D-Zielvektor.
        world_up (np.ndarray, optional): Der globale "Oben"-Vektor.

    Returns:
        np.ndarray: Die 3x3-Rotationsmatrix.
    """
    # --- Orientierungsmatrix für A bauen ---
    z_axis_a = a / np.linalg.norm(a)
    x_axis_a = np.cross(world_up, z_axis_a)

    # Sonderfall: a ist parallel zu world_up
    if np.linalg.norm(x_axis_a) < 1e-6:
        # Wenn a nach oben zeigt, nehmen wir z als "vorne"
        x_axis_a = np.cross(np.array([0, 0, 1]), z_axis_a)
        # Wenn a auch parallel zu z ist (0,1,0) x (0,0,1) -> (-1,0,0)
        # Wenn a nach unten zeigt (-1,0,0)
        if np.linalg.norm(x_axis_a) < 1e-6:  # a ist (0,0,1) oder (0,0,-1)
            x_axis_a = np.cross(np.array([1, 0, 0]), z_axis_a)

    x_axis_a /= np.linalg.norm(x_axis_a)
    y_axis_a = np.cross(z_axis_a, x_axis_a)

    matrix_a = np.column_stack([x_axis_a, y_axis_a, z_axis_a])

    # --- Orientierungsmatrix für B bauen ---
    z_axis_b = b / np.linalg.norm(b)
    x_axis_b = np.cross(world_up, z_axis_b)

    if np.linalg.norm(x_axis_b) < 1e-6:
        x_axis_b = np.cross(np.array([0, 0, 1]), z_axis_b)
        if np.linalg.norm(x_axis_b) < 1e-6:
            x_axis_b = np.cross(np.array([1, 0, 0]), z_axis_b)

    x_axis_b /= np.linalg.norm(x_axis_b)
    y_axis_b = np.cross(z_axis_b, x_axis_b)

    matrix_b = np.column_stack([x_axis_b, y_axis_b, z_axis_b])

    # --- Finale Rotationsmatrix berechnen: R = B * A^T ---
    return matrix_b @ matrix_a.T


# not necessary, only for understandig the principle
def berechne_rotationsmatrix(a, b):
    """
    Berechnet die Rotationsmatrix, die den Vektor a auf den Vektor b dreht.

    Args:
        a (np.ndarray): Der 3D-Ausgangsvektor.
        b (np.ndarray): Der 3D-Zielvektor.

    Returns:
        np.ndarray: Die 3x3-Rotationsmatrix.
    """
    # Vektoren normalisieren, um sicherzustellen, dass sie Einheitsvektoren sind
    a = a / np.linalg.norm(a)
    b = b / np.linalg.norm(b)

    # Kreuzprodukt zur Bestimmung der Rotationsachse
    k = np.cross(a, b)
    # Skalarprodukt zur Bestimmung des Cosinus des Winkels
    c = np.dot(a, b)

    # Sonderfall: Vektoren sind identisch -> keine Rotation
    if np.allclose(a, b):
        return np.identity(3)

    # Sonderfall: Vektoren sind genau entgegengesetzt -> 180° Drehung
    if np.allclose(c, -1):
        # Einen beliebigen senkrechten Vektor als Rotationsachse finden
        # Wir nehmen das Kreuzprodukt mit einem nicht-parallelen Vektor
        temp_vec = np.array([1, 0, 0])
        if np.allclose(np.abs(a), temp_vec):
            temp_vec = np.array([0, 1, 0])
        k = np.cross(a, temp_vec)
        k /= np.linalg.norm(k)
        # 180°-Rotationsmatrix mit der Rodrigues-Formel (s=0, c=-1)
        return 2 * np.outer(k, k) - np.identity(3)

    # Die schiefsymmetrische Kreuzprodukt-Matrix K erstellen
    K = np.array([
        [0, -k[2], k[1]],
        [k[2], 0, -k[0]],
        [-k[1], k[0], 0]
    ])

    # Rotationsmatrix mit der Rodrigues-Formel berechnen
    # R = I + K + K^2 * (1 - c) / (s^2)
    # Wobei s^2 = ||k||^2 = 1 - c^2
    s_squared = 1 - c ** 2
    R = np.identity(3) + K + np.dot(K, K) * (1 - c) / s_squared

    return R