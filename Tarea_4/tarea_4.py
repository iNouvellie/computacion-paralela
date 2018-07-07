from mpi4py import MPI
import random
import math
from time import time
import numpy as np

#Definimos la funcion, para verificar que el numero aleatorio no se repita 0 al 59
def no_repetir_numero(numero_aleatorio, tamano_vector,lista):
	for i in range(tamano_vector):
		if numero_aleatorio == lista[i].numero_bola-1:
			return 0
	return 1		

#Definimos una funcion para verificar si el numer aleatorio se repite, para marcarlo en el carton 1 al 60
def repite_numero(numero,lista):
	for i in range(30):
		if numero == lista[i].numero_bola:
			return 1
	return 0		
#Creamos una clase, para definir el numero y color de la bola
class bola_numero:
	def __init__(self, numero, color):
		self.numero_bola = numero
		self.color_bola = color

#Carton
class carton:
	def __init__(self, numero, esta_marcado):
		self.numero_carton = numero
		self.marcado_carton = esta_marcado
#Rellenamos el vector con numero y color
def rellenar_bolas():
	lista=[0] *60
	for i in range(60):
		if i < 14:
			lista[i]=bola_numero(i+1,'amarillo')	
		elif i < 29:
			lista[i]=bola_numero(i+1,'azul')
		elif i < 44:
			lista[i]=bola_numero(i+1,'rojo')
		elif i < 60:
			lista[i]=bola_numero(i+1,'verde')		
	return lista

def rellenar_distribucion_bolas():
	lista=[0] *30
	for i in range(30):
		numero=np.random.random_integers(60)-1
		while no_repetir_numero(numero,i,lista) == 0:
			numero=np.random.random_integers(60)-1
		if numero < 14:
			lista[i]=bola_numero(numero+1,'amarillo')
		elif numero < 29:
			lista[i]=bola_numero(numero+1,'azul')
		elif numero < 44:
			lista[i]=bola_numero(numero+1,'rojo')
		elif numero < 60:
			lista[i]=bola_numero(numero+1,'verde')
	return lista

def crear_carton():
	matriz=np.zeros((3, 5))
	for i in range(3):
		for j in range(5):
			matriz[i][j]=carton(np.random.random_integers(60),'no')
	return matriz

def marcar_carton(bolas,matrtz):
	for i in range(3):
		for j in range(5):
			if repetir_numero(matriz[i][j].numero_carton,30,bolas)==1:
				matriz[i][j].marcado_carton='si'	
	return matriz
comm = MPI.COMM_WORLD

#Conocemos el numero de nodos totales
size = comm.Get_size()
rank = comm.Get_rank()

root=0
bolas=[]
distribucion_bolas=[]
carton=[]
if rank==root:
	#bolas=rellenar_bolas()
	#for i in range(60):
	#	print bolas[i].numero_bola,bolas[i].color_bola
	distribucion_bolas=rellenar_distribucion_bolas()
	for i in range(30):
		print distribucion_bolas[i].numero_bola,distribucion_bolas[i].color_bola, i+1	
	carton1=crear_carton()		
	carton2=crear_carton()
	carton3=crear_carton()
	carton4=crear_carton()
	carton=marcar_carton(distribucion_bolas,carton)
	for i in range(3):
		for j in range(5):
			print carton1[i][j].numero_carton,carton1[i][j].marcado_carton

