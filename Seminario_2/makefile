compilador:=g++
opcionesc:= -std=c++11 -pthread -Wfatal-errors -I.
hmonsrcs:= HoareMonitor.hpp HoareMonitor.cpp 


prodcons1_sc_FIFO.out: prodcons1_sc_FIFO.cpp
	$(compilador) $(opcionesc)  -o $@ $<

prodcons2_sc.out: prodcons2_sc.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera2_sc.out: barrera2_sc.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera2_su.out: barrera2_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp

prodcons1_sc.out: prodcons1_sc.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<


prodcons2_su.out: prodcons2_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp

prodcons2_su_FIFO.out: prodcons2_su_FIFO.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp


clean:
	rm -f *.out
