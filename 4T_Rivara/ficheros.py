import numpy as np
import math as mt
import matplotlib.pyplot as plt

#Calculamos distancia ingresando 2 vertices casteados en enteros y la lista lnodos
def cal_dist(v1,v2,lnodos):
	dist=mt.sqrt(((float(lnodos[v1][1])-float(lnodos[v2][1]))*(float(lnodos[v1][1])-float(lnodos[v2][1])))+((float(lnodos[v1][2])-float(lnodos[v2][2]))*(float(lnodos[v1][2])-float(lnodos[v2][2]))))
	return dist
#Calculamos los angulos con arcoseno ingresando 3 distancias
def angulos(a,b, c):
	lista= [None] * 3
	lista[0]=(mt.acos((c*c-a*a-b*b)/(-2*a*b)))*180/mt.pi
	lista[1]=(mt.acos((a*a-b*b-c*c)/(-2*b*c)))*180/mt.pi
	lista[2]=(mt.acos((b*b-a*a-c*c)/(-2*a*c)))*180/mt.pi
	return lista

#Genera los 3 angulos a todos los triangulos de toda la malla y se los agrega a la lista lelementos
def cal_ang(lnodos,lelementos):
	lista=[None] *3
	for i in range(1,int(lelementos[0][0])+1):
			lista=angulos(cal_dist(int(lelementos[i][1]),int(lelementos[i][2]),lnodos),cal_dist(int(lelementos[i][1]),int(lelementos[i][3]),lnodos),cal_dist(int(lelementos[i][2]),int(lelementos[i][3]),lnodos))
			lelementos[i].append(lista[0])
			lelementos[i].append(lista[1])
			lelementos[i].append(lista[2])

def cal_ang_solo(lnodos, lelementos):
	lista=[]
	lista=angulos(cal_dist(int(lelementos[-1][1]),int(lelementos[-1][2]),lnodos),cal_dist(int(lelementos[-1][1]),int(lelementos[-1][3]),lnodos),cal_dist(int(lelementos[-1][2]),int(lelementos[-1][3]),lnodos))
	lelementos[-1].append(lista[0])
	lelementos[-1].append(lista[1])
	lelementos[-1].append(lista[2])

#Criterio de refinamiento, si algun angulo del un triangulo es menor al angulo critico se le asigna un 1 de refinar en contraparte 0 de no refinar
def crit_ref(lelementos, crit_ang):
	cant_ref=0
	for i in range(1,int(lelementos[0][0])+1):
		if lelementos[i][4] < crit_ang or lelementos[i][5] < crit_ang or lelementos[i][6] < crit_ang:
			cant_ref=cant_ref+1
			lelementos[i].append(1)
		else:
			lelementos[i].append(0)
	return cant_ref				

#En esta funcion determinamos las distancias mayores y asignaermos los vertices a esa distancia mayor
def arista_larga(lelementos):
	for i in range(1,int(lelementos[0][0])+1):
		lista=[None] * 3
		if (cal_dist(int(lelementos[i][1]),int(lelementos[i][2]),lnodos) > cal_dist(int(lelementos[i][1]),int(lelementos[i][3]),lnodos)) and (cal_dist(int(lelementos[i][1]),int(lelementos[i][2]),lnodos) > cal_dist(int(lelementos[i][2]),int(lelementos[i][3]),lnodos)):
			lista[0]=cal_dist(int(lelementos[i][1]),int(lelementos[i][2]),lnodos)
			lista[1]=lelementos[i][1]
			lista[2]=lelementos[i][2]
			lelementos[i].append(lista)
		elif (cal_dist(int(lelementos[i][1]),int(lelementos[i][3]),lnodos) > cal_dist(int(lelementos[i][1]),int(lelementos[i][2]),lnodos)) and (cal_dist(int(lelementos[i][1]),int(lelementos[i][3]),lnodos) > cal_dist(int(lelementos[i][2]),int(lelementos[i][3]),lnodos)):
			lista[0]=cal_dist(int(lelementos[i][1]),int(lelementos[i][3]),lnodos)
			lista[1]=lelementos[i][1]
			lista[2]=lelementos[i][3]
			lelementos[i].append(lista)
		elif (cal_dist(int(lelementos[i][2]),int(lelementos[i][3]),lnodos) > cal_dist(int(lelementos[i][1]),int(lelementos[i][2]),lnodos)) and (cal_dist(int(lelementos[i][2]),int(lelementos[i][3]),lnodos) > cal_dist(int(lelementos[i][1]),int(lelementos[i][3]),lnodos)):
			lista[0]=cal_dist(int(lelementos[i][2]),int(lelementos[i][3]),lnodos)
			lista[1]=lelementos[i][2]
			lista[2]=lelementos[i][3]
			lelementos[i].append(lista)


