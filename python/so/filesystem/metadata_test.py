# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

import unittest
from random import randrange
from metadata import Metadata


class MetadataTest(unittest.TestCase):
    def setUp(self):
        self.meta = Metadata(100)
        self.root = self.meta.root
        self.madedir = self.meta.make_node(self.root, "made_dir", True)
        self.f = self.meta.make_node(self.madedir, "file", False)

    def test_root(self):
        self.assertTrue(self.meta.exists("/"))
        root = self.meta.get_element("/")
        self.assertEqual(root, self.root)
        self.assertTrue(root.is_dir())
        self.assertIsNone(root.parent)
        self.assertListEqual(root.blocks, [])

    def test_not_existance(self):
        self.assertFalse(self.meta.exists("/not_existing"))

    def test_node_making(self):
        self.assertTrue(self.madedir.is_dir())
        self.assertEqual(self.root, self.madedir.parent)
        self.assertEqual(self.madedir, self.meta.get_element("/made_dir"))
        self.assertEqual(self.madedir, self.meta.get_element("/made_dir/"))

        self.assertFalse(self.f.is_dir())
        self.assertTrue(self.meta.exists("/made_dir/file"))
        self.assertEqual(self.f, self.meta.get_element("/made_dir/file"))

    def test_block_reservation(self):
        reserved = []
        for i in range(0, 7):
            k = self.meta.reserve_block_for(self.f)
            reserved.append(k)
            self.assertNotIn(k, self.meta.free_blocks)
        self.assertListEqual(reserved, self.f.blocks)
        self.assertEqual(len(set(reserved)), 7)

    def test_removal(self):
        directory = self.meta.make_node(self.root, "rm_dir", True)
        file_one = self.meta.make_node(directory, "file_one", False)
        file_two = self.meta.make_node(directory, "file_two", False)
        reserved = []
        reserved.append(self.meta.reserve_block_for(file_one))
        reserved.append(self.meta.reserve_block_for(file_one))
        reserved.append(self.meta.reserve_block_for(file_two))

        self.meta.remove(directory)
        self.assertFalse(self.meta.exists("/rm_dir/file_one"))
        self.assertFalse(self.meta.exists("/rm_dir"))
        for i in reserved:
            self.assertIn(i, self.meta.free_blocks)

    def test_move(self):
        move_dir = self.meta.make_node(self.root, "move_dir", True)
        self.meta.move_node(self.f, move_dir, "blah")
        self.assertFalse(self.meta.exists("/made_dir/file"))
        self.assertTrue(self.meta.exists("/move_dir/blah"))
        self.meta.move_node(self.f, self.madedir, "file")
        self.assertTrue(self.meta.exists("/made_dir/file"))

unittest.main()
