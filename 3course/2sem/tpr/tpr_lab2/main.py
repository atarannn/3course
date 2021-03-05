from math import ceil

class TPR:
    def __init__(self, data_, c):
        self.data = data_
        self.c = c

    def nfa(self):  # Next Fit
        containers_num = 1
        volume = self.c
        compares = 0
        for i in self.data:
            compares += 1
            if i > volume:  # не хватает места - новый контейнер
                containers_num += 1
                volume = 100
            volume -= i

        print(f'Compares = {compares}')
        return containers_num

    def ffa(self):  # First Fit
        containers_list = []
        compares = 0

        for i in self.data:
            if containers_list:
                compares += 1
                if i > self.c - containers_list[-1]:  # не хватает места в последнем
                    pos = 0
                    for cont in containers_list:  # пробуем уже созданные
                        compares += 1
                        if i <= self.c - cont:
                            containers_list[pos] += i
                            break
                        pos += 1
                    else:
                        containers_list.append(i)  # если не влез - создаем новый
                else:
                    containers_list[-1] += i
            else:
                containers_list.append(i)

        print(f'Compares = {compares}')
        return len(containers_list)

    def wfa(self):  # Worst Fit - макс. свободного места
        containers_list = []
        compares = 0

        for i in self.data:
            if containers_list:
                compares += 1
                if i > self.c - containers_list[-1]:  # не хватает места в последнем
                    pos = 0
                    containers_list.sort()
                    for cont in containers_list: # пробуем минимально заполненный
                        compares += 1
                        if i <= self.c - cont:
                            containers_list[pos] += i
                            break
                        pos += 1
                    else:
                        containers_list.append(i)  # если не влез - создаем новый
                else:
                    containers_list[-1] += i
            else:
                containers_list.append(i)

        print(f'Compares = {compares}')
        return len(containers_list)

    def bfa(self):  # Best Fit
        containers_list = []
        compares = 0

        for i in self.data:
            if containers_list:
                compares += 1
                if i > self.c - containers_list[-1]:  # не хватает места в последнем
                    pos = 0
                    containers_list.sort(reverse=True)
                    for cont in containers_list:  # пробуем максимально зполненный, в котором хватает места
                        compares += 1
                        if i <= self.c - cont:
                            containers_list[pos] += i
                            break
                        pos += 1
                    else:
                        containers_list.append(i)  # если не влез - создаем новый
                else:
                    containers_list[-1] += i
            else:
                containers_list.append(i)

        print(f'Compares = {compares}')
        return len(containers_list)


#           без впорядкування
# data = [79, 20, 41, 93, 24, 28, 35, 8, 62, 42, 70, 96, 63, 66, 11, 51, 15, 87, 34, 32]
# data = [90,50,93,33,39,80,94,93,13,54,82,44,75,23,38,51,51,73,11,65]
# data = [68,81,13,83,99,77,35,14,64,69,46,7,72,91,40,11,23,87,57,36]
data = [79,20,41,93,24,28,35,8,62,42,70,96,63,66,11,51,15,87,34,32,
        90,50,93,33,39,80,94,93,13,54,82,44,75,23,38,51,51,73,11,65,
        68,81,13,83,99,77,35,14,64,69,46,7,72,91,40,11,23,87,57,36]


min_possible = lambda data: ceil(sum(data) / 100)
print(f'Min possible containers: {min_possible(data)}')

test = TPR(data, 100)

# data.sort()
data.sort(reverse = True)

print()
print('-------------- NFA --------------')
print(f'Number of containers needed = {test.nfa()}')

print()
print('-------------- FFA --------------')
print(f'Number of containers needed = {test.ffa()}')

print()
print('-------------- WFA --------------')
print(f'Number of containers needed = {test.wfa()}')
print()

print('-------------- BFA --------------')
print(f'Number of containers needed = {test.bfa()}')