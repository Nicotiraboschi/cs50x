import os, datetime, pytz

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)
        if stock == None:
            return apology("No stock found called like that")
        else:
            try:
                shares = int(shares)
            except ValueError:
                return apology("Wrong number of shares")
            if shares <= 0:
                return apology("Wrong number of shares")
            else:
                user = session["user_id"]
                cash_remaining = db.execute(
                    "SELECT cash FROM users WHERE id = ?", user
                )[0]["cash"]
                time = datetime.datetime.now(pytz.timezone("US/Eastern"))
                if stock["price"] * shares < cash_remaining:
                    total_purchase = stock["price"] * shares
                    cash_remaining = cash_remaining - total_purchase
                    old_shares = db.execute(
                        "SELECT shares FROM home WHERE user = ? AND stock = ? LIMIT 1",
                        user,
                        symbol,
                    )
                    try:
                        old_shares = old_shares[0]["shares"]
                    except IndexError:
                        old_shares = 0
                        db.execute(
                            "INSERT INTO home (user, stock, price, shares, time) VALUES (?, ?, ?, ?, ?)",
                            user,
                            symbol,
                            stock["price"],
                            shares,
                            time,
                        )

                    db.execute(
                        "INSERT INTO history (action, user, stock, price, shares, time) VALUES ('buy', ?, ?, ?, ?, ?)",
                        user,
                        symbol,
                        stock["price"],
                        shares,
                        time,
                    )
                    db.execute(
                        "UPDATE home SET shares = ? WHERE user = ? AND stock = ? LIMIT 1",
                        old_shares + shares,
                        user,
                        symbol,
                    )
                    print(total_purchase, cash_remaining)
                    db.execute(
                        "UPDATE users SET cash = ? WHERE id = ?", cash_remaining, user
                    )
                    return redirect("/")
                else:
                    return apology("Not enough money")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user = session["user_id"]
    history = db.execute("SELECT * FROM history WHERE user = ?", user)
    n = 0
    count = db.execute("SELECT COUNT(*) FROM history")[0]["COUNT(*)"]
    row = [[] for list in range(count)]
    for movement in history:
        print(movement, "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ")
        action = movement["action"]
        stock = movement["stock"]
        price = movement["price"]
        shares = movement["shares"]
        total = price * shares
        time = movement["time"]

        row[n] = [action, stock, usd(price), shares, usd(total), time]
        n += 1

    return render_template("history.html", row=row)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password")

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if stock == None:
            return apology("No stock found called like that")
        else:
            stock["price"] = usd(stock["price"])
            return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        # Ensure username was submitted
        if not username:
            return apology("must provide username")

        # Ensure password was submitted
        elif not password:
            return apology("must provide password")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username exists and password is correct
        if len(rows) == 1 or (password != request.form.get("confirmation")):
            return apology("invalid username and/or password")
        password = generate_password_hash(password)

        # Remember which user has logged in
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?,?)", username, password
        )
        return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)
        if stock == None:
            return apology("No stock found called like that")
        else:
            time = datetime.datetime.now(pytz.timezone("US/Eastern"))
            try:
                shares = int(shares)
            except ValueError:
                return apology("Wrong number of shares")
            if shares <= 0:
                return apology("Wrong number of shares")
            else:
                user = session["user_id"]
                price = stock["price"]
                # try:
                old_shares = db.execute(
                    "SELECT shares FROM home WHERE stock = ? AND shares >= ? AND user = ? LIMIT 1",
                    symbol,
                    shares,
                    user,
                )
                try:
                    old_shares = old_shares[0]["shares"]
                except IndexError:
                    return apology("no stock to sell")
                db.execute(
                    "INSERT INTO history (action, user, stock, price, shares, time) VALUES ('sell', ?, ?, ?, ?, ?)",
                    user,
                    symbol,
                    stock["price"],
                    shares,
                    time,
                )
                db.execute(
                    "UPDATE home SET shares = ? WHERE stock = ? AND shares >= ? AND user = ? LIMIT 1",
                    old_shares - shares,
                    symbol,
                    shares,
                    user,
                )
                db.execute("DELETE FROM home WHERE user = ? AND shares = 0", user)
                total_sell = price * shares
                cash_remaining = (
                    db.execute("SELECT cash FROM users WHERE id = ?", user)[0]["cash"]
                    + total_sell
                )
                db.execute(
                    "UPDATE users SET cash = ? WHERE id = ?", cash_remaining, user
                )
                return redirect("/")
    else:
        user = session["user_id"]
        stocks = db.execute("SELECT DISTINCT stock FROM home WHERE user = ?", user)
        print(stocks, "----------------")
        return render_template("sell.html", stocks=stocks)


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user = session["user_id"]
    home = db.execute("SELECT price, shares, stock FROM home WHERE user = ?", user)
    n = 0
    count = db.execute("SELECT COUNT(*) FROM home")[0]["COUNT(*)"]
    row = [[] for list in range(count)]
    total_cash = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]["cash"]
    cash_remaining = total_cash
    for purchase in home:
        stock = purchase["stock"]
        price = lookup(stock)["price"]
        total_cash += price * purchase["shares"]
        total = usd(price * purchase["shares"])

        row[n] = [
            stock,
            stock,
            purchase["shares"],
            usd(price),
            usd((price) * purchase["shares"]),
            total,
        ]
        n += 1

    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_remaining, user)

    return render_template(
        "index.html",
        row=row,
        total_cash=usd(total_cash),
        cash_remaining=usd(cash_remaining),
    )


@app.route("/password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        old_password = request.form.get("old_password")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        password_hash = db.execute(
            "SELECT hash FROM users WHERE id = ?", session["user_id"]
        )[0]["hash"]
        print(password_hash, old_password, password, confirmation, "✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅")
        if not check_password_hash(password_hash, old_password) or (
            password != confirmation
        ):
            return apology("wrong password")
        else:
            db.execute(
                "UPDATE users SET hash = ? WHERE id = ?",
                generate_password_hash(request.form.get("password")),
                session["user_id"],
            )
            return redirect("/")
    else:
        return render_template("password.html")
