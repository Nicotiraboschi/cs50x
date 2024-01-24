class Jar:
    def __init__(self, capacity = 12, size = 5):
        self._capacity = capacity
        if capacity < 0 or capacity != int(capacity):
            raise ValueError("capacity must be positive integer")
        self._size = size

    def __str__(self):
        return "🍪" * self._size

    def deposit(self, n):
        if self._size + n > self._capacity:
            raise ValueError("capacity not enough")
        else:
            self._size += n

    def withdraw(self, n):
        if self._size - n < 0:
            raise ValueError("eaten them all!")
        else:
            self._size -= n

    @property
    def capacity(self):
        return str(self._capacity)


    @property
    def size(self):
        return str(self._size)


def main():
    jar = Jar()
    jar.deposit(12)
    jar.withdraw(2)
    print(str(jar))

main()