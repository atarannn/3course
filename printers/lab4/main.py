import math
import numpy as np
import cv2
import argparse
import glob
import os

left_codes_dict = {  # типы последовательностей, для левой группы разрядов
    "AAAAAA": 0,
    "AABABB": 1,
    "AABBAB": 2,
    "AABBBA": 3,
    "ABAABB": 4,
    "ABBAAB": 5,
    "ABBBAA": 6,
    "ABABAB": 7,
    "ABABBA": 8,
    "ABBABA": 9,
}

sign_codes_dict = { #
    'A': { # ширина линий
        "3211": 0,
        "2221": 1,
        "2122": 2,
        "1411": 3,
        "1132": 4,
        "1231": 5,
        "1114": 6,
        "1312": 7,
        "1213": 8,
        "3112": 9,
    },
    'B': {
        "1123": 0,
        "1222": 1,
        "2212": 2,
        "1141": 3,
        "2311": 4,
        "1321": 5,
        "4111": 6,
        "2131": 7,
        "3121": 8,
        "2113": 9,
    },
    'C': {
        "3211": 0, # 1110010
        "2221": 1, # 1100110
        "2122": 2, # 1101100
        "1411": 3, # 1000010
        "1132": 4, # 1011100
        "1231": 5, # 1001110
        "1114": 6, # 1010000
        "1312": 7, # 1000100
        "1213": 8, # 1001000
        "3112": 9, # 1110100
    },
}

def convert_to_width(arr):
    width_list = []
    zeros = False
    width = 0
    state = 1
    for val in arr:
        if zeros:
            if val == state:
                width += 1
            else:
                width_list.append(width)
                width = 1
        else:
            if val == 0:
                zeros = True
                width += 1
        state = val
    width_list.append(width) #что-бы добавить посл. значение

    try:
        width_x1 = np.mean(width_list[:3] + width_list[27:32] + width_list[56:59])
        print("Width: ", width_x1) # ширина одной полоски
        norm_width = map(lambda x: x / width_x1, width_list) # нормируем нашу ширину
        norm_round_width = list(map(round, norm_width))
        return norm_round_width
    except Exception as e:
        print(e)
        return []

def decode(img):
    # ret_str = None
    avg_brit = np.mean(img)
    horizontal_line = map(lambda x: 0 if x < avg_brit else 1, img[256])
    vertical_line = map(lambda x: 0 if x[256] < avg_brit else 1, img)
    width_list_horiz = convert_to_width(horizontal_line)
    width_list_vert = convert_to_width(vertical_line)
    # print("Horizontal width",width_list_horiz)
    print("Штрих-код:",width_list_vert)

    width_list = width_list_horiz if len(width_list_horiz) > len(width_list_vert) else width_list_vert
    left_group = width_list[3:27]
    right_group = width_list[32:56]
    print("Левая часть (первые 6 групп):",left_group)
    print("Правая часть (вторые 6 групп):",right_group)

    decoded_list = [] # декодирование
    left_code_pattern = ''
    val_sum = 0
    for i, val in enumerate(left_group):  # парность/непарность
        if (i + 1) % 2 == 0:
            val_sum += val
        if (i + 1) % 4 == 0:
            left_code_pattern += 'A' if val_sum % 2 != 0 else 'B'
            val_sum = 0
    print("Схема кодирования первой цифры (разряда) штрих-кода:",left_code_pattern)
    decoded_list.append(left_codes_dict[left_code_pattern])
    print("Первый разряд:",decoded_list) # левая цифра штрих-кода

    left_group_nested = [[y for y in left_group[x:x + 4]] for x in range(0, len(left_group), 4)]
    for i, code in enumerate(left_group_nested):
        pattern = left_code_pattern[i]
        decode_dict = sign_codes_dict[pattern]
        decoded_list.append(decode_dict.get(''.join(map(str, code)), -1))
    print("Значение схемы кодирования первого разряда и левой части кода:",decoded_list)

    right_group_nested = [[y for y in right_group[x:x + 4]] for x in range(0, len(right_group), 4)]
    for code in right_group_nested: # для правой части кода используется тип С
        decode_dict = sign_codes_dict['C']
        decoded_list.append(decode_dict.get(''.join(map(str, code)), -1))
    print("Правая часть", decoded_list[7:13])

    print("Значение схемы кодирования (первый разряд + первая часть + вторая):",decoded_list)
    print()
    print("     Проверка контрольной суммы")
    check_sum = 10 - (sum(decoded_list[0:-1:2]) + 3 * sum(decoded_list[1:-1:2])) % 10
    print("     Контрольная сумма:", check_sum)
    assert (check_sum == decoded_list[-1])
    return "".join(map(str, decoded_list))

