from typing import List, NamedTuple, Tuple
from tabulate import tabulate

class Candidate:
    """A candidate in the election"""

    def __init__(self, name: str):
        self.name = name

    def __str__(self) -> str:
        return self.name

    def __repr__(self) -> str:
        return f"<Candidate('{self.name}')>"

    def __hash__(self):
        return hash(self.name)

    def __eq__(self, other) -> bool:
        return self.name == other.name


class Ballot:
    """
    A ballot (vote) where the voter has ranked all of the candidates.
    """

    def __init__(self, ranked_candidates: List[Candidate]):
        self.ranked_candidates: Tuple[Candidate] = tuple(ranked_candidates)

    def __repr__(self) -> str:
        candidate_name = ", ".join([candidate.name for candidate in self.ranked_candidates])
        return f"<Ballot({candidate_name})>"



class CandidateResult(NamedTuple):
    candidate: Candidate
    number_of_votes: int


class RoundResult:
    candidate_results: List[CandidateResult]

    def __init__(self, candidate_results):
        self.candidate_results = sorted(candidate_results, key=lambda x: x.number_of_votes, reverse=True)

    def __repr__(self):
        representation_string = "<RoundResult>"
        return representation_string

    def __str__(self):
        results = self.candidate_results
        pretty_print_string = tabulate(
            results,
            headers=['Candidate', 'Votes', 'Status']
        )
        return pretty_print_string


class ElectionResults:
    """
    ElectionResults store the result of all rounds in the election:
     - the ranking of candidates
     - how many votes they got
     - their election status (elected, hopeful, rejected)
    ElectionResults.get_winners() makes it trivial to receive the elected candidates.
    ElectedResults can be printed
    """

    def __init__(self):
        self.rounds: List[RoundResult] = []

    def register_round_results(self, round_: RoundResult):
        self.rounds.append(round_)

    def get_winner(self) -> Candidate:
        last_round = self.rounds[-1]
        winner_candidate = last_round.candidate_results[0]
        return winner_candidate

    def __repr__(self) -> str:
        return f"<ElectionResults({len(self.rounds)} rounds)>"

    def __str__(self) -> str:
        lines = []
        for i, round_ in enumerate(self.rounds):
            if i == len(self.rounds)-1:
                lines.append("FINAL RESULT")
            else:
                round_nr = i + 1
                lines.append(f"ROUND {round_nr}")
            lines.append(round_.__str__())
            lines.append("")
        string = "\n".join(lines)
        return string