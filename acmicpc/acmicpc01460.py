class Node:
    def __init__(self, value):
        self.data = value
        self.next = None

class DLinked:
    def __init__(self):
        self.head = None

    def read(self, index):
        current_node = self.head
        current_index = 0

        while current_index < index:
            current_node = current_node.next
            current_index += 1

        return current_node.data

    def insert(self, index, value):
        new_node = Node(value)

        if self.head:
            if index == 0:
                new_node.next = self.head
                self.head = new_node
            else:
                current_node = self.head
                current_index = 0

                while current_index < index - 1:
                    current_node = current_node.next
                    current_index += 1
                new_node.next = current_node.next
                current_node.next = new_node
        else:
            self.head = new_node

linked = DLinked()
linked.insert(0, 10)
linked.insert(0, 5)
linked.insert(1, 3)
linked.insert(3, 7)
print(linked.read(0))
print(linked.read(1))
print(linked.read(2))
print(linked.read(3))
