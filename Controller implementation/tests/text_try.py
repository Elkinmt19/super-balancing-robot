first_text = open('text1.txt','w')

for _ in range(5): 
    first_text.write('YA SE COMO ES LA VUELTA\n')

first_text.close()

def try_(proof):
    mm = proof
    print(mm)
    mm = not mm
    print(mm)


try_(True)