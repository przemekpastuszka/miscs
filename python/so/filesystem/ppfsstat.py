# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

import fuse
import stat
from time import time
import os


class PpFsStat(fuse.Stat):
    def __init__(self, is_dir):
        self.st_mode = stat.S_IFDIR if is_dir else stat.S_IFREG
        self.st_mode |= 0755
        self.st_ino = 0
        self.st_dev = 0
        self.st_nlink = 1
        self.st_uid = os.getuid()
        self.st_gid = os.getgid()
        self.st_size = 4096 if is_dir else 0
        self.update_time()

    def update_time(self):
        now = time()
        self.st_atime = now
        self.st_mtime = now
        self.st_ctime = now

    def change_mode(self, mode):
        is_dir = stat.S_ISDIR(self.st_mode)
        self.st_mode = stat.S_IFDIR if is_dir else stat.S_IFREG
        self.st_mode |= mode
