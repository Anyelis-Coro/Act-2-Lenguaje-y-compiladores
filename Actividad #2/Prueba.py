import hashlib
import random
from datetime import datetime

class Nodo:
    def __init__(self, partida, cuerpo):
        self.partida = partida
        self.cuerpo = cuerpo
        # Generar firma digital concatenando partida con los elementos del cuerpo
        contenido_firma = partida + " " + " ".join(map(str, cuerpo))
        self.firma = hashlib.sha256(contenido_firma.encode()).hexdigest()

def generar_lista(n, k):
    lista = []
    # Para el primer nodo, generamos partida con fecha/hora actual
    if n > 0:
        fecha = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
        partida_actual = hashlib.sha256(fecha.encode()).hexdigest()
        
        for _ in range(n):
            cuerpo = [random.randint(1, 100000) for _ in range(k)]
            nodo = Nodo(partida_actual, cuerpo)
            lista.append(nodo)
            # La partida para el próximo nodo es la firma del nodo actual
            partida_actual = nodo.firma
    return lista

def ejecutar_escenarios():
    escenarios = [
        {"n": 3, "k": 4},
        {"n": 10, "k": 200},
        {"n": 200, "k": 10}
    ]
    
    resultados = []
    
    for escenario in escenarios:
        n = escenario["n"]
        k = escenario["k"]
        
        print(f"\nEjecutando escenario: n={n}, k={k}")
        inicio = datetime.now()
        
        lista = generar_lista(n, k)
        
        fin = datetime.now()
        tiempo_ejecucion = (fin - inicio).total_seconds() * 1000  # milisegundos
        
        print(f"Tiempo de ejecución: {tiempo_ejecucion:.2f} ms")
        
        # Mostrar información de los primeros 2 nodos para verificación
        if len(lista) > 0:
            print("\nPrimer nodo:")
            print(f"Partida: {lista[0].partida[:12]}...")
            print(f"Cuerpo (muestra): {lista[0].cuerpo[:5]}... (total {len(lista[0].cuerpo)} elementos)")
            print(f"Firma: {lista[0].firma[:12]}...")
            
            if len(lista) > 1:
                print("\nSegundo nodo:")
                print(f"Partida (debe ser igual a firma anterior): {lista[1].partida[:12]}...")
                print(f"Cuerpo (muestra): {lista[1].cuerpo[:5]}...")
                print(f"Firma: {lista[1].firma[:12]}...")
        
        resultados.append({
            "n": n,
            "k": k,
            "tiempo_ms": tiempo_ejecucion
        })
    
    return resultados

# Ejecutar todos los escenarios
resultados = ejecutar_escenarios()

# Mostrar tabla comparativa
print("\nTabla Comparativa de Resultados:")
print("| Escenario |   n  |  k  | Tiempo (ms) |")
print("|-----------|------|-----|-------------|")
for i, res in enumerate(resultados):
    print(f"|    {chr(97+i)}      | {res['n']:4} | {res['k']:3} | {res['tiempo_ms']:11.2f} |")