def win_var(img, wlen): # дисперсия
    wmean, wsqrmean = (cv2.boxFilter(x, -1, (wlen, wlen), borderType=cv2.BORDER_REFLECT) for x in (img, img * img))
    return wsqrmean - wmean * wmean

for filename in glob.iglob('input-images/*'): # получаем пути файлов
    image = cv2.imread(filename) # читаем картинку
    (img_h, img_w, _) = image.shape # запись размера
    kernel = np.array([[-1, 1, -1],
                       [0, 3, 0],
                       [-1, 1, -1]])
    image = cv2.filter2D(image, -1, kernel) # повышаем четкость (больше для полосок)

    size_for_proc = 255 # уменьшаем изоб/
    image_res_for_proc = cv2.resize(image, (size_for_proc, size_for_proc))

    # контраст изображения
    lab = cv2.cvtColor(image_res_for_proc, cv2.COLOR_BGR2LAB)
    l, a, b = cv2.split(lab)
    clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(4, 4))
    cl = clahe.apply(l)
    limg = cv2.merge((cl, a, b))
    contrast = cv2.cvtColor(limg, cv2.COLOR_LAB2BGR).astype('uint8')

    # шум
    noise = np.random.normal(loc=10, scale=45, size=contrast.shape)
    contrast_noise = np.clip(contrast + noise * 0.05, 0, 255).astype('uint8')

    # оттенки серого (0 -255)
    gray_noise = cv2.cvtColor(contrast_noise, cv2.COLOR_BGR2GRAY)

    # градиент
    gradX = cv2.Sobel(gray_noise, ddepth=cv2.CV_32F, dx=1, dy=0, ksize=-2)
    gradY = cv2.Sobel(gray_noise, ddepth=cv2.CV_32F, dx=0, dy=1, ksize=-2)

    gradient = cv2.subtract(gradX, gradY)
    gradient = cv2.convertScaleAbs(gradient)
    gradient = cv2.normalize(gradient, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)

    # cv2.imshow("Image", gradient)
    # cv2.waitKey(0)

    # размытие
    core_size = 50
    blurred = cv2.blur(gradient, (core_size, core_size))
    blurred = cv2.normalize(blurred, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8UC1)

    # cv2.imshow("Image", blurred)
    # cv2.waitKey(0)

    variance = np.mean(win_var(image_res_for_proc, 3)) # дисперсия изобр/
    print(f"{filename}:\t{variance}")
    correction = 0.0055 * variance + 0.95
    print(correction)

    cnts = None
    for i in reversed(range(3, 7)):

        (_, thresh) = cv2.threshold(blurred, int(60 * correction) + 12 * (i + 3), 255, cv2.THRESH_BINARY)
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (12 + (7 - i), 9 + (7 - i)))
        closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)
        closed_erode = cv2.erode(closed, None, iterations=int(math.floor(2 * i - 7)))
        closed_deilate = cv2.dilate(closed_erode, None, iterations=6 + (7 - i))
        (finded, _) = cv2.findContours(closed_deilate.copy(), cv2.RETR_EXTERNAL,
                                       cv2.CHAIN_APPROX_SIMPLE)
        if finded:
            cnts = finded
            break

    if not cnts:
        raise Exception("Can`t find code")

    # cv2.imshow("Image", closed_deilate)
    # cv2.waitKey(0)

    # получаем точки вокруг контура
    c = max(cnts, key=cv2.contourArea)
    rect = cv2.minAreaRect(c)
    box = np.int0(cv2.boxPoints((rect[0], (rect[1][0] + 30, rect[1][1] + 30), (rect[2]))))

    size_src = max(img_h, img_w)
    box_f32 = np.float32(sorted(box, key=sum))
    size = size_src
    if img_h > img_w:
        image_points = np.float32(((0, 0), (0, size), (size, 0), (size, size)))
    else:
        image_points = np.float32(((0, 0), (size, 0), (0, size), (size, size)))
    M = cv2.getPerspectiveTransform(box_f32 * size_src / size_for_proc, image_points)

    # получаем картинку в высоком качестве
    image_res_for_export = cv2.resize(image, (size_src, size_src))
    gray = cv2.cvtColor(image_res_for_export, cv2.COLOR_BGR2GRAY)
    image_out = cv2.resize(cv2.warpPerspective(gray, M, (size, size)), (512, 512))

    try:
        decoded_str = decode(image_out)

        print(decoded_str)

        image_file = 'output-images/' + os.path.basename(decoded_str + '.' + filename.split('.')[-1])
        cv2.imwrite(image_file, image_out)
    except:
        pass
