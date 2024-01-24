WITH avg_energy_drake AS (
    SELECT AVG(energy) FROM songs
    JOIN artists ON songs.artist_id = artists.id
    WHERE artists.name = 'Drake'
    ) SELECT * FROM avg_energy_drake;