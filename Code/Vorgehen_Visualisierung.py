import math
import numpy as np
from numpy import cos as cos
from numpy import sin as sin
import matplotlib.pyplot as plt
import matplotlib as mpl

# Gut im Plot bewegen
mpl.rcParams["axes3d.mouserotationstyle"] = "azel"

plt.ion()

punkt_a = [0, 0, -1]
punkt_b = [0, 1, 0]


# Punkte für Einheitskugel
factor_phi = np.pi/2
sphere_phi = np.linspace(-1 * np.pi/2+factor_phi, np.pi/2+factor_phi, 100)
sphere_theta = np.linspace(0, np.pi, 100)
sphere_phi, sphere_theta = np.meshgrid(sphere_phi, sphere_theta)
sphere_x = sin(sphere_theta) * cos(sphere_phi)
sphere_y = sin(sphere_theta) * sin(sphere_phi)
sphere_z = cos(sphere_theta)

plt.figure(figsize=(10, 10))
ax = plt.axes(projection='3d')

surf = ax.plot_surface(sphere_x, sphere_y, sphere_z, color='red', alpha=0.15)

ax.scatter(punkt_a[0], punkt_a[1], punkt_a[2], color='blue', s=100, label='Ursprünglicher Schnittpunkt, (0, 0, -1)')
ax.scatter(punkt_b[0], punkt_b[1], punkt_b[2], color='blue', s=100, label='Transformierter Schnittpunkt, Fokuspunkt (0, 1, 0)')

ax.plot([-1.5, 1.5], [0, 0], [0, 0], 'k-', lw=1)
ax.plot([0, 0], [-1.5, 1.5], [0, 0], 'k-', lw=1)
ax.plot([0, 0], [0, 0], [-1.5, 1.5], 'k-', lw=1)

count = 2
all_phi = [0, 15] # np.linspace(0, 360, count)
theta = np.linspace(np.pi/2, np.pi, 100)
all_phi = np.deg2rad(all_phi)  # np.full_like(theta, fill_value=np.deg2rad(phi))
for i, phi in enumerate(all_phi):
    phi, theta = np.meshgrid(phi, theta)

    x_utrans = sin(theta) * cos(phi)
    y_utrans = sin(theta) * sin(phi)
    z_utrans = cos(theta)


    x_trans = sin(theta) * cos(phi)
    y_trans = -1 * cos(theta)
    z_trans = sin(theta) * sin(phi)
    deg = (180 / count) * i
    ax.plot(
        x_utrans, y_utrans, z_utrans, '-', color='green',
        # label=f'Untransformierte Bahn {deg}°'
    )
    ax.plot(
        x_trans, y_trans, z_trans, '-', color='red',
        # label=f'Transformierte Bahn {deg}°'
    )

ax.plot([], [], '-', color='red', label='Transformierte Bahn')
ax.plot([], [], '-', color='green', label='Untransformierte Bahn')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Einheitskugel mit Punkten und Funktionen')

ax.set_xlim([-1.5, 1.5])
ax.set_ylim([-1.5, 1.5])
ax.set_zlim([-1.5, 1.5])

ax.legend()
ax.set_box_aspect([1,1,1])
ax.grid(True)
ax.view_init(elev=30, azim=60)


plt.show()

input("Beenden ? -- ")


"""
Zusammenfassung, 
Transformation funktioniert wie gewünscht. 
Phi kann ganz normal verwendet werden. 0° entpricht auch im Goldmann Bogen 0 Grad (Außenkreis)
Das heißt für die Perimetrie wird nun einfach in 15° Schritten phi gewählt und anschließend theta verändert.
Dabei startet Theta bei pi/2 (Äußerste Punkt)und endet bei pi (Mittelpunkt)
Wir können so einen Theta-Regler bestimmen, wobei 0 den Außenpunkt angibt 1 den Mittelpunkt.
Normale min-max-normierung: 
x_skaliert = (x - x_min) / (x_max - x_min)
Eingabe = (theta(Ausgabe) - (pi/2)) / (pi - (pi/2))
Eingabe = (theta(Ausgabe) - (pi/2)) / (pi/2)
Eingabe * pi/2 = theta(Ausgabe) - (pi/2)
Eingabe * pi/2 + (pi/2) = theta(Ausgabe)

der theta Regler wird dann also erhöht, bis der Patient drückt. Der dazugehörige kartesische Punkt, für Phi und Theta wird
anschließend  in ein normales Kugelkoordinatensystem umgewandelt (Fokuspunkt ist bei phi=0 und theta=0). 
Dafür muss von dem errechneten noch pi/2 abgezogen werden und anschließend in einen Winkel übersetzt werden.
"""
