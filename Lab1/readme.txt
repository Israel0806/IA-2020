Se compilo usando en C++ en ZinjaI usando la libreria glut para OpenGL
-------
Al correrlo aparecerán los puntos del grafo dibujados(incluyendo el 20% eliminados), luego le da ENTER en la interfaz gráfica
y  en la consola le pedirá los Nodos Iniciales que se coloca de la forma: 0 0
Le da ENTER y luego de la misma forma coloca los nodos objetivos,
en caso uno de los nodos ya no exista o se haya removido(se eliminó aleatoriamente el 20%) mostrará el nodo eliminado,
de lo contrario mostrará dos caminos usados, uno por amplitud(color verde) que es el más largo y el A*(color azul) que es el más directo.
Luego para ingresar otra búsqueda de caminos se da ENTER a la interfaz gráfica y en el cmd le pedirá de nuevo los nodos iniciales y objetivo
Comandos:
tecla "esc" par salir del programa
tecla "b" para mostrar las aristas
tecla "ENTER" para buscar un camino entre dos coordenadas