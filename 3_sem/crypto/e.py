def rev(x, y):
    a = x ** (y - 2)
    a %= y
    print('rev ', x, y, ' = ', a)
    return a

x = 0
x += 3 * 3 * 5 * rev(15, 7)
x += 2 * 7 * 5 * rev(35, 3)
x += 1 * 3 * 7 * rev(21, 5)
x %= 7 * 3 * 5
print(x)
print(x % 7)
print(x % 3)
print(x % 5)
