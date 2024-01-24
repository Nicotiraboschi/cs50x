-- Keep a log of any SQL queries you execute as you solve the mystery.

-- get informations about the crime scene
SELECT * FROM crime_scene_reports
WHERE crime_scene_reports.year = 2021
AND crime_scene_reports.month = 07
AND crime_scene_reports.day = 28
AND crime_scene_reports.street = 'Humphrey Street';

--get the interviews
SELECT * FROM interviews
WHERE interviews.year = 2021
AND interviews.month = 07
AND interviews.day = 28
AND interviews.transcript LIKE '%bakery%';

--get account_number from ATM transaction
SELECT atm_transactions.account_number FROM atm_transactions
WHERE atm_transactions.day = 28
AND atm_transactions.month = 07
AND atm_transactions.year = 2021
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw';

--get range of person_id from bank_accounts, confronting with account_number at ATM
SELECT bank_accounts.person_id
FROM bank_accounts
WHERE bank_accounts.account_number IN
(
SELECT atm_transactions.account_number FROM atm_transactions
WHERE atm_transactions.day = 28
AND atm_transactions.month = 07
AND atm_transactions.year = 2021
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw'
)ORDER BY person_id;

+--------+
|   id THIEF |
+--------+
| 395717 |
| 396669 |
| 438727 |
| 449774 |
| 458378 |
| 467400 |
| 514354 |
| 686048 |
+--------+

--get id based on license plate
SELECT people.id
    FROM people
        WHERE people.license_plate IN
            (SELECT license_plate FROM bakery_security_logs
            WHERE bakery_security_logs.day = 28
            AND bakery_security_logs.month = 07
            AND bakery_security_logs.year = 2021
            AND bakery_security_logs.hour = 10
            AND bakery_security_logs.minute < 26
            AND bakery_security_logs.activity = 'exit');
+--------+
|   id ??? |
+--------+
| 221103 |
| 243696 |
| 396669 |
| 398010 |
| 467400 |
| 514354 |
| 560886 |
| 686048 |
+--------+

--get ID of airport
SELECT * FROM airports
WHERE city = 'Fiftyville';

--get first flight of the next day
SELECT * FROM flights
WHERE day = 29
AND month = 07
AND year = 2021
AND origin_airport_id = 8
ORDER BY hour, minute;

-- destination of flight
SELECT * FROM airports
WHERE id = 4;

-- get passport of passengers of flight
SELECT id
    FROM people
        WHERE people.passport_number IN
            (SELECT passport_number FROM passengers
            WHERE flight_id = 36);
+--------+
|   id HELPER  |
+--------+
| 395717 |
| 398010 |
| 449774 |
| 467400 |
| 560886 |
| 651714 |
| 686048 |
| 953679 |
+--------+

--get info on phone call
SELECT caller, receiver FROM phone_calls
WHERE day = 28
AND month = 07
AND year = 2021
AND duration < 60;

+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (130) 555-0289 | (996) 555-8899 |
| (499) 555-9472 | (892) 555-8872 |
| (367) 555-5533 | (375) 555-8161 |
| (499) 555-9472 | (717) 555-1342 |
| (286) 555-6063 | (676) 555-6554 |
| (770) 555-1861 | (725) 555-3243 |
| (031) 555-6622 | (910) 555-3251 |
| (826) 555-1652 | (066) 555-9701 |
| (338) 555-6650 | (704) 555-2131 |
+----------------+----------------+


--get id of callers
SELECT DISTINCT people.id FROM people
JOIN phone_calls ON people.phone_number
IN (SELECT phone_calls.caller FROM phone_calls
WHERE day = 28
AND month = 07
AND year = 2021
AND phone_calls.duration < 60)
ORDER BY people.id;

+--------+
|   id THIEF  |
+--------+
| 395717 |
| 398010 |
| 438727 |
| 449774 |
| 514354 |
| 560886 |
| 686048 |
| 907148 |
+--------+

--get id of receivers
SELECT DISTINCT people.id FROM people
JOIN phone_calls ON people.phone_number
IN (SELECT phone_calls.receiver FROM phone_calls
WHERE day = 28
AND month = 07
AND year = 2021
AND phone_calls.duration < 60)
ORDER BY people.id;

|   id HELPER  |
+--------+
| 250277 |
| 251693 |
| 484375 |
| 567218 |
| 626361 |
| 712712 |
| 847116 |
| 864400 |
| 953679 |
+--------+


--caller (receiver)? and transactioner at ATM must combine (THIEF) and license plate (686048)
SELECT DISTINCT people.id FROM people
    JOIN phone_calls ON people.phone_number
        IN (SELECT phone_calls.caller FROM phone_calls
            WHERE day = 28
                AND month = 07
                    AND year = 2021
                    AND phone_calls.duration < 60)
INTERSECT
    SELECT people.id FROM people
        WHERE people.id IN(
            SELECT DISTINCT bank_accounts.person_id
                FROM bank_accounts
                    WHERE bank_accounts.account_number IN
                        (
                            SELECT atm_transactions.account_number FROM atm_transactions
                                WHERE atm_transactions.day = 28
                                    AND atm_transactions.month = 07
                                    AND atm_transactions.year = 2021
                                    AND atm_transactions.atm_location = 'Leggett Street'
                                    AND atm_transactions.transaction_type = 'withdraw'
                        ))
INTERSECT
    SELECT people.id
    FROM people
        WHERE people.license_plate IN
            (SELECT license_plate FROM bakery_security_logs
            WHERE bakery_security_logs.day = 28
            AND bakery_security_logs.month = 07
            AND bakery_security_logs.year = 2021
            AND bakery_security_logs.hour = 10
            AND bakery_security_logs.minute < 26
            AND bakery_security_logs.activity = 'exit')
INTERSECT
    SELECT DISTINCT people.id FROM people
    JOIN passengers ON people.passport_number
        IN (SELECT passengers.passport_number FROM passengers
            WHERE passengers.flight_id = 36);

--get back phone from id of receiver THIEF phone NUMBER ('(367) 555-5533')

SELECT DISTINCT people.phone_number
    FROM people
        WHERE people.id = 686048;

--get back phone from id of caller HELPER phone NUMBER ('(375) 555-8161')

SELECT phone_calls.receiver
    FROM phone_calls
        WHERE caller = '(367) 555-5533'
        AND day = 28
        AND month = 07
        AND year = 2021
        AND phone_calls.duration < 60;

--get HELPER id when I know phone (847116)

SELECT people.id
    FROM people
        WHERE phone_number = '(375) 555-8161';

--get caller (receiver) name knowing phone ROBIN

SELECT people.name
    FROM people
        WHERE id = 864400;

--get thief (caller) name knowing phone BRUCE

SELECT people.name
    FROM people
        WHERE id = 686048;
