import elections
from elections import Candidate, Ballot

#register candidates
cand_a = Candidate("a")
cand_b = Candidate("b")
cand_c = Candidate("c")
cand_d = Candidate("d")

candidates = [cand_a, cand_b, cand_c, cand_d]

#set ballots
ballots = [
Ballot(ranked_candidates=[cand_c, cand_a, cand_b, cand_d]),
Ballot(ranked_candidates=[cand_c, cand_a, cand_b, cand_d]),
Ballot(ranked_candidates=[cand_c, cand_a, cand_b, cand_d]),
Ballot(ranked_candidates=[cand_c, cand_a, cand_b, cand_d]),
Ballot(ranked_candidates=[cand_c, cand_a, cand_b, cand_d]),
Ballot(ranked_candidates=[cand_a, cand_b, cand_c, cand_d]),
Ballot(ranked_candidates=[cand_a, cand_b, cand_c, cand_d]),
Ballot(ranked_candidates=[cand_a, cand_b, cand_c, cand_d]),
Ballot(ranked_candidates=[cand_a, cand_b, cand_c, cand_d]),
Ballot(ranked_candidates=[cand_b, cand_a, cand_d, cand_c]),
Ballot(ranked_candidates=[cand_b, cand_a, cand_d, cand_c]),
]

print("ABSOLUTE MAXIMUM")
election_result = elections.absolute_max(candidates, ballots)
winner = election_result.get_winner()
print(election_result)
print(f"Winner is candidate \"{winner.candidate}\" with {winner.number_of_votes} votes!")

print("\nSIMPSON")
election_result = elections.simpson(candidates, ballots)
winner = election_result.get_winner()
print(election_result)
print(f"Winner is candidate \"{winner.candidate}\" with {winner.number_of_votes} votes!")

print("\nSEQUENT EXCLUSION")
election_result = elections.sequent_exclusion(candidates, ballots)
winner = election_result.get_winner()
print(election_result)
print(f"Winner is candidate \"{winner.candidate}\"!")
