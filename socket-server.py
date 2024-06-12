# TESTE DA COMUNICAÇÃO ENTRE O PROGRAMA EM C E PYTHON

import subprocess
import csv
import json

with open('select-response-SOCKETID.csv', 'w') as csv_file:
    csv_file.write('id,idade,nomeJogador,nacionalidade,nomeClube\n')
    csv_file.flush()
    p1 = subprocess.run(["c-api/bin/program", "select", "c-api/binario9.bin"], input=b'0', stdout=csv_file)
    
with open('select-response-SOCKETID.csv', 'r') as csv_file, open('select-responce-SOCKETID.json', 'w') as json_file:
    csv_reader = csv.DictReader(csv_file)
    json.dump([row for row in csv_reader], json_file)
