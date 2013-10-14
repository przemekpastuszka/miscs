# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

import mmap
import os


class Device:
    def __init__(self, path, block_size, data_blocks):
        self.path = path
        self.block_size = block_size
        self.data_blocks = data_blocks
        self.file_size = self.block_size * self.data_blocks
        self.__open_file()
        self.__map_file()
        self.closed = False

    def __map_file(self):
        nr = self.f.fileno()
        self.blocks = mmap.mmap(nr, self.file_size, access=mmap.ACCESS_WRITE)

    def __open_file(self):
        if not os.path.exists(self.path):
            with open(self.path, "wb") as f:
                f.seek(self.file_size - 1)
                f.write(b'0')
        self.f = open(self.path, "r+b")

    def read_block(self, block_id):
        self.blocks.seek(block_id * self.block_size)
        return self.blocks.read(self.block_size)

    def write_block(self, block_id, data):
        if len(data) > self.block_size:
            raise DataSizeBiggerThanBlock
        self.blocks.seek(block_id * self.block_size)
        self.blocks.write(data)

    def flush(self):
        self.blocks.flush()

    def close(self):
        if not self.closed:
            self.blocks.close()
            self.f.close()
            self.closed = True

    def __del__(self):
        self.close()


class DataSizeBiggerThanBlock(Exception):
    pass
