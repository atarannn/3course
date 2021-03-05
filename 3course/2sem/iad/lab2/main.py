import pandas # программная библиотека, для обработки и анализа данных
import math # библиотека, для выполнения математических операций

def check_word(word): # функция для проверки слова
    signs = [',', '!', '/', '?', '.', '(', ')'] # символы
    word = word.lower()
    if word:
        for sign in signs:
            if sign in word:
                word = word.replace(sign, '')
    return word

def get_class_frequency(df):
    a = df.groupby('Class').count() # для группировки по классам
    freq_dict = {} # словарь кол-ва повторений в словаре
    sum = 0

    for index in a.index: # для кол-ва потворений слов в словаре
        freq_dict[index] = a.loc[index].Message
        sum += freq_dict[index]

    for key in freq_dict.keys(): # для кол-ва повторений классов
        freq_dict[key] = freq_dict[key] / sum

    return freq_dict


def dict_words_num(df, freq_dict): # создаем словарь
    dict = {} # наш словарь со словами та кол-во их повторений за классами

    for index in range(len(df.index)):
        message = df.iloc[index].Message.split(' ')

        # проверка слова на наличие в словаре
        for word in message:
            word = check_word(word) # проверка слова
            if word not in dict:  # если слов нет в словаре
                dict[word] = {'Internet': 0, 'Science': 0, 'Space': 0}

            # если есть -> записываем и +1
            if df.iloc[index].Class == 'Internet':
                dict[word]['Internet'] = dict[word]['Internet'] + 1
            elif df.iloc[index].Class == 'Science':
                dict[word]['Science'] = dict[word]['Science'] + 1
            elif df.iloc[index].Class == 'Space':
                dict[word]['Space'] = dict[word]['Space'] + 1

    # для нормализации вероятности в словаре
    for value in dict.values():  # (p_before + p_class_freq) / N + 1
        N = 0
        for cl in value.values():
            N += cl
        for i in value:
            value[i] = (value[i] + freq_dict[i]) / (N + 1)

    return dict


def classifier(dict, message, freq): # функция для классификации
    possible_classes = {'Internet': 0, 'Science': 0, 'Space': 0} # существующие классы
    new_message = [] # сообщение
    message = message.split(' ') # разделитель

    for word in message: # слова в сообщении
        word = check_word(word)  # проводим функцию проверки слова
        new_message.append(word) # добавляем новое слово

    for word in new_message: # слова в новом сообщении
        for i in possible_classes.keys():
            if word in dict.keys(): # если слово уже было
                possible_classes[i] += math.log2(dict[word][i])
            else:  # unknown word
                possible_classes[i] += math.log2(0.5)

    for i in freq.keys():
        possible_classes[i] += math.log2(freq[i])

    result = ([x[0] for x in filter(lambda x: possible_classes[x[0]] == max(possible_classes.values()),
                                    possible_classes.items())])[0]
    return result


def test(test, dict, freq): # тест
    correct = 0
    for index in range(len(test.index)):
        if classifier(dict, test.Message[index], freq) == test.Class[index]:
            correct += 1
            print(f'Correct: {test.Message[index]} - {test.Class[index]}')
        else:
            print(f'Incorrect: {test.Message[index]} - {test.Class[index]}')

    return (correct / len(test.index)) * 100


df = pandas.read_csv('./data_train.csv')
print(df) # выводим данные с учебного файла
freq = get_class_frequency(df)
dict = dict_words_num(df, freq)

test1 = pandas.read_csv('./data_test.csv')
print(f'Correct predictions = {test(test1, dict, freq)}%') # выводим процент коректноси результатов

