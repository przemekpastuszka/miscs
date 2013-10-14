# Przemyslaw Pastuszka
# Systemy operacyjne 2011/2012

import stat
from ppfsstat import PpFsStat


class Metadata:
    def __init__(self, nr_of_blocks):
        self.root = Node("/", True, None)
        self.free_blocks = set(range(0, nr_of_blocks))

    def get_element(self, path):
        path_list = path.rstrip('/').split('/')[1:]
        current = self.root
        for child in path_list:
            current = current.elements[child]
        return current

    def exists(self, path):
        try:
            self.get_element(path)
            return True
        except:
            return False

    def add_element(self, path, is_dir, mode):
        (parent, name) = self.__get_parent_and_name(path)
        node = self.make_node(parent, name, is_dir)
        node.attributes.change_mode(mode)

    def __get_parent_and_name(self, path):
        (parent_path, _, name) = path.rstrip("/").rpartition("/")
        return (self.get_element(parent_path), name)

    def make_node(self, parent, name, is_dir):
        if parent.is_dir():
            parent.elements[name] = Node(name, is_dir, parent)
        return parent.elements[name]

    def remove(self, node):
        node.parent.elements.pop(node.name)
        self.__recover_blocks(node)

    def __recover_blocks(self, node):
        self.free_blocks |= set(node.blocks)
        for child in node.elements.itervalues():
            self.__recover_blocks(child)

    def reserve_block_for(self, node):
        reserved_block = self.free_blocks.pop()
        node.blocks.append(reserved_block)
        return reserved_block

    def move(self, old_path, new_path):
        node = self.get_element(old_path)
        (parent, name) = self.__get_parent_and_name(new_path)
        self.move_node(node, parent, name)

    def move_node(self, node, new_parent, new_name):
        node.parent.elements.pop(node.name)
        new_parent.elements[new_name] = node
        node.name = new_name
        node.parent = new_parent


class Node:
    def __init__(self, name, is_dir, parent):
        self.name = name
        self.attributes = PpFsStat(is_dir)
        self.blocks = []
        self.elements = dict()
        self.parent = parent

    def is_dir(self):
        return stat.S_ISDIR(self.attributes.st_mode)
