import csv

def read():
    with open('pkw2000.csv') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        a = [list(map(lambda s : s.title(), x)) for x in reader]
        columns = a[0]
        rows = a[1:]
        return columns, rows

if __name__ == "__main__":
    cols, rows = read()
    i = 0
    for c in cols:
        print(i, c)
        i += 1

    mx = 0
    for r in rows:
        mx = max(mx, int(r[5]))
    print(mx)
