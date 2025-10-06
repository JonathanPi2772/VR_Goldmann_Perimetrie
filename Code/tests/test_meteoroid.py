import unittest
from gl_implementation.goldmann_sizes import *
import os


class TestMeteoroid(unittest.TestCase):
    def test_sizes(self):
        distance = 0.3
        size_i = Size_I()
        size_i.set_distance(distance)
        size_ii = Size_II()
        size_ii.set_distance(distance)
        size_iii = Size_III()
        size_iii.set_distance(distance)
        size_iv = Size_IV()
        size_iv.set_distance(distance)
        size_v = Size_V()
        size_v.set_distance(distance)
        # Assert the sizes
        # --- V ---
        first = size_v.get_size_meter2()
        second = 64 * (10 ** -6)
        self.assertAlmostEqual(first, second, places=6,
                               msg=f"First= {first}, second = {second}, delta = {abs(first - second)}")
        # --- IV ---
        first = size_iv.get_size_meter2()
        second = 16 * (10 ** -6)
        self.assertAlmostEqual(first, second, places=6,
                               msg=f"First= {first}, second = {second}, delta = {abs(first - second)}")
        # --- III ---
        first = size_iii.get_size_meter2()
        second = 4 * (10 ** -6)
        self.assertAlmostEqual(first, second, places=6,
                               msg=f"First= {first}, second = {second}, delta = {abs(first - second)}")
        # --- II ---
        first = size_ii.get_size_meter2()
        second = 1 * (10 ** -6)
        self.assertAlmostEqual(first, second, places=6,
                               msg=f"First= {first}, second = {second}, delta = {abs(first - second)}")
        # --- I ---
        first = size_i.get_size_meter2()
        second = 0.25 * (10 ** -6)
        self.assertAlmostEqual(first, second, places=6,
                               msg=f"First= {first}, second = {second}, delta = {abs(first - second)}")
