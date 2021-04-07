glob_voice_dict = {
    "dacb": 5,
    "abcd": 4,
    "bcda": 2,
}

def absolute_max(voice_dict): # метод абсолютної більшості в 2 тури
    print("\nAbsolute max (2 tours)")
    sorted_voices = sorted(voice_dict.items(), key=lambda x: x[1], reverse=True) # відсортовані голоси (від найб. кл. до найменшої)
    first_tour_results = list(map(lambda x: f"{x[0][0]}: {x[1]}", sorted_voices)) # список кандидатів за кількістю їх голосів
    print(f"First tour results:{first_tour_results}")

    second_tour_candidates = list(map(lambda x: x[0][0], sorted_voices[:2])) # залишаємо 2х кандидатів з найб. кл. голосів
    print(f"Second tour candidates:{second_tour_candidates}")

    second_tour_dict = dict(zip(second_tour_candidates, [0]*len(second_tour_candidates))) # cписок кандидатів 2го туру
    # рахуємо кількість голосів цих кандидатів
    for candidate_list, voices in sorted_voices:
        for candidate in candidate_list:
            if candidate in second_tour_candidates:
                second_tour_dict[candidate] += voices
                break
    sorted_voices = sorted(second_tour_dict.items(), key=lambda x: x[1], reverse=True) # відсортовані голоси другого туру
    second_tour_results = list(map(lambda x: f"{x[0][0]}: {x[1]}", sorted_voices)) # список кандидатів так кл. їх голосів
    print(f"Second tour results:{second_tour_results}")
    print(f"Winner is {second_tour_results[0]} voices!")


def simpson(voice_dict): # метод Сімпсона
    print("\nSimpson")

    def get_score(cand_x, cand_y): # функція для підрахунку кількості голосів
        score = 0
        # рахуємо скільки виборців надали перевагу другому кандидату
        for candidate_list, voices in voice_dict.items():
            if candidate_list.find(cand_x) < candidate_list.find(cand_y):
                score += voices
        return score

    candidate_list = sorted(list(voice_dict.keys())[0]) # список кандидатів
    candidates_scores = dict()
    # знаходимо найгірший результат порівняння (кількість голосів) кандидата відносно інших
    for candidate_x in candidate_list: # кандидат, відносно якого будуть порівнюватись інші
        min_score = sum(voice_dict.values()) # задаємо початкове мінімальне значення балів, що дорівнює загальній кл. голосів
        for candidate_y in candidate_list: # другий кандидат
            if candidate_x != candidate_y:
                score = get_score(candidate_x, candidate_y) # рахуємо кількість голосів виборців
                if score < min_score: # якщо отримана кл. голосів менша за попередню -> записуємо
                    min_score = score
        candidates_scores[candidate_x] = min_score # записуємо кандидату кількість голосів
    sorted_scores = sorted(candidates_scores.items(), key=lambda x: x[1], reverse=True) # сортуємо кл. голосів
    results = list(map(lambda x: f"{x[0][0]}: {x[1]}", sorted_scores)) # виводим кандидатів за кількістю голосів
    print(f"Results:{results}")
    print(f"Winner is {results[0]} score!")


def sequent_exclusion(voice_dict): # метод послідовного виключення
    print("\nSequent exclusion")
    def compare(cand_x, cand_y): # функція, для порівняння кандидатів
        x_score = 0
        y_score = 0
        for candidate_list, voices in voice_dict.items():
            if candidate_list.find(cand_x) < candidate_list.find(cand_y):
                x_score += voices
            else:
                y_score += voices
        return cand_x if x_score > y_score else cand_y

    candidate_list = sorted(list(voice_dict.keys())[0]) # список кандидатів
    curr_leader = candidate_list[0] # кандидат, з якого починається порівняння (якого порівнюють)

    for cand in candidate_list[1:]:
        new_leader = compare(curr_leader, cand) # порівнюємо 2 кандидата
        print(f"Comparing {curr_leader} with {cand}: {new_leader} wins!")
        curr_leader = new_leader # кандидат,котрий має більше голосів - стає наступним кандидатом, котрого будуть порівнювати
    print(f"Winner is {curr_leader} !")

absolute_max(glob_voice_dict)
simpson(glob_voice_dict)
sequent_exclusion(glob_voice_dict)