def arista_larga_solo(lelementos):
		lista=[None] * 3
		if (cal_dist(int(lelementos[-1][1]),int(lelementos[-1][2]),lnodos) > cal_dist(int(lelementos[-1][1]),int(lelementos[-1][3]),lnodos)) and (cal_dist(int(lelementos[-1][1]),int(lelementos[-1][2]),lnodos) > cal_dist(int(lelementos[-1][2]),int(lelementos[-1][3]),lnodos)):
			lista[0]=cal_dist(int(lelementos[-1][1]),int(lelementos[-1][2]),lnodos)
			lista[1]=lelementos[-1][1]
			lista[2]=lelementos[-1][2]
			lelementos[-1].append(lista)
		elif (cal_dist(int(lelementos[-1][1]),int(lelementos[-1][3]),lnodos) > cal_dist(int(lelementos[-1][1]),int(lelementos[-1][2]),lnodos)) and (cal_dist(int(lelementos[-1][1]),int(lelementos[-1][3]),lnodos) > cal_dist(int(lelementos[-1][2]),int(lelementos[-1][3]),lnodos)):
			lista[0]=cal_dist(int(lelementos[-1][1]),int(lelementos[-1][3]),lnodos)
			lista[1]=lelementos[-1][1]
			lista[2]=lelementos[-1][3]
			lelementos[-1].append(lista)
		elif (cal_dist(int(lelementos[-1][2]),int(lelementos[-1][3]),lnodos) > cal_dist(int(lelementos[-1][1]),int(lelementos[-1][2]),lnodos)) and (cal_dist(int(lelementos[-1][2]),int(lelementos[-1][3]),lnodos) > cal_dist(int(lelementos[-1][1]),int(lelementos[-1][3]),lnodos)):
			lista[0]=cal_dist(int(lelementos[-1][2]),int(lelementos[-1][3]),lnodos)
			lista[1]=lelementos[-1][2]
			lista[2]=lelementos[-1][3]
			lelementos[-1].append(lista)

#Con esto limitamos que el punto medio se agregue a lnodos SI Y SOLO SI ESTE CONSTRUYA UN TRIANGULO NUEVO
def agregar_vertice_lnodos(v1,v2,lnodos):
	a=int(lnodos[0][0])+1
	lnodos[0][0]=a
	lnodos.append([a])
	lnodos[a].append(v1)
	lnodos[a].append(v2)

#Calcula el punto medio			
def pto_mdo(v1, v2):
	v1=int(v1)
	v2=int(v2)
	lista=[None] * 2
	lista[0]=(float(lnodos[v1][1])+float(lnodos[v2][1]))/2
	lista[1]=(float(lnodos[v1][2])+float(lnodos[v2][2]))/2
	return lista

#Se le sacara punto medio a todos los triangulos, pero se analizara despues si es necesario sacar punto medio a la arista mayor de todos los T
def asig_pto_mdo(lelementos):
	lista=[None] * 2
	for i in range(1,int(lelementos[0][0])+1):
		lista=pto_mdo(int(lelementos[i][8][1]),int(lelementos[i][8][2]))
		lelementos[i].append(lista)

def pto_mdoa_ele(lelements,indice,lnodes):
	i=0
	while (lelementos[indice][9][0] == lnodos[i][1] and lelementos[indice][9][1] == lnodos[i][2]) or (i==lnode[0][0]):
		return int(lnodos[i][0])

def crear_indice(lelementos):
	
	ult_crea=int(lelementos[-1][0])+1
	lelementos[0][0]=int(lelementos[0][0])+1
	#print lelementos[-1][0]
	lelementos.append([ult_crea])
def crear_triangulo(v1, v2, v3,lelementos):
	#Esta lista genera los tres vertices y el numero del triangulo si es refinable 1 y 0
	lista=[None] * 3
	#Genera 3 angulos
	#genera la distancia mas larga y los vertices asociados
	lista3=[None] * 3
	#Genera el punto medio de la arista mayor
	lista4=[None] * 2

	lista[0]=v1
	lista[1]=v2
	lista[2]=v3

	crear_indice(lelementos)
	#se agregaran los 3 vertices en las casillas de la fila nueva -lista[0]-

	lelementos[-1].append(lista[0])
	lelementos[-1].append(lista[1])
	lelementos[-1].append(lista[2])

	cal_ang_solo(lnodos,lelementos)
	"""#Se agregaran los 3 angulos
	lelementos[lelementos[0][0]].append(lista2[0])
	lelementos[lelementos[0][0]].append(lista2[1])
	lelementos[lelementos[0][0]].append(lista2[2])"""
	#No entra al criterio de refinamiento, pero si al de conformidad
	lelementos[-1].append(0)
	lista3=arista_larga_solo(lelementos)
	#se llena la casilla 8 con la distancia mayor y los dos vertices que contienen la arista mayor
	#Se genera 
	lista4=pto_mdo(lelementos[-1][8][1],lelementos[-1][8][2])
	lelementos[-1].append(lista4)

	pto_opuesto_solo(lelementos)

