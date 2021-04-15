from typing import List
from elections.models import Candidate, Ballot, ElectionResults, RoundResult, CandidateResult

def absolute_max(candidates: List[Candidate], ballots: List[Ballot]) -> ElectionResults:
    """
    Calculates ElectionResult with absolute maximum method
    :param candidates:
    :param ballots:
    :return: ElectionResults
    """
    er = ElectionResults()
    candidate_results = []
    for candidate in candidates:
        votes = 0
        for ballot in ballots:
            if ballot.ranked_candidates[0] == candidate:
                votes += 1
        candidate_results.append(CandidateResult(candidate, votes))
    rr = RoundResult(candidate_results)
    er.register_round_results(rr)
    sorted_candidates = sorted(rr.candidate_results, key=lambda x: x.number_of_votes, reverse=True)
    second_tour_candidates = list(map(lambda x: x.candidate, sorted_candidates))[:2]
    second_tour_candidates_dict = dict(zip(second_tour_candidates, [0]*len(second_tour_candidates)))
    for ballot in ballots:
        for candidate in ballot.ranked_candidates:
            if candidate in second_tour_candidates:
                second_tour_candidates_dict[candidate] += 1
                break
    candidate_results = [CandidateResult(candidate, votes) for candidate, votes in second_tour_candidates_dict.items()]
    rr = RoundResult(candidate_results)
    er.register_round_results(rr)
    return er


def simpson(candidates: List[Candidate], ballots: List[Ballot]) -> ElectionResults:
    """
    Calculates ElectionResult with simpson method
    :param candidates:
    :param ballots:
    :return: ElectionResults
    """
    er = ElectionResults()
    def get_score(cand_x: Candidate, cand_y: Candidate):
        score = 0
        for ballot in ballots:
            if ballot.ranked_candidates.index(cand_x) < ballot.ranked_candidates.index(cand_y):
                score += 1
        return score

    candidates_scores = dict()
    for candidate_x in candidates:
        min_score = len(ballots)
        for candidate_y in candidates:
            if candidate_x != candidate_y:
                score = get_score(candidate_x, candidate_y)
                if score < min_score:
                    min_score = score
        candidates_scores[candidate_x] = min_score
    candidate_results = [CandidateResult(candidate, votes) for candidate, votes in candidates_scores.items()]
    rr = RoundResult(candidate_results)
    er.register_round_results(rr)
    return er


def sequent_exclusion(candidates: List[Candidate], ballots: List[Ballot]) -> ElectionResults:
    """
    Calculates ElectionResult with sequent exclusion method
    :param candidates:
    :param ballots:
    :return: ElectionResults
    """
    def get_scores(cand_x: Candidate, cand_y: Candidate):
        x_score = 0
        y_score = 0
        for ballot in ballots:
            if ballot.ranked_candidates.index(cand_x) < ballot.ranked_candidates.index(cand_y):
                x_score += 1
            else:
                y_score += 1
        return x_score, y_score

    er = ElectionResults()
    curr_leader = candidates[0]
    for cand in candidates[1:]:
        curr_leader_score, cand_score = get_scores(curr_leader, cand)
        rr = RoundResult([CandidateResult(curr_leader, curr_leader_score), CandidateResult(cand, cand_score)])
        er.register_round_results(rr)
        curr_leader = curr_leader if curr_leader_score > cand_score else cand
    return er