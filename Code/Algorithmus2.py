import numpy as np
import matplotlib.pyplot as plt
from gl_implementation.helper_functions import calc_rotation_matrix


def visualisiere_rotation(A, B, C):
    """
    Visualisiert die Rotation von Vektor A nach B und wendet sie auf C an.

    Args:
        A (list or np.ndarray): Koordinaten des Punktes A.
        B (list or np.ndarray): Koordinaten des Punktes B.
        C (list or np.ndarray): Koordinaten des Punktes C, der mitrotiert wird.
    """
    plt.ion()
    plt.rcParams["axes3d.mouserotationstyle"] = "azel"
    # Vektoren in NumPy-Arrays umwandeln
    a_vec = np.array(A, dtype=float)
    b_vec = np.array(B, dtype=float)
    c_vec = np.array(C, dtype=float)

    # Sicherstellen, dass A und B den gleichen Radius haben (für eine gültige Rotation)
    radius_a = np.linalg.norm(a_vec)
    b_vec = b_vec / np.linalg.norm(b_vec) * radius_a

    # Rotationsmatrix berechnen
    R = calc_rotation_matrix(a_vec, b_vec)

    # Rotation auf Vektor c anwenden
    c_prime_vec = R @ c_vec

    # --- Plot erstellen ---
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.set_title("Rotation im 3D-Raum")
    ax.set_box_aspect([1, 1, 1])

    # Vektoren als Pfeile vom Ursprung zeichnen
    origin = [0, 0, 0]
    ax.quiver(origin, origin, origin, a_vec, a_vec, a_vec, color='blue', label='A (Original)', arrow_length_ratio=0.1)
    ax.quiver(origin, origin, origin, b_vec, b_vec, b_vec, color='red', label='B (Ziel)', arrow_length_ratio=0.1)
    ax.quiver(origin, origin, origin, c_vec, c_vec, c_vec, color='green', label='C (Original)', linestyle='--',
              arrow_length_ratio=0.1)
    ax.quiver(origin, origin, origin, c_prime_vec, c_prime_vec, c_prime_vec, color='purple', label="C' (Rotiert)",
              linestyle='--', arrow_length_ratio=0.1)

    # Endpunkte der Vektoren als Punkte markieren
    ax.scatter(*a_vec, color='blue', s=50)
    ax.scatter(*b_vec, color='red', s=50)
    ax.scatter(*c_vec, color='green', s=50)
    ax.scatter(*c_prime_vec, color='purple', s=50)

    # Visualisierung einer Kugel, auf der A und B liegen
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    x = radius_a * np.outer(np.cos(u), np.sin(v))
    y = radius_a * np.outer(np.sin(u), np.sin(v))
    z = radius_a * np.outer(np.ones(np.size(u)), np.cos(v))
    ax.plot_surface(x, y, z, color='gray', alpha=0.1)

    # Achsen beschriften und Grenzen setzen
    max_val = np.max(np.abs([A, B, C, c_prime_vec])) * 1.2
    ax.set_xlim([-max_val, max_val])
    ax.set_ylim([-max_val, max_val])
    ax.set_zlim([-max_val, max_val])
    ax.set_xlabel('X-Achse')
    ax.set_ylabel('Y-Achse')
    ax.set_zlabel('Z-Achse')
    ax.legend()
    ax.grid(True)

    # Ansicht für bessere Darstellung anpassen
    ax.view_init(elev=20, azim=30)

    plt.show()


# --- Beispielaufruf ---
if __name__ == '__main__':
    # Definieren Sie hier Ihre Punkte A, B und C
    # A und B sollten denselben Abstand zum Ursprung haben.
    punkt_A = [5, 0, 0]
    punkt_B = [0, 5, 0]
    punkt_C = [5, 2, 2]  # Ein Punkt, der "in der Nähe" von A ist

    # Visualisierungsfunktion aufrufen
    visualisiere_rotation(punkt_A, punkt_B, punkt_C)
    input("Beenden ? -- ")