#Recorremos la matriz de triangulos y preguntamos el primer elemento de cada fila, este nos comparara el num
# de triangulo que buscamos, para luego borrarlo	
def return_indice_ele(lelementos, num_triangulo):
	for i in range(1,int(lelementos[0][0])+1):
		if int(lelementos[i][0]) == num_triangulo:
			return i
	return 0


#REVISAR PUNTO MEDIO, el i es el indice al triangulo que se encontro a refinar -valor 1 a refinar-
def cuatro_t(lelementos, i):
	agregar_vertice_lnodos(lelementos[i][9][0],lelementos[i][9][1],lnodos)
	pto_mdo_uno=pto_mdo(lelementos[i][10],lelementos[i][8][1])
	pto_mdo_dos=pto_mdo(lelementos[i][10],lelementos[i][8][2])
	pto_mdo_mayor=int(lnodos[-1][0])

	agregar_vertice_lnodos(pto_mdo_uno[0],pto_mdo_uno[1],lnodos)
	vertice_pto_mdo_uno=int(lnodos[-1][0])
	
	agregar_vertice_lnodos(pto_mdo_dos[0],pto_mdo_dos[1],lnodos)
	vertice_pto_mdo_dos=int(lnodos[-1][0])

	crear_triangulo(pto_mdo_mayor,lelementos[i][8][1],vertice_pto_mdo_uno,lelementos)
	crear_triangulo(pto_mdo_mayor,lelementos[i][8][2],vertice_pto_mdo_dos,lelementos)
	crear_triangulo(pto_mdo_mayor,lelementos[i][10],vertice_pto_mdo_uno,lelementos)
	crear_triangulo(pto_mdo_mayor,lelementos[i][10],vertice_pto_mdo_dos,lelementos)
	lelementos.pop(i)
	lelementos[0][0]=lelementos[0][0]-1

def pto_opuesto_solo(lelementos):
	if (lelementos[-1][1] != lelementos[-1][8][1]) and (lelementos[-1][1] != lelementos[-1][8][2]):
		lelementos[-1].append(lelementos[-1][1])
	elif (lelementos[-1][2] != lelementos[-1][8][1]) and (lelementos[-1][2] != lelementos[-1][8][2]):
		lelementos[-1].append(lelementos[-1][2])
	elif (lelementos[-1][3] != lelementos[-1][8][1]) and (lelementos[-1][3] != lelementos[-1][8][2]):
		lelementos[-1].append(lelementos[-1][3])
#Calculamos punto opuesto a la arista mayor

def pto_opuesto(lelementos):
	for i in range(1,int(lelementos[0][0])+1):
		if (lelementos[i][1] != lelementos[i][8][1]) and (lelementos[i][1] != lelementos[i][8][2]):
			lelementos[i].append(lelementos[i][1])
		elif (lelementos[i][2] != lelementos[i][8][1]) and (lelementos[i][2] != lelementos[i][8][2]):
			lelementos[i].append(lelementos[i][2])
		elif (lelementos[i][3] != lelementos[i][8][1]) and (lelementos[i][3] != lelementos[i][8][2]):
			lelementos[i].append(lelementos[i][3])

def leer_node():
	#Lee el fichero .node y lo asigna a la lista lnodos
	lineas = open("africa.node").readlines()
	lnodos = [[m.strip() for m in n] for n in [linea.split(" ") for linea in lineas]]
	return lnodos	
def ultimo_indice_fijo(lnodes):
	ult_indice = lnodes[-1][0]
	return int(ult_indice)

def leer_ele():	
	#Lee el fichero .ele y lo asigna a la lista lelementos
	lineas = open("africa.ele").readlines()
	lelementos = [[m.strip() for m in n] for n in [linea.split(" ") for linea in lineas]]
	return lelementos

def anadir_linea_ele(lelementos):
	fp=open('africa.ele','a')
	fp.write('\n1')
	fp.close()

def ele_a_pc(lelementos):
	fp = open("africarf.ele","w+")
	#contenido = archivo.read()
	#indice = [lelementos[0][0]]

	fp.write(str(lelementos[0][0]))
	fp.write(" ")
	fp.write(str(3))
	for i in range(1,int(lelementos[0][0])+1):
		fp.write(str("\n"))
		fp.write(str(lelementos[i][0]))
		fp.write(" ")
		fp.write(str(lelementos[i][1]))
		fp.write(" ")
		fp.write(str(lelementos[i][2]))
		fp.write(" ")
		fp.write(str(lelementos[i][3]))
	fp.close()

