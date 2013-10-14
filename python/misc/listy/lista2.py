#1

def znajdzPolaczenie(rozklad, stacjaA, stacjaB):
    polaczenie = znajdzPolaczenie2(rozklad, stacjaA, stacjaB, -1, [])
    polaczenie.reverse()
    if polaczenie == []:
        print "Nie jest mozliwe odnalezienie polaczenia miedzy podanymi stacjami"
    else:
        for i in polaczenie:
            print i
    return polaczenie

def znajdzPolaczenie2(rozklad, stacjaA, stacjaB, czas, sprawdzone):
    if stacjaA in sprawdzone:
        return []
    if stacjaA == stacjaB:
        return [stacjaB]
    sprawdzone.append(stacjaA)
    
    for dokad, odjazd, przyjazd in rozklad[stacjaA]:
        if odjazd < czas:
            continue
        polaczenie = znajdzPolaczenie2(rozklad, dokad, stacjaB, przyjazd, sprawdzone)
        if polaczenie != []:
            polaczenie.append(stacjaA)
            return polaczenie
            
    return []
    
rozklad = {"A" : [("B", 10, 20)], "B" : [("A", 21, 30), ("C", 21, 24)]}
znajdzPolaczenie(rozklad, "A", "C")
