# settings.py
import numpy as np

from gl_implementation.goldmann_sizes import Size_I, Size_II, Size_III, Size_IV, Size_V


class Settings:
    """
    A centralized class to hold all configurable parameters for the scene.
    This makes it easy to tweak the appearance and behavior of the environment.
    """
    def __init__(self):
        # --- Mountain Parameters ---
        self.peak_amplitude = 20.0        # Peak amplitude of the Gaussian curve (in meters)
        self.sigma = 7.0                  # Standard deviation of the Gaussian curve (in meters)
        self.summit_y = -1.70             # Y-coordinate of the mountain's summit (1.7m below the camera at y=0)
        self.mountain_extent = 20.0       # Half-width of the mountain in the x/z plane (in meters)
        self.mountain_resolution = 150    # Resolution of the mesh (vertices per axis)
        self.mountain_color = (0.06, 0.25, 0.06)  # Dark green

        # --- Starry Sky Parameters ---
        self.sky_radius = 250.0           # Radius of the hollow sphere for the sky
        self.num_stars = 200              # Total number of stars to generate
        self.star_distance = 249.0        # Distance of the stars from the origin
        self.star_size_variation = 1.6    # Factor for randomizing star sizes
        self.star_position_noise = 1.8    # Factor for randomizing star positions
        self.sky_color = (0.015, 0.015, 0.02)   # Near-black for the sky
        self.star_color = (1.0, 1.0, 1.0)       # White for the stars

        # --- Focus Star Parameters ---
        self.focus_star_size = 0.75             # Base size of the focus star
        self.focus_star_pulse_freq = 0.5        # Pulse frequency in Hz
        self.focus_star_pulse_magnitude = 0.3   # Pulse magnitude (+- percent)
        self.focus_star_distance = 25           # Distance from the camera
        self.focus_star_wait_time = 1.0         # Seconds to wait after camera stops before moving
        self.focus_star_move_duration = 2.0     # Seconds for the star to travel to the new position

        # --- Meteoroid Parameters ---
        self.meteoroid_distance = 50
        self.meteoroid_color = (1.0, 0.0, 0.0)
        self.meteoroid_way_color = (1.0, 0.0, 0.0)
        """self.meteoroid_longitudes_deg = [
            0, 15, 30, 45, 60, 75, 90,
            105, 120, 135, 150, 165, 180,
            195, 210, 225, 240, 255, 270,
            285, 300, 315, 330, 345
        ]"""
        """self.meteoroid_longitudes_deg = [
            0, 30, 60, 90,
            120, 150, 180,
            210, 240, 270,
            300, 330,
        ]"""
        self.meteoroid_longitudes_deg = [
            45, 90,
            135, 180,
            225, 270,
            315, 0
        ]
        self.meteoroid_random = True
        self.meteoroid_sizes = {
            Size_I: False,
            Size_II: False,
            Size_III: True,
            Size_IV: True,
            Size_V: False,
        }
        self.meteoroid_speed = 10  # deg/sec
        self.general_thales_point = np.array([0, 0, -self.meteoroid_distance], dtype=float)
        self.show_meteoroid_way = True

