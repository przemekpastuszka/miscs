# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

from math import ceil


class DeviceWrapper:
    def __init__(self, device):
        self.device = device
        self.b_size = device.block_size

    def read(self, offset, length, blocks):
        result = self.__read_with_borders(offset, length, blocks)
        over = offset % self.b_size
        return result[over:over + length]

    # it'll read all data from blocks, that intersects with given range
    # [offset:offset + length]
    def __read_with_borders(self, offset, length, blocks):
        needed_blocks = self.__get_needed_blocks(offset, length, blocks)
        return ''.join(map(self.device.read_block, needed_blocks))

    # get blocks that intersects with [offset:offset + length]
    def __get_needed_blocks(self, offset, length, blocks):
        first_block = offset / self.b_size
        last_block = first_block + length / self.b_size
        if length % self.b_size != 0:
            last_block += 1
        return blocks[first_block:last_block]

    def write(self, offset, buff, blocks):
        length = len(buff)
        data = bytearray(self.__read_with_borders(offset, length, blocks))
        over = offset % self.b_size
        data[over:over + length] = buff
        data = str(data)

        first_block = offset / self.b_size
        for i, p in enumerate(range(0, len(data), self.b_size)):
            block_id = blocks[i + first_block]
            self.device.write_block(block_id, data[p:p + self.b_size])
