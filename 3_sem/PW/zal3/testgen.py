mx = 150
print('2 1 ', mx, sep = '')
print('1 x[0] = ', end ='')
for i in range(1, mx):
    print('x[', i, '] * ', sep = '', end = '')
print('x[', mx, ']', sep = '')
print('2 ', end = '')
for i in range(1, mx + 1):
    print('x[', i, '] ', i, ' ', sep = '', end = '')
print()

