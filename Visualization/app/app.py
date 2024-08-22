from uuid import uuid4
import requests
from urllib.parse import urlencode
from flask import Flask, request, session, redirect, url_for, render_template

import json

app = Flask(__name__)


def load_output(file):
    with open(file, 'r') as f:
        lines = f.readlines()
    lines.reverse()

    parent_nodes = {}
    nodes = []

    translation_table = str.maketrans('01', '10')
    for line in lines:
        line = json.loads(line)
        parent_nodes[line['depth']] = line
        line['children'] = []
        line['is_best'] = False
        line['mystate'] = line['mystate'].translate(translation_table).zfill(361)
        line['otherstate'] = line['otherstate'].translate(translation_table).zfill(361)
        if line['depth'] == 3:
            line['is_best'] = True
            nodes.append(line)
        else:
            if line['orig_move'] == parent_nodes[line['depth'] + 1]['best_child'] and parent_nodes[line['depth'] + 1]['is_best']:
                line['is_best'] = True
            parent_nodes[line['depth'] + 1]['children'].append(line)
    return nodes


def reverse_children(items):
    for item in items:
        if 'children' in item:
            item['children'].reverse()
            reverse_children(item['children'])


@app.route('/')
def index():
    items = load_output("../output")
    reverse_children(items)
    return render_template('nodes.html', items=items)
