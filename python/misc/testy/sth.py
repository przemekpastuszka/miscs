class Matrix:    
    def __init__(self, rows):
        self._rows = rows;
        if rows:
            length = len(rows[0])
            if any(len(y) != length for y in rows):
                print "kupsko"
        
    def transpose(self):
        return Matrix(zip(*self._rows))
    
    def multiply(self, other):
        trans = other.transpose()
        result = [
                    [
                        sum(x[i] * y[i] for i in range(len(y)))
                        for y in trans._rows
                    ]
                    for x in self._rows
                ]
        return Matrix(result)

with open("testme", "r") as f:
    for line in f:
        for i in line.strip().split(' '):
            print int(i) + 1
