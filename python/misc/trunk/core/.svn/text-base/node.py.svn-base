"""Contains classes: Node (of avl) and Iterator (iterates over Node)"""


class Node:
    """Represents one node of AVL tree"""
    def __init__(self, key, value, parent):
        self.key = key
        self.value = value
        self.parent = parent
        self.left = None  # children
        self.right = None
        self.size = 1  # nr of nodes in subtree rooted at self
        self.height = 1  # length of the longest path to leaf

    def get_by_key(self, key):
        """Returns iterator representing node with key

        Returns iterator to node with next key if key's not found
        Returns empty iterator only if there's no next element"""
        if self.key == key:
            return Iterator(self)
        if key < self.key:
            return self.left.get_by_key(key) if self.left else Iterator(self)
        else:
            if self.right:
                return self.right.get_by_key(key)
            else:
                return Iterator(self.next())

    def get_by_position(self, position):
        """Returns iterator to element at given position

        Function assumes that 1 <= position <= tree size"""
        left_count = self.left.size if self.left else 0
        if left_count + 1 == position:
            return Iterator(self)
        if position <= left_count:  # node is in the left subtree
            return self.left.get_by_position(position)
        return self.right.get_by_position(position - left_count - 1)

    def insert(self, key, value):
        """Inserts to subtree rooted at self. Involves rotations"""
        if key < self.key:
            if self.left:
                self.left.insert(key, value)
            else:
                self.left = Node(key, value, self)
        else:
            if self.right:
                self.right.insert(key, value)
            else:
                self.right = Node(key, value, self)
        # we need to rebalance (if needed) after each insertion
        return self.balance()  # returns current root of the subtree

    def balance(self):
        """Performs rotations to keep subtree rooted at self balanced"""
        # right tree is higher than left (Right - Right/Left case)
        if(self.calculate_balance() == -2):
            if self.right and self.right.calculate_balance() > 0:
                self.right.rotate_right()  # Right - Left case
            return self.rotate_left()  # return new root
        # Left - Left/Right case
        if(self.calculate_balance() == 2):
            if self.left and self.left.calculate_balance() < 0:
                self.left.rotate_left()  # Left - Right case
            return self.rotate_right()  # return new root
        self.update_data()
        return self  # root hasn't changed

    def calculate_balance(self):
        """Returns balance factor

        height of left subtree - height of right subtree"""
        left_height = self.left.height if self.left else 0
        right_height = self.right.height if self.right else 0
        return left_height - right_height

    def update_data(self):
        """Updates height and size of the node"""
        left_height = left_size = right_size = right_height = 0
        if self.left:
            left_height = self.left.height
            left_size = self.left.size
        if self.right:
            right_height = self.right.height
            right_size = self.right.size
        self.size = left_size + right_size + 1
        self.height = 1 + max(left_height, right_height)

    def rotate_right(self):
        """Right rotation. Left child of self becomes new root"""
        new_root = self.left
        # right child of new root must be now left child of self
        self.left = new_root.right
        new_root.right = self
        if self.left:
            self.left.parent = self
        return self.finish_rotation(new_root)  # returns new_root

    def rotate_left(self):
        """Left rotation. Right child of self becomes new root"""
        new_root = self.right
        self.right = new_root.left
        new_root.left = self
        if self.right:
            self.right.parent = self
        return self.finish_rotation(new_root)

    def finish_rotation(self, new_root):
        """Performs common operations for both rotations"""
        new_root.parent = self.parent
        self.parent = new_root
        self.update_data()  # we need to calculate new size and height
        new_root.update_data()  # same thing
        # we need to update parent of the subtree
        if new_root.parent and new_root.parent.left == self:
            new_root.parent.left = new_root
        if new_root.parent and new_root.parent.right == self:
            new_root.parent.right = new_root
        return new_root

    def next(self):
        """Returns node next to self. None if there's no next node"""
        # first case: right child exists
        # the next node is the leftmost node in right subtree
        if self.right:
            tmp = self.right
            while tmp.left:
                tmp = tmp.left
            return tmp
        # second case: right child doesn't exist
        # next node is one of the self's ancestors - first on the path
        # to root, which has self in its left subtree
        tmp = self
        while tmp.parent and tmp == tmp.parent.right:
            tmp = tmp.parent
        return tmp.parent


class Iterator:
    """Is used to iterate over nodes of avl tree

    We say that iterator's empty when current_node == None"""
    def __init__(self, node):
        self.current_node = node
        self.previous_node = None

    def __iter__(self):
        return self

    def __next__(self):
        """Returns (key, value) of the next node or raises exception"""
        if self.current_node:
            self.previous_node = self.current_node
            self.current_node = self.current_node.next()
            return (self.previous_node.key, self.previous_node.value)
        raise StopIteration  # we are outside the tree

    def get_key(self):
        """Returns key of node represented by iterator.

        None if iterator's empty"""
        return self.current_node.key if self.current_node else None

    def get_value(self):
        """Returns value of node represented by iterator.

        None if iterator's empty"""
        return self.current_node.value if self.current_node else None

    def modify(self, new_value):
        """Modify value of node represented by iterator

        Does nothing if iterator's empty"""
        if self.current_node:
            self.current_node.value = new_value
