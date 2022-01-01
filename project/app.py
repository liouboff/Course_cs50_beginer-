import os

import datetime
import shelve


from flask import Flask, render_template, flash, redirect, request, session
from flask_session import Session

from tempfile import mkdtemp

from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from extent import apology, login_required
from cs50 import SQL




app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///organiser.db")



@app.route("/", methods=["GET","POST"])
def index():
    if not session:
            return render_template("block.html")

    else:
        if request.method == "POST":

            lock = db.execute("SELECT position FROM employees WHERE id = ?", session["session_id"])
            if lock[0]["position"] == "manager":
                return redirect("/manager_table")
            else:
                return redirect("/worker_table")
        else:

            to_day = datetime.datetime.now()
            bw_day = to_day.strftime("%A")
            sw_day = to_day.strftime("%a")
            nw_day = to_day.strftime("%w")
            day = to_day.strftime("%d")
            month = to_day.strftime("%B")
            year = to_day.strftime("%Y")

            result_mrows = db.execute("SELECT * FROM mschedule ORDER BY id ASC")
            if len(result_mrows) != 1:
                db.execute("DELETE FROM mschedule ")
            result_erows =db.execute("SELECT * FROM eschedule ORDER BY id ASC")
            if len(result_mrows) != 1:
                db.execute("DELETE FROM eschedule ")
            request_rows = db.execute("SELECT  myshifts.sun AS sun, myshifts.mon AS mon, myshifts.tue AS tue, myshifts.wed AS wed, myshifts.thu AS thu, myshifts.fri AS fri, myshifts.sat AS sat, myshifts.exp AS exp, employees.name AS name FROM myshifts INNER JOIN employees ON myshifts.employee = employees.id ")

            return render_template("index.html",  bw_day = bw_day, day = day, month = month, year = year, request_rows = request_rows, result_mrows = result_mrows, result_erows = result_erows)


