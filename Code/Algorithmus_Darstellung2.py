import math
import numpy as np
from numpy import cos as cos
from numpy import sin as sin
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.animation import FuncAnimation

from gl_implementation.helper_functions import calc_rotation_matrix


# Funktionen für Berechnung
def theta_from_regler(theta_regler):
    return theta_regler * np.pi / 2 + (np.pi / 2)


def get_coordinates(laengengrad, theta_regler):
    theta = theta_from_regler(theta_regler)
    phi = np.deg2rad(laengengrad)
    x = sin(theta) * cos(phi)
    y = sin(theta) * sin(phi)
    z = cos(theta)
    return x, y, z


def transform_coordinates(a_vec, b_vec, c_vec):
    R = calc_rotation_matrix(a_vec, b_vec)
    return R @ c_vec


def get_laengengrad_line(laengengrad):
    theta = np.linspace(np.pi / 2, np.pi, 100)
    phi = np.deg2rad(laengengrad)
    x = sin(theta) * cos(phi)
    y = sin(theta) * sin(phi)
    z = cos(theta)
    return x, y, z


if __name__ == "__main__":
    # Plot in neuem Fenster
    plt.ion()

    # Gut im Plot bewegen
    mpl.rcParams["axes3d.mouserotationstyle"] = "azel"

    # Blickpunkt/Fokuspunkt
    focus_point = [0, 1, 0]

    # Punkte für Perimeterkugel
    factor_phi = np.pi / 2
    sphere_phi = np.linspace(-1 * np.pi / 2 + factor_phi, np.pi / 2 + factor_phi, 100)
    sphere_theta = np.linspace(0, np.pi, 100)
    sphere_phi, sphere_theta = np.meshgrid(sphere_phi, sphere_theta)
    sphere_x = sin(sphere_theta) * cos(sphere_phi)
    sphere_y = sin(sphere_theta) * sin(sphere_phi)
    sphere_z = cos(sphere_theta)

    fig = plt.figure(figsize=(10, 10))
    ax = plt.axes(projection='3d')

    # Kugel darstellen
    surf = ax.plot_surface(sphere_x, sphere_y, sphere_z, color='red', alpha=0.15)
    # Fokuspunkt darstellen
    ax.scatter(focus_point[0], focus_point[1], focus_point[2], color='blue', s=50, label='Fokuspunkt')
    # Koordinatenachsen darstellen
    ax.plot([-1.5, 1.5], [0, 0], [0, 0], 'k-', lw=1)
    ax.plot([0, 0], [-1.5, 1.5], [0, 0], 'k-', lw=1)
    ax.plot([0, 0], [0, 0], [-1.5, 1.5], 'k-', lw=1)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Einheitskugel mit Punkten und Funktionen')
    ax.legend()
    ax.set_box_aspect([1, 1, 1])
    ax.grid(True)
    ax.view_init(elev=30, azim=60)

    # Animationsregeln
    degree_index = np.linspace(0, 360, 25)  # 15° Schritte für Abtastung (360/15) +1
    number_of_frames_per_laegengrad = 1000
    total_frames = len(degree_index) * number_of_frames_per_laegengrad
    actual_laegengrad = 0
    (point,) = ax.plot([], [], [], 'o', color="green", markersize=10)  # der wandernde Punkt
    (line,) = ax.plot([], [], [], '-', color='red', label='Bahn des Punktes')


    def init():
        point.set_data([], [])
        point.set_3d_properties([])
        return point,


    # Update-Funktion für jeden Frame
    def update(frame):
        # aktuellen Winkel und Frame bestimmen
        angle_idx = frame // number_of_frames_per_laegengrad
        theta_regler = (frame % number_of_frames_per_laegengrad) / number_of_frames_per_laegengrad

        degree = degree_index[angle_idx]
        x_line, y_line, z_line = get_laengengrad_line(degree)
        line.set_data(x_line, y_line)
        line.set_3d_properties(z_line)

        x_point, y_point, z_point = get_coordinates(degree, theta_regler)
        a_vec = np.array([0, 0, -1], dtype=float)
        b_vec = np.array([0, 1, -0], dtype=float)
        c_vec = np.array([x_point, y_point, z_point], dtype=float)
        x_point, y_point, z_point = transform_coordinates(a_vec, b_vec, c_vec)
        point.set_data([x_point], [y_point])
        point.set_3d_properties([z_point])
        if frame % 10 == 0:
            r = np.sqrt(x_point ** 2 + y_point ** 2 + z_point ** 2)
            theta = math.acos(z_point / r) if r != 0 else 0
            theta = -((theta / math.pi * 180) - 90)
            phi = np.arctan2(y_point, x_point)
            phi = 90 - (phi / math.pi * 180)
            print(f"\r phi={phi} theta={theta} r={r}", end="")

        # optional: Legende
        if frame % number_of_frames_per_laegengrad == 0:
            line.set_label(f"Winkel {degree:.1f}°")
            ax.legend(loc="upper right")

        return point, line


    # Animation starten
    input("Animation starten?")

    ani = FuncAnimation(fig, update, frames=range(total_frames),
                        init_func=init, blit=True, interval=5, repeat=False)

    plt.show()
    input("Beenden ? -- ")
