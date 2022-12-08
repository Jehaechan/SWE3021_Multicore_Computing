f1_name = "result1.txt"
f2_name = "result2.txt"
f1 = open(f1_name,'r')
f2 = open(f2_name,'r')
while True:
    line1 = f1.readline().strip()
    line2 = f2.readline().strip()
    if(line1 != line2):
        print("Wrong Answer!\n");
    if not line1:
        break

