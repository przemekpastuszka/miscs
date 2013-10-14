#2
def sum(x, y, z):
    return x + y + z
	
print sum(2, 3, 4)

#4
def dot(vec1, vec2):
    result = 0
    for i in range(len(vec1)):
        result += vec1[i] * vec2[i]
    return result
	
print dot([1,2,3], [3,4,5])

#6
def toNumber(ls):
    values = {"jeden" : 1, "dwa" : 2, "trzy" : 3, "cztery" : 4, "piec" : 5,
        "szesc" : 6, "siedem" : 7, "osiem" : 8, "dziewiec" : 9, "zero" : 0}
    result = 0
    for num in ls:
        result = result * 10 + values[num]
    return result
    
print toNumber(["jeden", "siedem"])
