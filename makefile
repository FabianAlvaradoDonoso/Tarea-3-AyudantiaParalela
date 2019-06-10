#Variables

CXX = mpic++
CXXR = mpirun
CXXFLAGS = --oversubscribe

FUENTE = paralela.cpp
NOMBRE = ejecutable.out



#Reglas


tarea: $(FUENTE)
	$(CXX) $(FUENTE) -o $(NOMBRE)
	clear
	$(CXXR)  $(CXXFLAGS) -np $(shell bash -c 'read  -p "Cantidad -np: " pwd; echo $$pwd') $(NOMBRE)

clear:
	rm $(NOMBRE)
