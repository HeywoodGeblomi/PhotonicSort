#!/usr/bin/env python3
"""Unit tests for PhotonicSort — correctness + adaptive routing."""
from __future__ import annotations

import random
import unittest

from photonic_sort import (
    photonic_sort,
    photonic_probe,
    photonic_collapse,
    negative_time_early_exit,
    compute_ranks,
)


class TestPhotonicSort(unittest.TestCase):
    def assert_sorted_eq(self, data, reverse=False, key=None):
        out = photonic_sort(data, reverse=reverse, key=key)
        expected = sorted(data, reverse=reverse, key=key)
        self.assertEqual(out, expected)
        self.assertEqual(len(out), len(data))

    def test_empty_and_singleton(self):
        self.assertEqual(photonic_sort([]), [])
        self.assertEqual(photonic_sort([42]), [42])

    def test_already_sorted(self):
        data = list(range(200))
        self.assert_sorted_eq(data)
        probe = photonic_probe(data)
        self.assertTrue(probe["is_negative_delay"])

    def test_reverse_sorted(self):
        data = list(range(200, 0, -1))
        self.assert_sorted_eq(data)
        probe = photonic_probe(data)
        self.assertTrue(probe["is_negative_delay"])

    def test_random(self):
        rng = random.Random(0)
        data = [rng.randint(0, 10_000) for _ in range(500)]
        self.assert_sorted_eq(data)

    def test_duplicates(self):
        data = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5]
        self.assert_sorted_eq(data)
        # stability: equal keys keep relative order of first occurrence ranks via stable collapse
        out = photonic_sort(data)
        self.assertEqual(out, sorted(data))

    def test_organpipe(self):
        data = list(range(100)) + list(range(100, 0, -1))
        self.assert_sorted_eq(data)
        probe = photonic_probe(data)
        self.assertTrue(probe["is_negative_delay"] or probe["sortedness"] > 0.3)

    def test_sawtooth(self):
        data = [i % 17 for i in range(300)]
        self.assert_sorted_eq(data)

    def test_force_collapse(self):
        data = [7, 2, 9, 1, 5, 3, 8, 4, 6, 0]
        out = photonic_sort(data, force_collapse=True)
        self.assertEqual(out, list(range(10)))

    def test_key_and_reverse(self):
        data = ["bb", "a", "ccc", "dddd"]
        self.assert_sorted_eq(data, key=len)
        self.assert_sorted_eq(data, reverse=True)
        self.assert_sorted_eq(data, key=len, reverse=True)

    def test_ranks_cover(self):
        data = [40, 10, 30, 20]
        ranks = compute_ranks(data)
        self.assertEqual(sorted(ranks), list(range(4)))
        collapsed = photonic_collapse(data)
        self.assertEqual(collapsed, [10, 20, 30, 40])

    def test_probe_metrics(self):
        probe = photonic_probe(list(range(50)))
        self.assertEqual(probe["n"], 50)
        self.assertGreaterEqual(probe["sortedness"], 0.7)
        self.assertIn("group_delay_proxy", probe)
        self.assertIn("inv_ratio", probe)


if __name__ == "__main__":
    unittest.main()
