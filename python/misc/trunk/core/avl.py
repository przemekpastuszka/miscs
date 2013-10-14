"""Module with AVL tree class"""
from node import Node, Iterator


class AVL:
    """Implementation of AVL tree with some additional features"""
    ################################################################
    def __init__(self):
        self.root = None

    ################################################################
    def insert(self, key, value):
        """Insert (key, value) pair to the tree"""
        if self.root:
            self.root = self.root.insert(key, value)
        else:
            self.root = Node(key, value, None)

    ################################################################
    def get_iterator_by_key(self, key):
        """Returns iterator representing first node with given key

        Returns empty iterator when key's not found
        """
        result = self.get_iterator_by_closest_match(key)
        return result if result.get_key() == key else Iterator(None)

    def get_iterator_by_position(self, pos):
        """Returns iterator representing pos-th key in whole tree

        If pos >= tree size or pos <= 0 then returns empty iterator"""
        if pos <= 0 or pos > self.count() or not self.root:
            return Iterator(None)
        return self.root.get_by_position(pos)

    def get_iterator_by_closest_match(self, key):
        """Returns iterator or closest match if key's not found

        'Closest' means the next element
        Returns empty iterator if there's no next element"""
        if self.root:
            return self.root.get_by_key(key)
        return Iterator(None)

    ################################################################
    def get_value_by_key(self, key):
        """Returns value at a given key

        If tree doesn't contain key it returns None"""
        return self.get_iterator_by_key(key).get_value()

    def get_value_by_position(self, pos):
        """Returns value of pos-th key in the tree

        If pos >= tree size or pos <= 0 then returns None"""
        return self.get_iterator_by_position(pos).get_value()

    def get_value_by_closest_match(self, key):
        """Returns value or closest match if key's not found

        'Closest' means the next element
        Returns None if there's no next element"""
        return self.get_iterator_by_closest_match(key).get_value()

    ################################################################
    def modify_by_key(self, key, new_value):
        """Modifies value at given key

        Does nothing if key's not present in the tree"""
        self.get_iterator_by_key(key).modify(new_value)

    ################################################################
    def count(self):
        """Size of the tree"""
        if self.root:
            return self.root.size
        return 0

    def get_closest_element_position(self, key):
        """Returns element position if key's present in the tree

        Otherwise it returns position of the element with next key
        None if there's no element with next key"""
        element = self.get_iterator_by_closest_match(key).current_node
        if not element:
            return None
        result = 1 + element.left.size if element.left else 1
        # all elements v in right subtree R have positions: size of left
        # subtree + 1 + position of v in R.
        # positions of elements from left subtree L are equal to
        # positions in L
        # we go up from node to root to calculate position using
        # above equations
        while element.parent:
            if element.parent.right == element:  # it's in right subtree
                # add left subtree size + 1
                result += element.parent.size - element.size
            element = element.parent
        return result

    def count_subset(self, begin, end):
        """Returns number of elements with key in [begin, end)"""
        left = self.get_closest_element_position(begin)
        # begin is larger than all keys in the tree - return 0
        if not left:
            return 0
        right = self.get_closest_element_position(end)
        return right - left if right else self.count() - left + 1

    ################################################################
    def remove_by_key(self, key):
        """Removes node with a given key

        If key's not found it does nothing"""
        self.remove_by_iterator(self.get_iterator_by_key(key))

    def remove_by_iterator(self, iterator):
        """Removes node which is represented by iterator"""
        if not iterator.current_node:
            return None
        node = iterator.current_node
        # first case: node has no more than one child
        # so we can easily remove it
        if not node.left or not node.right:
            child = node.left if node.left else None
            child = node.right if node.right else child
            # if child exists it'll be used to substitute node
            if child:
                child.parent = node.parent
            if node.parent and node.parent.left == node:
                node.parent.left = child
            if node.parent and node.parent.right == node:
                node.parent.right = child
            # we need to rebalance tree after deletion
            parent = node.parent
            while parent:
                self.root = parent.balance()
                parent = parent.parent
            # special case when removing root
            if node == self.root:
                self.root = child
        # second case: node has two children
        # action: find tmp, which is next node in subtree rooted in
        # node (it is the leftmost node in right subtree)
        # swap tmp with node and remove tmp
        else:
            tmp = node.right
            while tmp.left:
                tmp = tmp.left
            node.key = tmp.key
            node.value = tmp.value
            self.remove_by_iterator(Iterator(tmp))

    def clear(self):
        """Removes all nodes from the tree"""
        self.root = None

    def is_empty(self):
        """False if tree contains at least one node. Otherwise True"""
        return False if self.root else True

    ################################################################
    def __iter__(self):
        return self.get_iterator_by_position(1)