@app.route("/login_phone", methods=["GET", "POST"])
def login_phone():
    """" Log worker with phone number """""
    # Forget any user_id
    session.clear()

    if request.method == "POST":


        # Ensure username was submitted
        if not request.form.get("phone"):
            return apology("must provide phone", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM employees WHERE phone = ?", request.form.get("phone"))

        # Ensure username exists and phone is correct
        if len(rows) != 1 or rows[0]["phone"] != request.form.get("phone"):
            return apology("wrong  phone number", 403)
        if rows[0]["position"] == "manager":
            return apology("please login with password", 403)

        # Remember which user has logged in
        session["session_id"] = rows[0]["id"]

        worker_name = rows[0]["name"]


        # Moove user to the workers page
        return render_template("worker_table.html", worker_name = "Hi, " + worker_name)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return redirect("/")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("name"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM employees WHERE name = ?", request.form.get("name"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["session_id"] = rows[0]["id"]

        # Moove user to the manager page
        return redirect ("/manager_table")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if (request.method == "POST"):
        name = request.form.get("name")
        phone = request.form.get("phone")
        password = request.form.get("password")
        confirm = request.form.get("confirmation")

        if not name:
            return apology("what do you mean?")
        rows = db.execute("SELECT * FROM employees WHERE name = ?", name)
        if rows:
            return apology("There somebody have the same name?")
        if not phone:
            return apology("type yore phone number?")
        rows = db.execute("SELECT * FROM employees WHERE phone = ?", phone)
        if rows:
            return apology("There somebody have the same phone number?")
        if not password:
            return apology("but what with the password")
        if password != confirm:
            return apology("you typing it wrong")

        hash = generate_password_hash(password)
        try:
            db.execute("INSERT INTO employees (name, position, phone, hash) VALUES (?, ?, ?, ?)", name, "manager", phone, hash)
        except:
            return apology("looks like we have alredy this username")

            # Redirect user to login form
        return render_template("login.html")
    else:
        return render_template("register.html")
    # return apology("oi oi")



@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route("/add_employee", methods=["GET", "POST"])
@login_required
def edd_employee():
    """Register employee"""
    if (request.method == "POST"):
        name = request.form.get("name")
        phone = request.form.get("phone")
        position = request.form.get("position")
        if not name:
            return apology("what the name?")
        rows = db.execute("SELECT * FROM employees WHERE name = ?", name)
        if rows:
            return apology("There somebody have the same phone number?")
        if not phone:
            return apology("type phone number?")
        rows = db.execute("SELECT * FROM employees WHERE phone = ?", phone)
        if rows:
            return apology("There somebody have the same phone number?")
        if not position:
            return apology("type position")

        try:
            db.execute("INSERT INTO employees (name, position, phone) VALUES (?, ?, ?)", name, position, phone)
        except:
            return apology("looks like we have alredy this username")

            # Redirect  to manager table
        return redirect("/manager_table")
    else:
        return render_template("manager_table.html")


@app.route("/manager_table", methods=["GET", "POST"])
@login_required
def manager_table():
    if request.method == "POST":

        return render_template("manager_table.html")
    else:
        #show table of workers
        table_rows = db.execute("SELECT * FROM employees")
        # finde worker's name
        rows = db.execute("SELECT name FROM employees WHERE id = ?", session["session_id"])
        worker_name = rows[0]["name"]
        #show table of shifts requests
        request_rows = db.execute("SELECT  myshifts.id AS id, myshifts.date AS date, myshifts.sun AS sun, myshifts.mon AS mon, myshifts.tue AS tue, myshifts.wed AS wed, myshifts.thu AS thu, myshifts.fri AS fri, myshifts.sat AS sat, myshifts.exp AS exp, employees.name AS name FROM myshifts INNER JOIN employees ON myshifts.employee = employees.id ")


        return render_template("manager_table.html", worker_name = "Hello dear "+ worker_name, table_rows = table_rows, request_rows = request_rows)


@app.route("/shift_assignment", methods=["GET", "POST"])
@login_required
def shift_assignment():
    if request.method == "POST":
        msun = [request.form.get("msun"), request.form.get("msun1"), request.form.get("msun2"), request.form.get("msun3")]
        msun = ' '.join(msun)
        mmon = [request.form.get("mmon"), request.form.get("mmon1"), request.form.get("mmon2"), request.form.get("mmon3")]
        mmon = ' '.join(mmon)
        mtue = [request.form.get("mtue"), request.form.get("mtue1"), request.form.get("mtue2"), request.form.get("mtue3")]
        mtue = ' '.join(mtue)
        mwed = [request.form.get("mwed"), request.form.get("mwed1"), request.form.get("mwed2"), request.form.get("mwed3")]
        mwed = ' '.join(mwed)
        mthu = [request.form.get("mthu"), request.form.get("mthu1"), request.form.get("mthu2"), request.form.get("mthu3")]
        mthu = ' '.join(mthu)
        mfri = [request.form.get("mfri"), request.form.get("mfri1"), request.form.get("mfri2"), request.form.get("mfri3")]
        mfri = ' '.join(mfri)
        msat = [request.form.get("msat"), request.form.get("msat1"), request.form.get("msat2"), request.form.get("msat3")]
        msat = ' '.join(msat)
        edsun = request.form.get("ed_msun")
        edmon = request.form.get("ed_mmon")
        edtue = request.form.get("ed_mtue")
        edwed = request.form.get("ed_mwed")
        edthu = request.form.get("ed_mthu")
        edfri = request.form.get("ed_mfri")
        edsat = request.form.get("ed_msat")

        try:
            db.execute("INSERT INTO mschedule (edsun, edmon, edtue, edwed, edthu, edfri, edsat, sun, mon, tue, wed, thu, fri, sat) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", edsun, edmon, edtue, edwed, edthu, edfri, edsat, msun, mmon, mtue, mwed, mthu, mfri, msat)
        except:
            return apology("can't get values")
        #return redirect ("/manager_table")

        esun = [request.form.get("esun"), request.form.get("esun1"), request.form.get("esun2"), request.form.get("esun3")]
        esun = ' '.join(esun)
        emon = [request.form.get("emon"), request.form.get("emon1"), request.form.get("emon2"), request.form.get("emon3")]
        emon = ' '.join(emon)
        etue = [request.form.get("etue"), request.form.get("etue1"), request.form.get("etue2"), request.form.get("etue3")]
        etue = ' '.join(etue)
        ewed = [request.form.get("ewed"), request.form.get("ewed1"), request.form.get("ewed2"), request.form.get("ewed3")]
        ewed = ' '.join(ewed)
        ethu = [request.form.get("ethu"), request.form.get("ethu1"), request.form.get("ethu2"), request.form.get("ethu3")]
        ethu = ' '.join(ethu)
        efri = [request.form.get("efri"), request.form.get("efri1"), request.form.get("efri2"), request.form.get("efri3")]
        efri = ' '.join(efri)
        esat = [request.form.get("esat"), request.form.get("esat1"), request.form.get("esat2"), request.form.get("esat3")]
        esat = ' '.join(esat)
        edesun = request.form.get("ed_esun")
        edemon = request.form.get("ed_emon")
        edetue = request.form.get("ed_etue")
        edewed = request.form.get("ed_ewed")
        edethu = request.form.get("ed_ethu")
        edefri = request.form.get("ed_efri")
        edesat = request.form.get("ed_esat")

        try:
            db.execute("INSERT INTO eschedule (edsun, edmon, edtue, edwed, edthu, edfri, edsat, sun, mon, tue, wed, thu, fri, sat) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", edsun, edmon, edtue, edwed, edthu, edfri, edsat, esun, emon, etue, ewed, ethu, efri, esat)
        except:
            return apology("can't get values")
        return redirect ("/manager_table")
    else:
        return render_template("manager_table.html")

@app.route("/up_to_date", methods=["GET", "POST"])
@login_required
def up_to_date():
    if request.method == "POST":
        trigger = request.form.get("trigger")
        db.execute("DELETE FROM myshifts WHERE date < ? ", trigger)

        return redirect ("/manager_table")
    else:
        return render_template("manager_table.html")

@app.route("/worker_table", methods=["GET", "POST"])
@login_required
def worker_table():

    if request.method == "POST":


        return render_template("worker_table.html")
    else:
        # finde worker's name
        rows = db.execute("SELECT name FROM employees WHERE id = ?", session["session_id"])
        worker_name = rows[0]["name"]
        return render_template("worker_table.html", worker_name = "Hello dear "+ worker_name)

@app.route("/myshifts", methods=["GET", "POST"])
@login_required
def myshifts():
    """send response"""
    if request.method == "POST":
        sun = request.form.get("sunday")
        if not sun:
            return apology("what on sunday?")
        mon = request.form.get("monday")
        if not mon:
            return apology("what on monday?")
        tue = request.form.get("tuesday")
        if not tue:
            return apology("what on tuesday?")
        wed = request.form.get("wednesday")
        if not wed:
            return apology("what on wednesday?")
        thu = request.form.get("thursday")
        if not thu:
            return apology("what on thursday?")
        fri = request.form.get("friday")
        if not fri:
            return apology("what on friday?")
        sat = request.form.get("saturday")
        if not sun:
            return apology("what on saturday?")
        employee = session["session_id"]
        exp = request.form.get("exp")
        if not exp:
            return apology("what what more?")
        try:
            db.execute("INSERT INTO myshifts (employee, sun, mon, tue, wed, thu, fri, sat, exp) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)", employee, sun, mon, tue, wed, thu, fri, sat, exp)
        except:
            return apology("looks like something wrong")
        # finde previous request
        rows = db.execute("SELECT * FROM myshifts WHERE employee = ? ORDER BY id ASC", session["session_id"])
        # delete previous request
        if len(rows) != 1:
            db.execute("DELETE FROM myshifts WHERE id = ?", rows[0]["id"])
            # Redirect  to manager table
            return redirect("/worker_table")
    else:
        return render_template("worker_table.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

