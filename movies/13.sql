
SELECT people.name FROM people WHERE people.id IN (SELECT DISTINCT stars.person_id FROM stars WHERE stars.movie_id IN (SELECT stars.movie_id FROM stars JOIN people ON stars.person_id = people.id
WHERE people.name = 'Kevin Bacon' AND people.birth = 1958)) AND people.name <> 'Kevin Bacon';