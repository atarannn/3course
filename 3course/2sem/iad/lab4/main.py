import pandas
import math
import statistics
import matplotlib.pyplot as plt
import os

NUM_OF_CLUSTERS = 3 # кількість кластерів

def save(name='', fmt='png'): # функція для збереження графічного зображення кластеризації
    pwd = os.getcwd()
    path = './Downloads'.format(fmt)
    if not os.path.exists(path):
        os.mkdir(path)
    os.chdir(path)
    plt.savefig('{}.{}'.format(name, fmt), format='png')
    os.chdir(pwd)


def check_word(word): # функція для перевірки слів
    signs = [',', '!', '/', '?', '.', '(', ')'] # символи
    word = word.lower()
    if word:
        for sign in signs:
            if sign in word: # якщо в слові є символи
                word = word.replace(sign, '') # замінюємо всі символи (ігноруємо їх)
    return word

def dict_words_num(df): # словник слів та кількість їх повторень
    dict = {} # словник
    for index in range(len(df.index)):
        message = df.iloc[index].Message.split(' ') # отримуємо список слів
        sentence = df.iloc[index].Message
        for word in message:
            if word:
                if word.lower() not in dict: # якщо слово не має в словнику -> записуємо результат
                        dict[word.lower()] = [{'x': sentence.index(word), 'y': len(word)}]
                else: # якщо слово є в словнику, заносимо результат в кінець списку
                        dict[word.lower()].append({'x': sentence.index(word), 'y': len(word)})
    result = {x[0]: x[1] for x in dict.items() if len(x[1]) > 1} # в словник заносимо слова, які зустрічаються більше 1 разу

    for x in result.items(): # середні значення атрибутів для кожного слова
        # описуємо атрибути
        attr_x = []
        attr_y = []
        for z in result[x[0]]:
            attr_x.append(z['x'])
            attr_y.append(z['y'])
        attr_x = statistics.mean(attr_x) # рахуємо середне значення атрибуту x
        attr_y = statistics.mean(attr_y) # рахуємо середне значення атрибуту y
        result[x[0]] = {'x': attr_x, 'y': attr_y} # виводимо результат середніх значень атрибутів
    print(result)
    return result

def measure_distance(A, B): # функція для розрахунку відстані між даними у просторі
    result = math.sqrt((B['x'] - A['x'])**2 + (B['y'] - A['y'])**2)
    return result

def find_coord_minimum(distance_array): # розрахунок найменшої відстані
    minimum = distance_array[1][0] # мінімальна відстань
    coord = []
    row = 0
    for i in distance_array:
        col = 0
        for j in i:
            if j > 0 and j < minimum:
                minimum = j
                coord = [row, col]
            col += 1
        row += 1

    if not coord:
        coord = [1, 0]
    return coord

def cluster(dict):
    while len(dict) > NUM_OF_CLUSTERS:
        sequence = [x for x in dict.keys()]
        distance_array = []
        index = 0

        for key in sequence:
            distance = [] # побудова таблиці відстаней слів
            for i in range(index):
                distance.append(measure_distance(dict[sequence[i]], dict[sequence[index]]))
            distance.append(0)
            distance_array.append(distance)
            index += 1

        coord = find_coord_minimum(distance_array) # знаходимо найменшу відстань

        # розрахунок центроїди
        centr_x = (dict[sequence[coord[0]]]['x'] + dict[sequence[coord[1]]]['x']) / 2
        centr_y = (dict[sequence[coord[0]]]['y'] + dict[sequence[coord[1]]]['y']) / 2

        # об'єднання слів
        dict[f'{sequence[coord[0]]}+{sequence[coord[1]]}'] = {'x': centr_x, 'y': centr_y}
        del dict[sequence[coord[0]]]
        del dict[sequence[coord[1]]]

        print(dict)
        return cluster(dict)
    return dict

def draw_graphic(graphic_data, clusters): # функція для побудови графічного зображення кластеризації
    colors_list = []
    X = []
    Y = []
    N = 50
    iterator = iter(clusters.keys())
    for i in range(NUM_OF_CLUSTERS):
        temp = next(iterator)
        color = N / (i + 1)
        for key in graphic_data.keys():
            if key in temp:
                colors_list.append(color)
                X.append(graphic_data[key]['x'])
                Y.append(graphic_data[key]['y'])

    plt.scatter(X, Y, c=colors_list)
    save('pic', 'png')
    plt.show()

df = pandas.read_csv('./iad4.csv')
# df = pandas.read_csv('./train_d.csv')

dict = dict_words_num(df)
graphic_data = dict.copy()
clusters = cluster(dict)
draw_graphic(graphic_data, clusters)