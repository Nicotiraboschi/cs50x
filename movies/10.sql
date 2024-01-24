SELECT name FROM people WHERE id IN (SELECT person_id FROM directors WHERE MOVIE_ID IN (SELECT movie_id FROM ratings WHERE rating >= 9.0));

SELECT COUNT(DISTINCT name) FROM people
JOIN directors ON directors.person_id = people.id
JOIN movies ON movies.id  = directors.movie_id
JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.rating >= 9.0;