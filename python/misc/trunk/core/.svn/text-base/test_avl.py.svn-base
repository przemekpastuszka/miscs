"""Tests for AVL tree"""
import unittest
import random
from avl import AVL


class AVLTester(unittest.TestCase):
    """First set of tests for avl"""
    def setUp(self):
        self.avl = AVL()
        # insert random permutation of [1, ..., 1000) list to avl tree
        ls = list(range(1, 1000))
        random.shuffle(ls)
        for v, k in enumerate(ls):
            self.avl.insert(k, v + 1)

    def test_is_all(self):
        """This function tests if all values are present in test tree"""
        test_set = set(range(1, 1000))
        # checks whether get_value_by_key works fine
        all_values = {self.avl.get_value_by_key(x) for x in range(1, 1000)}
        self.assertSetEqual(all_values, test_set, "get_value_by_key fails")
        # checks whether get_value_by_position works fine
        all_values = {self.avl.get_value_by_position(x) for x in range(1, 1000)}
        self.assertSetEqual(all_values, test_set, "get_value_by_position fails")
        # checks whether iteration over tree works fine
        all_values = {value for key, value in self.avl}
        self.assertSetEqual(all_values, test_set, "tree iterator fails")

    def test_all_values_removal(self):
        """Checks if tree's empty after removing all values"""
        for k in range(1, 1000):
            self.avl.remove_by_key(k)
        self.assertTrue(self.avl.is_empty())

    def test_count(self):
        """Check's whether counting tree elements works fine"""
        self.assertEqual(self.avl.count(), 999)
        self.assertEqual(self.avl.count_subset(24, 124), 100)
        self.assertEqual(self.avl.count_subset(-10, 10), 9)
        self.assertEqual(self.avl.count_subset(999, 1001), 1)
        self.assertEqual(self.avl.count_subset(997, 1001), 3)
        self.assertEqual(self.avl.count_subset(1001, 1002), 0)

    def test_special_cases(self):
        """'Edge' cases"""
        # we want value of key not present in tree
        self.assertIsNone(self.avl.get_value_by_key(-7))
        self.assertIsNone(self.avl.get_value_by_key(6666))
        self.assertIsNone(self.avl.get_value_by_position(6666))
        self.assertIsNone(self.avl.get_value_by_position(-21))
        self.assertIsNone(self.avl.get_value_by_closest_match(1001))
        self.assertEqual(self.avl.get_value_by_closest_match(-1), self.avl.get_value_by_key(1))
        self.assertIsNone(self.avl.get_value_by_closest_match(1001))
        self.assertIsNone(self.avl.get_closest_element_position(10001))
        # we try to remove nodes with key not present in tree
        self.avl.remove_by_key(-23)
        self.avl.remove_by_key(1001)
        self.avl.remove_by_iterator(self.avl.get_iterator_by_key(10001))
        self.assertEqual(self.avl.count(), 999)


class AVLTester2(unittest.TestCase):
    """Second set of tests for avl"""
    def setUp(self):
        self.avl = AVL()
        self.avl.insert(277, 19)  # some random values choosen by me
        self.avl.insert(31, 14)  # order of inserted elements
        self.avl.insert(91, 99)  # forces rotation of nodes
        self.avl.insert(99, 22)
        self.avl.insert(111, 78)
        self.avl.insert(121, 91)

    def test_modifications(self):
        """Checks whether modification is ok"""
        self.avl.modify_by_key(111, 19)
        self.avl.modify_by_key(200, 21)
        self.assertEqual(self.avl.get_value_by_key(111), 19)

    def test_removal(self):
        """Checks whether removal works fine"""
        self.avl.remove_by_key(31)
        self.avl.remove_by_key(99)
        self.assertListEqual([v for k, v in self.avl], [99, 78, 91, 19])
        self.assertIsNone(self.avl.get_value_by_key(31))
        self.assertEqual(self.avl.get_value_by_closest_match(31), 99)

    def test_some_things(self):
        """Test some random things"""
        self.assertListEqual([78, 91, 19], [value for key, value in self.avl.get_iterator_by_key(111)])
        self.assertListEqual([78, 91, 19], [value for key, value in self.avl.get_iterator_by_position(4)])
        self.assertEqual(self.avl.get_value_by_closest_match(91), 99)
        self.assertEqual(self.avl.get_value_by_closest_match(71), 99)
        self.assertEqual(self.avl.get_value_by_closest_match(95), 22)
        self.assertEqual(self.avl.get_closest_element_position(21), 1)
        self.assertEqual(self.avl.get_closest_element_position(98), 3)
        self.avl.clear()
        self.assertIsNone(self.avl.get_value_by_closest_match(111))

unittest.main()
