def pareto(q1, q2): # функція знаходження множини Парето
    N = len(q1)
    i = 0
    j = 1
    P = [False for x in range(N)] # массив False розміру N

    while i < N - 1: # перевіряємо виконання нерівності
        while j < N:
            if ((q1[i] > q1[j]) and (q2[i] >= q2[j]) or (q1[i] >= q1[j]) and (q2[i] > q2[j])): # Перевірка умови Парето
                if not P[j]: # якщо ще не вибрали  - вибираемо
                    P[j] = f'A{i+1}'
            elif ((q1[i] <= q1[j]) and (q2[i] < q2[j]) or (q1[i] < q1[j]) and (q2[i] <= q2[j])):
                if not P[i]:
                    P[i] = f'A{j+1}'
            j += 1
        i += 1
        j = i + 1
    return P

def slater(q1, q2): # функція знаходження множини Слейтера
    N = len(q1)
    i = 0
    j = 1
    S = [False for x in range(N)] # массив False розміру N

    while i < N - 1:
        while j < N:
            if ((q1[i] > q1[j]) and (q2[i] > q2[j])): # Перевірка умови Слейтера
                if not S[j]:
                    S[j] = f'A{i+1}'
            elif ((q1[i] < q1[j]) and (q2[i] < q2[j])):
                if not S[i]:
                    S[i] = f'A{j+1}'
            j += 1
        i += 1
        j = i + 1
    return S

def get_optimal_values(lst): # Отримання оптимальних значень з обробленого масиву
    optimal_values = []
    pos = 1
    for i in lst:
        if not i:
            optimal_values.append(f'A{pos}')
        pos += 1

    return optimal_values

def get_vectors(data): # Ділимо масив з даними на два підмасиви за правилами
    q1 = []
    q2 = []

    for index in data:
        if len(str(index)) == 1:
            q1.append(0)
            q2.append(index)
            continue
        q1.append(index // 10)
        q2.append(index % 10)

    return q1, q2

# data = [79,20,41,93,24,28,35,8,62,42,70,96,63,66,11,51,15,87,34,32]
# data = [90,50,93,33,39,80,94,93,13,54,82,44,75,23,38,51,51,73,11,65]
# data = [68,81,13,83,99,77,35,14,64,69,46,7,72,91,40,11,23,87,57,36]

q1, q2 = get_vectors(data)

P = pareto(q1, q2)
S = slater(q1, q2)
print(f'Q1: {q1}')
print(f'Q2: {q2}')
print(f'Pareto: {P}')
print(f'Pareto optimal values: {get_optimal_values(P)}')
print(f'Slater: {S}')
print(f'Slater optimal values: {get_optimal_values(S)}')