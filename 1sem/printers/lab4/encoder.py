import numpy as np
import cv2

left_codes_dict = {
    '0': "AAAAAA",
    '1': "AABABB",
    '2': "AABBAB",
    '3': "AABBBA",
    '4': "ABAABB",
    '5': "ABBAAB",
    '6': "ABBBAA",
    '7': "ABABAB",
    '8': "ABABBA",
    '9': "ABBABA",
}

sign_codes_dict = {
    'A': {
        '0': "3211",
        '1': "2221",
        '2': "2122",
        '3': "1411",
        '4': "1132",
        '5': "1231",
        '6': "1114",
        '7': "1312",
        '8': "1213",
        '9': "3112",
    },
    'B': {
        '0': "1123",
        '1': "1222",
        '2': "2212",
        '3': "1141",
        '4': "2311",
        '5': "1321",
        '6': "4111",
        '7': "2131",
        '8': "3121",
        '9': "2113",
    },
    'C': {
        '0': "3211",
        '1': "2221",
        '2': "2122",
        '3': "1411",
        '4': "1132",
        '5': "1231",
        '6': "1114",
        '7': "1312",
        '8': "1213",
        '9': "3112",
    },
}


def encode_string(input_str):
    left_pattern = left_codes_dict[input_str[0]]

    left_part = input_str[1:7]
    right_part = input_str[7:]

    width_arr = [1, 1, 1]

    # print(left_pattern)
    for num, code_sign in zip(left_part, left_pattern):
        num_width = sign_codes_dict[code_sign][num]
        for width_str in num_width:
            width_arr.append(int(width_str))

    # print(left_part)

    width_arr += [1, 1, 1, 1, 1]
    # print(len(width_arr))
    for num in right_part:
        num_width = sign_codes_dict['C'][num]
        for width_str in num_width:
            width_arr.append(int(width_str))

    # print(right_part)

    width_arr += [1, 1, 1]
    return width_arr


def generate_check_sum(input_str):
    int_list = [int(char) for char in input_str ]
    check_sum = str(10 - (sum(int_list[0:-1:2]) + 3 * sum(int_list[1:-1:2])) % 10)
    return input_str + check_sum


def encode(input_str):
    if len(input_str) == 11:
        input_str = '0' + input_str

    assert len(input_str) == 12

    str_with_check = generate_check_sum(input_str)

    width_list = encode_string(str_with_check)
    # print(len(width_list))
    black = False
    pixel_list = []
    pixel_wid = 5
    for wid in width_list:
        for k in range(wid):
            for j in range(pixel_wid):
                pixel_list.append(1 if black else 0)
        black = not black

    pixel_list = [1] * 30 + pixel_list + [1] * 30

    pixel_arr = np.array(pixel_list)
    zeros = np.ones(len(pixel_arr))
    top = [zeros] * 40
    code = [pixel_arr] * 300
    bottom = [zeros] * 40

    picture = np.array(top + code + bottom) * 255
    return picture, str_with_check

es = "123456789123"
print(f"\nInput: {es}")
img_out, str_with_check = encode(es)
print(f"\nWith checksum: {str_with_check}")
filename = "v28.png"
print(f"\nSaved as: {filename}")
cv2.imwrite(filename, img_out)