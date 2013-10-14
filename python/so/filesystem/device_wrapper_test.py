# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

import unittest
from random import randrange
from device import Device
from device_wrapper import DeviceWrapper


class DeviceWrapperTest(unittest.TestCase):
    def setUp(self):
        self.dev = DummyDevice()
        self.clear_device()

        self.wrapper = DeviceWrapper(self.dev)

    def clear_device(self):
        self.dev.write_block(0, b'ab')
        self.dev.write_block(1, b'cd')
        self.dev.write_block(2, b'ef')
        self.dev.write_block(3, b'gh')
        self.dev.write_block(4, b'ij')

    def test_read(self):
        self.assertEqual(self.wrapper.read(0, 8, [0, 1, 2, 3]), b'abcdefgh')
        self.assertEqual(self.wrapper.read(0, 3, [1, 3]), b'cdg')
        self.assertEqual(self.wrapper.read(3, 3, [0, 1, 2, 3]), b'def')

    def test_write(self):
        self.wrapper.write(3, b'xyz', [0, 1, 3])
        self.assertEqual(self.dev.read_block(0), b'ab')
        self.assertEqual(self.dev.read_block(1), b'cx')
        self.assertEqual(self.dev.read_block(2), b'ef')
        self.assertEqual(self.dev.read_block(3), b'yz')
        self.assertEqual(self.dev.read_block(4), b'ij')
        self.clear_device()


class DummyDevice:
    def __init__(self):
        self.block_size = 2
        self.data = dict()

    def read_block(self, i):
        return self.data[i]

    def write_block(self, i, d):
        self.data[i] = d

unittest.main()
