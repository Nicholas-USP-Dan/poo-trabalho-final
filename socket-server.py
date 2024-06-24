import subprocess
import csv
import json
import socket
import threading
import secrets

# Chamada da função create na API do arquivo
def C_create(req, session_tok: str):
    print(f"[CREATING FILE] Creating data-{session_tok}.bin")
    p = subprocess.run(["c-api/bin/program", "create", f"c-api/{req["database"]}", f"data-{session_tok}.bin", f"indice-{session_tok}.bin"])
    if p.returncode != 0:
        print("Erro durante execução de criação do arquivo!")
        return
    print("Comando executado com sucesso")

# Chamada da função select na API do arquivo
# tomar já que temos o envio de muitos dados
def C_select(req, session_tok: str):
    query = req["query"]
    req_query = (f"{ query["id"] } "
                f"{ query["idade"] if query["idade"] >= 0 else "NULO" } "
                f"{ query["nomeJogador"] if query["nomeJogador"] else "NULO" } " 
                f"{ query["nacionalidade"] if query["nacionalidade"] else "NULO" } "
                f"{ query["nomeClube"] if query["nomeClube"] else "NULO" }"
                )
    with open(f'select-out-{session_tok}.csv', 'w+') as csv_file, open(f'select-out-{session_tok}.json', 'w') as json_file:
        p = subprocess.run(["c-api/bin/program", "select", f"data-{session_tok}.bin"], input=req_query.encode('utf-8'), stdout=csv_file)
        if p.returncode != 0:
            print("Erro durante execução de criação do arquivo!")
            return
        print("Convertendo arquivo .csv para .json")
        
        csv_file.flush()
        csv_file.seek(0)
        csv_reader = csv.DictReader(csv_file)
        json.dump([row for row in csv_reader], json_file)

        print("Enviar arquivo para a interface")

        print("Comando executado com sucesso")

# Chamada da função edit na API do arquivo
def C_edit(req, session_tok: str):
    query = req["query"]
    req_query = (f"{ query["id"] } "
                f"{ query["idade"] if query["idade"] >= 0 else "NULO" } "
                f"{ query["nomeJogador"] if query["nomeJogador"] else "NULO" } " 
                f"{ query["nacionalidade"] if query["nacionalidade"] else "NULO" } "
                f"{ query["nomeClube"] if query["nomeClube"] else "NULO" }"
                )
                
    p = subprocess.run(["c-api/bin/program", "edit", f"data-{session_tok}.bin", f"indice-{session_tok}.bin", f"{query["id"]}"], input=req_query.encode('utf-8'))
    if p.returncode != 0:
        print("Erro durante execução de criação do arquivo!")
        return

    print("Comando executado com sucesso")

# Chamada da função delete na API do arquivo
def C_delete(req, session_tok: str):
    print("hi")

c_calls = {
    "CREATE": C_create,
    "SELECT": C_select,
    "EDIT": C_edit,
    "DELETE": C_delete,
}

def handle_client_thread(conn: socket.socket, addr, session_tok: str):
    print(f"[NEW CONNECTION] {addr} connected (session_tok: {session_tok})")
    
    while True:
        msg = conn.recv(1024)

        if not msg:
            break

        try:
            resp = json.loads(msg.decode('utf-8'))

            op = resp["operation"]
            if op == "CLOSE":
                print(f"[CLOSE CONNECTION] Close connection with {addr}")
                conn.close()
                return
            elif op in c_calls:
                c_calls[op](resp, session_tok)
            else:
                print("Invalid operation!")

        except json.JSONDecodeError as err:
            print("Invalid JSON syntax: ", err)

        except KeyError as err:
            print("Invalid request; missing parameter: ", err)

    print(f"[CLOSE CONNECTION] Close connection with {addr}")

print("[STARTING] Starting server...")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(('127.0.0.1', 5050))
    s.listen()
    while True:
        conn, addr = s.accept()
        session_tok = secrets.token_urlsafe(16)
        thread = threading.Thread(target=handle_client_thread, args=(conn, addr, session_tok))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count()-1}, {conn.getsockname()}")



# Criar o arquivo .csv para requests de select
# with open(f'select-response-{socket_id}.csv', 'w+') as csv_file, open(f'select-responce-{socket_id}.json', 'w') as json_file:
#     p1 = subprocess.run(["c-api/bin/program", "select", "c-api/test-files/binario9.bin"], input=b'1 nacionalidade SPAIN', stdout=csv_file)
#     if p1.returncode != 0:
#         exit(p1.returncode)
#     csv_file.flush()
#     csv_file.seek(0)
#     csv_reader = csv.DictReader(csv_file)
#     json.dump([row for row in csv_reader], json_file)
