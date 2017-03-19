import csv

def read():
    with open('pkw2000.csv') as csvfile:
        reader = csv.reader(csvfile, delimiter = ',')
        a = [x for x in reader]
        columns = a[0]
        rows = a[1:]
        print(columns)
        for row in rows[:10]:
            print(row)

read()

