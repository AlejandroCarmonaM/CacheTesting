# CacheTesting

El objetivo de este repositorio es mostrar un ejemplo de programas escritos en C++ capaces de estimar la velocidad (ancho de banda) de un sistema de caché. Al estar escritos en C++ en lugar de ensamblador, pueden no ser tan exactos como si lo serían otras aplicaciones disponibles en el mercado como CPU-X, sin embargo, la finalidad de estos programas es mostrar cómo las distintas interacciones con la memoria caché pueden tener efectos significativos en el ancho de banda y, por consiguiente, en el rendimiento de una determinada aplicación.

Para ejecutar los tests existe un makefile que compila todos los programas, pero que por defecto compila el programa de nombre "peak-mem-cache" usando el compilador gcc. Con el objetivo de facilitar la ejecución, se proporciona un script para el compilador gcc (gcc-run-peak-mem.sh) en el cual se puede sustituir el peak-mem-cache predeterminado (peak-mem-random-read-64-a), por alguno de los programas disponibles o ir sobreescribiendo el archivo peak-mem. Se recomienda usar gcc ya que es el compilador que proporciona los resultados más coherentes y precisos, aunque puede depender de la máquina.

Se adjunta un archivo pdf que hace las labores de informe analizando los resultados de los distintos tests y comparándolos con el programa CPU-X.

Estos programas se basan en el programa "peak-mem" provisto por el departamento Departamento de Ingeniería y Tecnología de Computadores de la Facultad de Informática de la Universidad de Murcia cuyo propósito era calcular aproximadamente el ancho de banda del sistema de memoria completo, incluyendo RAM, obtenido al realizar una copia de 1 GiB.

Los archivos util.cpp y util.h también fueron provistos por el departamento Departamento de Ingeniería y Tecnología de Computadores de la Facultad de Informática de la Universidad de Murcia.
