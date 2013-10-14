def pierwsza(n):
    return [y for y in range(2, n + 1) if [x for x in range(2, y) if  y % x == 0] == []]        
    
for i in pierwsza(20):
    print i
    
def rozklad(n):
    return [x for x in pierwsza(n + 1) if n % x == 0 for n in range(1, n) if x**n % ]
