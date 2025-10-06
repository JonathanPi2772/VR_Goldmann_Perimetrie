# main.py

import sys
import time

import OpenGL.GL as gl
import OpenGL.GLU as glu
import OpenGL.GLUT as glut

from Algorithmus_Darstellung2 import get_laengengrad_line
from gl_implementation.focus_star import FocusStar
# Import the refactored classes
from gl_implementation.settings import Settings
from gl_implementation.starry_sky import StarrySky
from gl_implementation.terrain import Terrain
from gl_implementation.meteoroid import Meteoroid


class PerimetryScene:
    """
    The main application class. It initializes the environment,
    manages scene objects, and handles user interaction and rendering.
    """

    def __init__(self):
        # Load all settings
        self.settings = Settings()

        # Initialize scene objects
        self.terrain = Terrain(self.settings)
        self.starry_sky = StarrySky(self.settings)
        self.focus_star = FocusStar(self.settings)
        self.meteoroid = Meteoroid(self.settings)
        self.focus_star.set_meteoroid(self.meteoroid)

        # Camera and mouse state
        self.camera_pitch = 0.0
        self.camera_yaw = 0.0
        self.last_camera_pitch = 0.0
        self.last_camera_yaw = 0.0
        self.mouse_x = 0
        self.mouse_y = 0
        self.mouse_pressed = False
        # For start window
        self.waiting_for_start = True
        self.started = False
        self.ended = False

        # keys
        self.last_key_pressed = time.time()

    def _init_opengl(self):
        """Sets up the initial OpenGL state."""
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glEnable(gl.GL_LIGHTING)
        gl.glEnable(gl.GL_LIGHT0)
        gl.glEnable(gl.GL_COLOR_MATERIAL)
        gl.glShadeModel(gl.GL_SMOOTH)
        gl.glClearColor(0.0, 0.0, 0.0, 1.0)

        # Set up a light source
        light_pos = [50.0, 200.0, 100.0, 1.0]
        gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, light_pos)
        gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT, [0.12, 0.12, 0.12, 1.0])
        gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE, [0.9, 0.9, 0.9, 1.0])
        gl.glLightfv(gl.GL_LIGHT0, gl.GL_SPECULAR, [0.2, 0.2, 0.2, 1.0])

        # Enable point smoothing for better-looking stars (on some drivers)
        gl.glEnable(gl.GL_POINT_SMOOTH)
        gl.glHint(gl.GL_POINT_SMOOTH_HINT, gl.GL_NICEST)

    # --- GLUT Callbacks ---

    def display(self):
        """The main rendering callback."""
        # Check if the camera has moved
        if (self.camera_yaw != self.last_camera_yaw or
                self.camera_pitch != self.last_camera_pitch):
            self.focus_star.notify_camera_moved(self.camera_pitch, self.camera_yaw)
            self.last_camera_yaw = self.camera_yaw
            self.last_camera_pitch = self.camera_pitch

        # Update animations
        self.focus_star.update()

        # --- Rendering ---
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        if self.waiting_for_start:
            # Nur Text anzeigen
            self.draw_text("Möchten Sie starten? -- y/n", 0.3, 0.5)
            glut.glutSwapBuffers()
            return

        if not self.started:
            # Falls Benutzer 'n' gedrückt hat -> nichts tun oder Programm beenden
            self.draw_text("Programm abgebrochen.", 0.35, 0.5)
            glut.glutSwapBuffers()
            return

        # Apply camera transformations by rotating the world
        gl.glRotatef(self.camera_pitch, 1.0, 0.0, 0.0)
        gl.glRotatef(self.camera_yaw, 0.0, 1.0, 0.0)

        # Draw scene objects
        self.starry_sky.draw()
        self.terrain.draw()
        self.focus_star.draw(self.camera_pitch, self.camera_yaw)
        running = self.meteoroid.draw()
        if running is False:
            pass
        if self.settings.show_meteoroid_way:
            self.meteoroid.draw_meteoroid_line()

        glut.glutSwapBuffers()

    def reshape(self, width, height):
        """Callback for window resize events."""
        if height == 0: height = 1
        gl.glViewport(0, 0, width, height)
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluPerspective(60.0, float(width) / float(height), 0.1, 500.0)
        gl.glMatrixMode(gl.GL_MODELVIEW)

    def mouse_motion(self, x, y):
        """Callback for mouse movement."""
        if self.mouse_pressed:
            dx = x - self.mouse_x
            dy = y - self.mouse_y
            self.camera_yaw += dx * 0.5
            self.camera_pitch += dy * 0.5
            # Clamp pitch to avoid flipping
            self.camera_pitch = max(-90, min(90, self.camera_pitch))

        self.mouse_x = x
        self.mouse_y = y

    def mouse_button(self, button, state, x, y):
        """Callback for mouse button events."""
        if button == glut.GLUT_LEFT_BUTTON:
            self.mouse_pressed = (state == glut.GLUT_DOWN)
            self.mouse_x = x
            self.mouse_y = y

    def keyboard(self, key, x, y):
        if time.time() - self.last_key_pressed < 1:
            return
        self.last_key_pressed = time.time()
        """Callback for keyboard events."""
        if key == b'\x1b':  # ESC key
            sys.exit(0)

        if self.waiting_for_start:
            if key == b'y':
                self.waiting_for_start = False
                self.started = True
                self.meteoroid.start_animation()
                self.last_key_pressed = time.time()
            elif key == b'n':
                self.waiting_for_start = False
                self.started = False
                self.last_key_pressed = time.time()
            return

            # Wenn gestartet: Leertaste fängt Funktion ab
        if self.started and key == b' ':
            # print("Leertaste gedrückt -> Punkt erkannt")
            self.meteoroid.point_detected()
            self.last_key_pressed = time.time()
        elif self.started and key == b'p':
            if self.meteoroid.perimetry_status == "running":
                self.meteoroid.pause_animation()
                print(f"Animation/Perimetry paused")
            else:
                print("Perimetry can not be paused in the Moment")
        elif self.started and key == b'r':
            if self.meteoroid.perimetry_status == "paused":
                self.meteoroid.resume_animation()
                print(f"Animation/Perimetry resumed")
            else:
                print("Perimetry can not be resumed in the Moment")

    def run(self):
        """Initializes GLUT and starts the main loop."""
        glut.glutInit()
        glut.glutInitDisplayMode(glut.GLUT_DOUBLE | glut.GLUT_RGB | glut.GLUT_DEPTH)
        glut.glutInitWindowSize(1200, 800)
        glut.glutInitWindowPosition(100, 100)
        glut.glutCreateWindow(b"Virtual Perimetry Environment with Focus Star")

        self._init_opengl()

        # Register callbacks
        glut.glutDisplayFunc(self.display)
        glut.glutReshapeFunc(self.reshape)
        glut.glutMotionFunc(self.mouse_motion)
        glut.glutMouseFunc(self.mouse_button)
        glut.glutKeyboardFunc(self.keyboard)
        glut.glutIdleFunc(self.display)  # Redraw continuously for animations

        print("Virtual Environment Started!")
        print("- Drag with the left mouse button to rotate the camera.")
        print("- Press ESC to exit.")

        glut.glutMainLoop()

    def draw_text(self, text, x, y):
        """Hilfsfunktion: Zeichnet Text im Fensterkoordinatensystem."""
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glPushMatrix()
        gl.glLoadIdentity()
        glu.gluOrtho2D(0, 1, 0, 1)  # Normalisierte 2D-Koordinaten
        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glPushMatrix()
        gl.glLoadIdentity()

        gl.glDisable(gl.GL_LIGHTING)
        gl.glColor3f(1, 1, 1)  # Weiß
        gl.glRasterPos2f(x, y)
        for ch in text:
            glut.glutBitmapCharacter(glut.GLUT_BITMAP_HELVETICA_18, ord(ch))
        gl.glEnable(gl.GL_LIGHTING)

        gl.glPopMatrix()
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glPopMatrix()
        gl.glMatrixMode(gl.GL_MODELVIEW)


if __name__ == "__main__":
    scene = PerimetryScene()
    scene.run()