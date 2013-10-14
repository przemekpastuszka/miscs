# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

from device import Device
from metadata import Metadata
from device_wrapper import DeviceWrapper
import fuse
import errno
import pickle
import sys


# metachanger pickles metadata and saves it to device after invocation of f
def metachanger(f):
    def wrapper(self, *args):
        result = f(self, *args)
        self.pickle_metadata()
        return result if result is not None else 0
    return wrapper


class PpFs(fuse.Fuse):
    def __init__(self, *args, **kw):
        fuse.Fuse.__init__(self, *args, **kw)
        self.blocks_nr = 120
        self.block_size = 8 * 1024
        self.K = 5
        self.device = Device("data", self.block_size, self.blocks_nr + self.K)
        self.wrapper = DeviceWrapper(self.device)
        # metadata is held in last self.K blocks of device
        self.meta_blocks = list(range(self.blocks_nr, self.blocks_nr + self.K))
        self.meta = self.__get_meta()

    def __get_meta(self):
        try:
            data_size = self.K * self.block_size
            data = self.wrapper.read(0, data_size, self.meta_blocks)
            return pickle.loads(data)
        except:
            return Metadata(self.blocks_nr)

    def pickle_metadata(self):
        pickled = pickle.dumps(self.meta)
        self.wrapper.write(0, pickled, self.meta_blocks)

    def getattr(self, path):
        if not self.meta.exists(path):
            return -errno.ENOENT
        return self.meta.get_element(path).attributes

    def readdir(self, path, offset):
        node = self.meta.get_element(path)
        for d in node.elements.iterkeys():
            yield fuse.Direntry(d)

    def mknod(self, path, mode, dev):
        return self.create(path, None, mode)

    @metachanger
    def create(self, path, flags, mode):
        self.meta.add_element(path, False, mode)

    @metachanger
    def mkdir(self, path, mode):
        self.meta.add_element(path, True, mode)

    @metachanger
    def rmdir(self, path):
        return self.unlink(path)

    @metachanger
    def unlink(self, path):
        self.meta.remove(self.meta.get_element(path))

    def read(self, path, size, offset):
        blocks = self.meta.get_element(path).blocks
        return self.wrapper.read(offset, size, blocks)

    @metachanger
    def write(self, path, buff, offset):
        node = self.meta.get_element(path)
        buff_end = offset + len(buff)
        # reserve enough blocks for file to hold data
        while len(node.blocks) * self.block_size < buff_end:
            self.meta.reserve_block_for(node)
        self.wrapper.write(offset, buff, node.blocks)
        node.attributes.st_size = max([node.attributes.st_size, buff_end])
        return len(buff)

    def release(self, path, flags):
        return self.flush(path, None)

    def fsync(self, path, isfsyncfile):
        return self.flush(path, None)

    def flush(self, path, fh=None):
        self.device.flush()
        return 0

    def open(self, path, flags):
        return 0

    @metachanger
    def rename(self, pathfrom, pathto):
        self.meta.move(pathfrom, pathto)

    @metachanger
    def chmod(self, path, mode):
        node = self.meta.get_element(path)
        node.attributes.change_mode(mode)

    @metachanger
    def chown(self, path, uid, gid):
        node = self.meta.get_element(path)
        if uid >= 0:
            node.attributes.st_uid = uid
        if gid >= 0:
            node.attributes.st_gid = gid

    def truncate(self, path, size):
        return 0

    def utime(self, path, times):
        return 0


if __name__ == '__main__':
    fuse.fuse_python_api = (0, 2)
    server = PpFs()
    server.parse(errex=1)
    server.main()