def node_a_pc(lnodos):
	fp = open("africarf.node","w+")
	#contenido = archivo.read()
#	indice = []
	fp.write(str(lnodos[0][0]))
	fp.write(" ")
	fp.write(str(2))
	for i in range(1,int(lnodos[0][0])+1):
		fp.write("\n")
		fp.write(str(lnodos[i][0]))
		fp.write(" ")
		fp.write(str(lnodos[i][1]))
		fp.write(" ")
		fp.write(str(lnodos[i][2]))
	fp.close()

def part_a_pc(lelementos):
	fp = open("africarf.part","w+")
	#contenido = archivo.read()
	fp.write(str(lelementos[0][0]))
	fp.write(" ")
	fp.write(str(1))
	for i in range(1,int(lelementos[0][0])+1):
		fp.write("\n")
		fp.write(str(lelementos[i][0]))
		fp.write(" ")
		fp.write(str(1))
	fp.close()


def conformidad(lelementos,lnodos,vertices_iniciales, ultimo_indice):
	i=1
	while (i <= int(lelementos[0][0])):
		if comp_pto_mdo(pto_mdo(int(lelementos[i][1]),int(lelementos[i][2])),pto_mdo(int(lelementos[i][1]),int(lelementos[i][3])),pto_mdo(int(lelementos[i][2]),int(lelementos[i][3])),lnodos,vertices_iniciales, ultimo_indice) == 1:
			#print i
			print lelementos[i][0] 
			agregar_vertice_lnodos(lelementos[i][9][0],lelementos[i][9][1],lnodos)
			pto_mdo_mayor=int(lnodos[-1][0])
			crear_triangulo(pto_mdo_mayor,lelementos[i][8][1],lelementos[i][10],lelementos)
			crear_triangulo(pto_mdo_mayor,lelementos[i][8][2],lelementos[i][10],lelementos)
			lelementos.pop(i)
			lelementos[0][0]=lelementos[0][0]-1
			i=1 
		else:	
			i=i+1
			
#Entran 3 listas las cuales vamos a comprar con los lnodos
def comp_pto_mdo(pmdo_uno, pmdo_dos,pmdo_tres, lnodos,vertices_iniciales, ultimo_indice):
	for i in range(ultimo_indice,lnodos[0][0]):
		if (pmdo_uno[0] == lnodos[i][1] and pmdo_uno[1] == lnodos[i][2]) or (pmdo_dos[0] == lnodos[i][1] and pmdo_dos[1] == lnodos[i][2]) or (pmdo_tres[0] == lnodos[i][1] and pmdo_tres[1] == lnodos[i][2]):
			return 1
	return 0


i=1
#Recopilacion de datos
contador=0
lnodos=leer_node()
uif = ultimo_indice_fijo(lnodos)
#print uif
lelementos=leer_ele()
cal_ang(lnodos,lelementos)
cant_r=crit_ref(lelementos,39)
arista_larga(lelementos)
asig_pto_mdo(lelementos)
pto_opuesto(lelementos)

#for ele in lelementos:
#	print

while i <= int(lelementos[0][0]):
	#print i
	if lelementos[i][7] == 1:
		cuatro_t(lelementos, return_indice_ele(lelementos,int(lelementos[i][0])))
		vertices_iniciale=[None] * 3
		vertices_iniciale[0]=lelementos[i][1]
		vertices_iniciale[1]=lelementos[i][2]
		vertices_iniciale[2]=lelementos[i][3]
		conformidad(lelementos,lnodos,vertices_iniciale, uif)	
		contador=contador+1
		i=1
	else:
		i=i+1	
#cuatro_t(lelementos,return_indice_ele(lelementos,1))

#cuatro_t(lelementos,return_indice_ele(lelementos,5))

#cuatro_t(lelementos,return_indice_ele(lelementos,2))

"""for i in range(1,int(lnodos[0][0])+1):
	print lnodos[i][0]," ",lnodos[i][1], " ", lnodos[i][2]


for i in range(1,int(lelementos[0][0])+1):
	print lelementos[i][0]," ",lelementos[i][1], " ", lelementos[i][2], " ", lelementos[i][3], " ", lelementos[i][7]

print ""
print "Se van a refinar: ",cant_r, " triangulos."
print ""
print "Se refinaron: ", contador, " triangulos."	"""
ele_a_pc(lelementos)
node_a_pc(lnodos)
part_a_pc(lelementos)