# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

import unittest
from random import randrange
from device import Device


class DeviceTest(unittest.TestCase):
    def setUp(self):
        self.block_size = 1
        self.blocks = 100
        self.dev = Device("data", self.block_size, self.blocks)
        self.data = [randrange(0, 256) for i in range(0, self.blocks)]
        for i, block in enumerate(self.data):
            self.dev.write_block(i, chr(block))

    def test_memory_read(self):
        self.should_preserve_data(self.dev)

    def test_file_read(self):
        self.dev.close()
        dev = Device("data", self.block_size, self.blocks)
        self.should_preserve_data(dev)

    def should_preserve_data(self, dev):
        for i, block in enumerate(self.data):
            read = dev.read_block(i)
            self.assertEqual(read, chr(block))

    @unittest.expectedFailure
    def test_fail_to_write_data_bigger_than_block(self):
        self.dev.write_block(0, b'aa')

    @unittest.expectedFailure
    def test_fail_with_out_of_bounds(self):
        self.dev.write_block(self.blocks + 1, b'a')

    def tearDown(self):
        self.dev.close()

unittest.main()
