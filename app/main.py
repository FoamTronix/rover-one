from flask import Flask
from flask import render_template

app = Flask(__name__)

@app.route('/')
@app.route('/<msg>')
def index(msg=None):
    return render_template('index.html', msg = msg)

if __name__ == "__main__":
    app.run(host='192.168.0.17')
