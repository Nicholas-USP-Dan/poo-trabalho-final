# TESTE DA COMUNICAÇÃO ENTRE O PROGRAMA EM C E PYTHON

import subprocess
import csv
import json
import random

socket_id = random.randint(0,100000)

with open(f'select-response-{socket_id}.csv', 'w+') as csv_file, open(f'select-responce-{socket_id}.json', 'w') as json_file:
    p1 = subprocess.run(["c-api/bin/program", "select", "c-api/test-files/binario9.bin"], input=b'1 nacionalidade SPAIN', stdout=csv_file)
    if p1.returncode != 0:
        exit(p1.returncode)
    csv_file.flush()
    csv_file.seek(0)
    csv_reader = csv.DictReader(csv_file)
    json.dump([row for row in csv_reader], json_file)
