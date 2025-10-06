# focus_star.py

import numpy as np
import OpenGL.GL as gl
import math
import time
from gl_implementation.meteoroid import Meteoroid


class FocusStar:
    """
    Manages the state, animation, and rendering of the focus star.
    The star waits for camera movement to stop, then smoothly moves
    to the new focal point.
    """

    def __init__(self, settings):
        """
        Initializes the FocusStar with its parameters.

        Args:
            settings: An object containing configuration parameters.
        """
        # Animation and timing
        self.wait_time = settings.focus_star_wait_time
        self.move_duration = settings.focus_star_move_duration
        self.distance = settings.focus_star_distance

        # Appearance
        self.size = settings.focus_star_size
        self.pulse_freq = settings.focus_star_pulse_freq
        self.pulse_magnitude = settings.focus_star_pulse_magnitude

        # State management
        self.position = np.array([0, 0, -self.distance])  # Initial position
        self.target_pos = self.position.copy()
        self.start_pos = self.position.copy()
        self.state = "idle"  # States: "idle", "waiting", "moving"
        self.wait_start_time = None
        self.anim_start_time = None

        # Perimetry
        self.meteoroid: None | Meteoroid = None

    def set_meteoroid(self, meteoroid: Meteoroid):
        self.meteoroid = meteoroid

    def _calculate_target_position(self, camera_pitch, camera_yaw):
        """Calculates the 3D position in front of the camera."""
        pitch_rad = math.radians(camera_pitch)
        yaw_rad = math.radians(camera_yaw)

        dx = math.cos(pitch_rad) * math.sin(yaw_rad)
        dy = -math.sin(pitch_rad)
        dz = -math.cos(pitch_rad) * math.cos(yaw_rad)

        direction = np.array([dx, dy, dz])
        return direction * self.distance

    def _slerp(self, p0, p1, t):
        """Spherical Linear Interpolation for smooth movement on a sphere."""
        # Normalize vectors to ensure they are on the unit sphere
        u = p0 / np.linalg.norm(p0)
        v = p1 / np.linalg.norm(p1)

        # Calculate the angle between the vectors
        dot = np.clip(np.dot(u, v), -1.0, 1.0)
        theta = math.acos(dot)

        # If the angle is very small, return the target vector
        if theta < 1e-5:
            return p1

        # Perform the interpolation
        s1 = math.sin((1 - t) * theta) / math.sin(theta)
        s2 = math.sin(t * theta) / math.sin(theta)

        # Return the interpolated vector, scaled to the correct distance
        return (s1 * u + s2 * v) * self.distance

    def notify_camera_moved(self, camera_pitch, camera_yaw):
        """
        Called by the main scene when the camera moves. This resets the wait timer.
        """
        self.state = "waiting"
        self.meteoroid.pause_animation()
        self.target_pos = self._calculate_target_position(camera_pitch, camera_yaw)
        self.wait_start_time = time.time()

    def update(self):
        """
        Updates the star's state and position based on time.
        This should be called every frame.
        """
        now = time.time()

        # If waiting, check if it's time to start moving
        if self.state == "waiting" and now - self.wait_start_time > self.wait_time:
            self.state = "moving"
            self.start_pos = self.position.copy()
            self.anim_start_time = now

        # If moving, update the position via interpolation
        elif self.state == "moving":
            t = (now - self.anim_start_time) / self.move_duration
            if t >= 1.0:
                self.state = "idle"
                self.position = self.target_pos.copy()
                self.meteoroid.resume_animation()
            else:
                self.position = self._slerp(self.start_pos, self.target_pos, t)
            self.meteoroid.star_position_changed(self.position)

    def draw(self, camera_pitch, camera_yaw):
        """
        Renders the focus star as a 2D billboarded shape.

        Args:
            camera_pitch: Current pitch of the camera for billboarding.
            camera_yaw: Current yaw of the camera for billboarding.
        """
        gl.glPushMatrix()
        gl.glTranslatef(*self.position)

        # --- Billboarding: Undo camera rotation to face the camera ---
        gl.glRotatef(-camera_yaw, 0.0, 1.0, 0.0)
        gl.glRotatef(-camera_pitch, 1.0, 0.0, 0.0)

        gl.glColor3f(1.0, 1.0, 0.0)  # Yellow color
        gl.glDisable(gl.GL_LIGHTING)  # Make it self-illuminated

        # --- Pulsing Animation ---
        current_time = time.time()
        pulse = 1.0 + self.pulse_magnitude * math.sin(2 * math.pi * self.pulse_freq * current_time)
        inner_radius = self.size * 0.5 * pulse
        outer_radius = self.size * pulse

        # --- Draw a simple 2D star shape ---
        num_points = 5
        gl.glBegin(gl.GL_TRIANGLE_FAN)
        gl.glVertex3f(0, 0, 0)  # Center point
        for i in range(num_points * 2 + 1):
            angle = i * math.pi / num_points
            # Alternate between outer and inner radius for the points
            r = outer_radius if i % 2 == 0 else inner_radius
            x = r * math.cos(angle)
            y = r * math.sin(angle)
            gl.glVertex3f(x, y, 0)
        gl.glEnd()

        gl.glEnable(gl.GL_LIGHTING)
        gl.glPopMatrix()
