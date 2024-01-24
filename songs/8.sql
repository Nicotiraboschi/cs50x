WITH feat AS (
    SELECT name FROM songs
    WHERE name LIKE '%feat.%'
) SELECT * FROM feat;