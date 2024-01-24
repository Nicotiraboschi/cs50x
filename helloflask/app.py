from flask import Flask, render_template, request

app = Flask(__name__)
list_colors = ["blue"]


@app.route("/", methods=["GET", "POST"])
def index():
    color = request.form.get("color")
    if request.method == "GET":
        return render_template("index.html")
    else:
        if color in list_colors:
            print("Form submitted!")
            return render_template("color.html", color=color)
        else:
            return render_template("index.html")
