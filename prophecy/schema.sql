from cs50 import SQL

db = SQL("sqlite:///roster.db")

db.execute("CREATE TABLE student (id INTEGER,student_name TEXT,PRIMARY KEY(id));")

db.execute("CREATE TABLE houses (id INTEGER,house TEXT,PRIMARY KEY(id));")

db.execute("CREATE TABLE link (student_id INTEGER,house_id INTEGER);")

with open("students.csv", "r") as filename:
    reader = filename
    next(reader)
    houses = []
    for row in reader:
        row_arr = row.split(",")
        db.execute("INSERT or IGNORE INTO student (id, student_name) VALUES (?, ?);", row_arr[0], row_arr[1])
        if row_arr[2] in houses:
            db.execute("INSERT INTO link (student_id, house_id) VALUES (?, (SELECT id FROM houses WHERE house = ?));",row_arr[0], row_arr[2])

        else:
            houses.append(row_arr[2])
            db.execute("INSERT or IGNORE INTO houses (house) VALUES (?);", row_arr[2])
            db.execute("INSERT INTO link (student_id, house_id) VALUES (?, (SELECT id FROM houses WHERE house = ?));",row_arr[0], row_arr[2])

    link = db.execute("SELECT * FROM link;")
    houses = db.execute("SELECT * FROM houses;")
    student = db.execute("SELECT * FROM student;")
    print(student)
    db.execute("DROP TABLE houses;")
    db.execute("DROP TABLE link;")
    db.execute("DROP TABLE student;")